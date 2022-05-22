#include "model/handler.h"
#include <model/game_model.h>
#include <cassert>
#include <unordered_set>
#include "model/blocks.h"
#include "model/projectile.h"

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

std::vector<const Entity *> MovableHandler::look(Direction direction) {
    auto res = lookMutable(direction);
    return {res.begin(), res.end()};
}

void ForegroundHandler::restoreBackground() {
    std::unordered_set<Entity *> restored;
    for (int row = 0; row < getEntity().getHeight(); row++) {
        for (int col = 0; col < getEntity().getWidth(); col++) {
            if (restored.count(background_[row][col]) != 0) {
                background_[row][col] = nullptr;
                continue;
            }
            restored.insert(background_[row][col]);
            getModel().map_.insert(*background_[row][col]);
            background_[row][col] = nullptr;
        }
    }
}

void ForegroundHandler::setBackground() {
    background_.resize(getEntity().getHeight(),
                       std::vector<Entity *>(getEntity().getWidth(), nullptr));

    auto &entity = getEntity();

    int top = entity.getTop();
    int left = entity.getLeft();
    int height = entity.getHeight();
    int width = entity.getWidth();
    for (int row = entity.getTop(); row < entity.getTop() + height; row++) {
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
    // TODO lock model
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
            getModel().removeEntity(*entity);
        }
        getModel().removeEntity(getEntity());
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
    // retuns square [left, right) x [down, top)
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

void TankHandler::shoot() {
    // TODO lock model
    if (getModel().getTick() <= lastShootTick_ + RELOAD_TICKS) {
        return;
    }

    lastShootTick_ = getModel().getTick();

    auto &tank = dynamic_cast<Tank &>(getEntity());

    static const std::unordered_map<Direction, int> DCOL = {
        {Tanks::model::Direction::UP, getEntity().getWidth() / 2},
        {Tanks::model::Direction::DOWN, getEntity().getWidth() / 2},
        {Tanks::model::Direction::RIGHT, getEntity().getWidth()},
        {Tanks::model::Direction::LEFT, -1}};

    static const std::unordered_map<Direction, int> DROW = {
        {Tanks::model::Direction::UP, -1},
        {Tanks::model::Direction::DOWN, getEntity().getHeight()},
        {Tanks::model::Direction::RIGHT, getEntity().getHeight() / 2},
        {Tanks::model::Direction::LEFT, getEntity().getHeight() / 2}};

    getModel().addEntity(std::make_unique<Projectile>(
        getEntity().getLeft() + DCOL.at(tank.getDirection()),
        getEntity().getTop() + DROW.at(tank.getDirection()),
        tank.getDirection(), getModel(), getModel().getIncrId()));
}

TankHandler::TankHandler(GameModel &model, Tank &entity)
    : MovableHandler(model, entity) {
}

void TankHandler::move(Direction dir, int speed) {
    if (getModel().getTick() <= std::max(lastShootTick_, lastMoveTick_)) {
        return;
    }

    lastMoveTick_ = getModel().getTick();
    MovableHandler::move(dir, speed);
}

void TankHandler::move(Direction direction) {
    move(direction, dynamic_cast<Tank &>(getEntity()).getSpeed());
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
        destroyByBullet(*entity);
    }
    if (!closest.empty()) {
        getModel().removeEntity(bullet);
        return true;
    }
    return false;
}

void ProjectileHandler::destroyByBullet(Entity &other) {
    if (other.getStrength() > getEntity().getStrength() ||
        other.getStrength() == 0) {
        return;
    }

    if (dynamic_cast<Block *>(&other) == nullptr) {
        getModel().removeEntity(other);
        return;
    }
    int left = other.getLeft();
    int top = other.getTop();
    getModel().removeEntity(other);
    getModel().addEntity(
        std::make_unique<Floor>(left, top, getModel().getIncrId()));
}

bool ProjectileHandler::isBreakOnCreation() {
    auto &bullet = dynamic_cast<Projectile &>(getEntity());
    assert(bullet.snapshotBackground().empty());  // true only on creation
    bool survive = true;

    for (int row = bullet.getTop(); row < bullet.getTop() + bullet.getHeight();
         row++) {
        for (int col = bullet.getLeft();
             col < bullet.getLeft() + bullet.getWidth(); col++) {
            auto &entity = getModel().getByCoords(col, row);
            if (entity.getStrength() != 0) {  // TODO .doInteract
                dynamic_cast<ProjectileHandler &>(getModel().getHandler(bullet))
                    .destroyByBullet(entity);
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

}  // namespace Tanks::model
