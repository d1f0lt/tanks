#include "model/handler.h"
#include <model/game_model.h>
#include <cassert>
#include <memory>
#include <unordered_set>
#include "model/blocks.h"
#include "model/bonus.h"
#include "model/projectile.h"
#include "model/tank.h"

namespace Tanks::model {
BasicHandler::BasicHandler(GameModel &model_, Entity &entity_)
    : model_(model_), entity_(entity_) {
    assert(model_.handlers_.count(&entity_) == 0);
    model_.handlers_[&entity_] = this;
}

Entity &BasicHandler::getEntity() const {
    return entity_;
}

GameModel &BasicHandler::getModel() const {
    return model_;
}

BasicHandler::~BasicHandler() {
    getModel().handlers_.erase(&getEntity());
}

bool BasicHandler::canStandOn(const Entity &other) const {
    return other.isTankPassable();
}

void BasicHandler::destroyByBullet() {
    assert(getEntity().getStrength() != 0);
    int left = getEntity().getLeft();
    int top = getEntity().getTop();
    int id = getEntity().getId();
    auto &model = getModel();
    model.wasDestroyedBlockThisTurn_ = true;
    destroyEntity();
    model.addEntity(std::make_unique<Floor>(left, top, id, model));
}

void BasicHandler::destroyEntity() {
    getModel().eraseEntity(getEntity());
}

std::vector<const Entity *> MovableHandler::look(Direction direction) {
    auto res = lookMutable(direction);
    return {res.begin(), res.end()};
}

void ForegroundHandler::restoreBackground() {
    std::unordered_set<Entity *> restored;
    for (auto &row : background_) {
        for (auto *entity : row) {
            if (restored.count(entity) != 0) {
                continue;
            }
            restored.insert(entity);
            getModel().getMap().insert(*entity);
        }
        row.clear();
    }
}

void ForegroundHandler::setBackground() {
    if (background_.empty()) {
        background_.resize(
            getEntity().getHeight(),
            std::vector<Entity *>(getEntity().getWidth(), nullptr));
    }

    auto &entity = getEntity();

    int top = entity.getTop();
    int left = entity.getLeft();
    int height = entity.getHeight();
    int width = entity.getWidth();
    for (int row = entity.getTop(); row < entity.getTop() + height; row++) {
        background_[row - top].resize(width);
        for (int col = entity.getLeft(); col < left + width; col++) {
            background_[row - top][col - left] =
                &getModel().getByCoords(col, row);
        }
    }
    getModel().map_.insert(entity);
}

ForegroundHandler::ForegroundHandler(GameModel &model, ForegroundEntity &entity)
    : BasicHandler(model, entity) {
}

std::vector<std::vector<const Entity *>> ForegroundHandler::snapshotBackground()
    const {
    std::vector<std::vector<const Entity *>> vec;
    vec.reserve(getEntity().getHeight());
    for (const auto &line : background_) {
        vec.emplace_back(line.begin(), line.end());
    }
    return vec;
}

MovableHandler::MovableHandler(GameModel &model, MovableEntity &entity)
    : ForegroundHandler(model, entity) {
}

void MovableHandler::move(Direction direction, int speed) {
    auto &movable = dynamic_cast<MovableEntity &>(getEntity());

    movable.setDirection(direction);
    restoreBackground();

    int dist = speed;
    auto block = nearest(direction, [&movable](const Entity *entity) {
        return !movable.canPass(*entity);
    });

    if (!block.empty()) {
        dist = std::min(dist, getEntity().dist(*block[0]) - 1);
    }

    auto bullets = nearest(direction, [](const Entity *entity) {
        return dynamic_cast<const Projectile *>(entity) != nullptr;
    });

    if (!bullets.empty() && movable.dist(*bullets[0]) <= dist) {
        for (auto *entity : bullets) {
            getModel().getHandler(*entity).destroyByBullet();
        }
        ForegroundHandler::destroyByBullet();
        return;
    }

    switch (direction) {
        case Direction::UP:
            movable.setTop(getEntity().getTop() - dist);
            break;
        case Direction::LEFT:
            movable.setLeft(getEntity().getLeft() - dist);
            break;
        case Direction::DOWN:
            movable.setTop(getEntity().getTop() + dist);
            break;
        case Direction::RIGHT:
            movable.setLeft(getEntity().getLeft() + dist);
            break;
    }
    setBackground();
}

std::vector<Entity *> MovableHandler::lookMutable(Direction direction) {
    auto &movableEntity = dynamic_cast<MovableEntity &>(getEntity());
    // retuns square [left, right) x [down, top_)
    int left = -1;
    int top = -1;
    int right = -1;
    int down = -1;
    switch (direction) {
        case (Direction::UP):
            left = getEntity().getLeft();
            right = left + getEntity().getWidth();
            down = getEntity().getTop() - 1;
            top = std::max(-1, down - movableEntity.getSpeed());
            break;
        case (Direction::DOWN):
            left = getEntity().getLeft();
            right = left + getEntity().getWidth();
            top = getEntity().getTop() + getEntity().getHeight() - 1;
            down = std::min(getModel().getHeight() - 1,
                            top + movableEntity.getSpeed());
            break;
        case (Direction::LEFT):
            right = getEntity().getLeft();
            left = std::max(0, right - movableEntity.getSpeed());
            top = getEntity().getTop() - 1;
            down = top + getEntity().getHeight();
            break;
        case (Direction::RIGHT):
            left = getEntity().getLeft() + getEntity().getWidth();
            right = std::min(getModel().getWidth(),
                             left + movableEntity.getSpeed());
            top = getEntity().getTop() - 1;
            down = top + getEntity().getHeight();
            break;
    }

    std::unordered_set<Entity *> buff;
    buff.reserve(movableEntity.getSpeed());
    for (int row = down; row > top; row--) {
        for (int col = left; col < right; col++) {
            buff.insert(&getModel().getByCoords(col, row));
        }
    }

    return {buff.begin(), buff.end()};
}

void ForegroundHandler::setPosition(int left, int top) {
    restoreBackground();
    auto &entity = dynamic_cast<ForegroundEntity &>(getEntity());
    entity.setLeft(left);
    entity.setTop(top);
    setBackground();
}

void ForegroundHandler::destroyByBullet() {
    destroyEntity();
}

void ForegroundHandler::destroyEntity() {
    restoreBackground();
    BasicHandler::destroyEntity();
}

void MovableHandler::setDirection(Direction direction) {
    dynamic_cast<MovableEntity &>(getEntity()).setDirection(direction);
}

ProjectileHandler::ProjectileHandler(GameModel &model, MovableEntity &entity)
    : MovableHandler(model, entity) {
}

bool ProjectileHandler::breakIfBreakable() {
    auto &bullet = dynamic_cast<Projectile &>(getEntity());
    auto closest = nearest(bullet.getDirection(), [](const Entity *entity) {
        return entity->getStrength() > 0;
    });

    for (auto *entity : closest) {
        destroy(*entity);
    }
    if (!closest.empty()) {
        destroyByBullet();
        return true;
    }
    return false;
}

void ProjectileHandler::destroy(Entity &other) {
    if (other.getStrength() > getEntity().getStrength() ||
        other.getStrength() == 0) {
        return;
    }

    getModel().getHandler(other).destroyByBullet();
}

bool ProjectileHandler::isBreakOnCreation() {
    auto &bullet = dynamic_cast<Projectile &>(getEntity());
    assert(bullet.snapshotBackground().empty());  // true only on creation
    bool survive = true;

    for (int row = bullet.getTop(); row < bullet.getTop() + bullet.getHeight();
         row++) {
        for (int col = bullet.getLeft();
             col < bullet.getLeft() + bullet.getWidth(); col++) {
            auto &other = getModel().getByCoords(col, row);
            if (other.getStrength() != 0) {
                destroy(other);
                survive = false;
            }
        }
    }

    return !survive;
}

void ProjectileHandler::interactOnNextTick() {
    if (breakIfBreakable()) {
        return;
    }

    auto &bullet = dynamic_cast<Projectile &>(getEntity());
    move(bullet.getDirection(), bullet.getSpeed());
}

bool ProjectileHandler::canStandOn(const Entity &other) const {
    return other.isBulletPassable();
}

BonusHandler::BonusHandler(GameModel &model, Bonus &entity)
    : ForegroundHandler(model, entity) {
}

WalkOnWaterHandler::WalkOnWaterHandler(GameModel &model, WalkOnWater &entity)
    : BonusHandler(model, entity) {
}

void WalkOnWaterHandler::apply(Tank &tank) {
    tank.getAccessToHandler() = std::make_unique<TankMovableOnWaterHandler>(
        getModel(), tank, getModel().getTick());
}
}  // namespace Tanks::model
