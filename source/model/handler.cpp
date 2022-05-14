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

std::vector<const Entity *> MovableHandler::look(Direction direction) {
    auto res = lookMutable(direction);
    return {res.begin(), res.end()};
}

void ForegroundHandler::restoreBackground() {
    std::unordered_set<Entity *> restored;
    for (int row = 0; row < entity_.getHeight(); row++) {
        for (int col = 0; col < entity_.getWidth(); col++) {
            if (restored.count(background_[row][col]) != 0) {
                background_[row][col] = nullptr;
                continue;
            }
            restored.insert(background_[row][col]);
            model_.map_.insert(*background_[row][col]);
            background_[row][col] = nullptr;
        }
    }
}

void ForegroundHandler::setBackground() {
    background_.resize(entity_.getHeight(),
                       std::vector<Entity *>(entity_.getWidth(), nullptr));

    int top = entity_.getTop();
    int left = entity_.getLeft();
    int height = entity_.getHeight();
    int width = entity_.getWidth();
    for (int row = entity_.getTop(); row < entity_.getTop() + height; row++) {
        for (int col = entity_.getLeft(); col < left + width; col++) {
            background_[row - top][col - left] = &model_.getByCoords(col, row);
        }
    }
    model_.map_.insert(entity_);
}

ForegroundHandler::ForegroundHandler(GameModel &model, ForegroundEntity &entity)
    : BasicHandler(model, entity) {
}

std::vector<std::vector<const Entity *>> ForegroundHandler::snapshotBackground()
    const {
    std::vector<std::vector<const Entity *>> vec;
    vec.reserve(entity_.getHeight());
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
    auto &movable = dynamic_cast<MovableEntity &>(entity_);

    movable.setDirection(direction);
    restoreBackground();

    int dist = speed;
    auto block = nearest(direction, [&movable](const Entity *entity) {
        return !movable.canPass(*entity);
    });
    if (!block.empty()) {
        dist = std::min(dist, entity_.dist(*block[0]) - 1);
    }

    auto bullets = nearest(direction, [](const Entity *entity) {
        return dynamic_cast<const Projectile *>(entity) != nullptr;
    });

    if (!bullets.empty() && movable.dist(*bullets[0]) <= dist) {
        for (auto *entity : bullets) {
            model_.removeEntity(*entity);
        }
        model_.removeEntity(entity_);
        return;
    }

    switch (direction) {
        case Direction::UP:
            movable.setTop(entity_.getTop() - dist);
            break;
        case Direction::LEFT:
            movable.setLeft(entity_.getLeft() - dist);
            break;
        case Direction::DOWN:
            movable.setTop(entity_.getTop() + dist);
            break;
        case Direction::RIGHT:
            movable.setLeft(entity_.getLeft() + dist);
            break;
    }
    setBackground();
}

std::vector<Entity *> MovableHandler::lookMutable(Direction direction) {
    auto &movableEntity = dynamic_cast<MovableEntity &>(entity_);
    // retuns square [left, right) x [down, top)
    int left = -1;
    int top = -1;
    int right = -1;
    int down = -1;
    switch (direction) {
        case (Direction::UP):
            left = entity_.getLeft();
            right = left + entity_.getWidth();
            down = entity_.getTop() - 1;
            top = std::max(-1, down - movableEntity.getSpeed());
            break;
        case (Direction::DOWN):
            left = entity_.getLeft();
            right = left + entity_.getWidth();
            top = entity_.getTop() + entity_.getHeight() - 1;
            down = std::min(model_.getHeight() - 1,
                            top + movableEntity.getSpeed());
            break;
        case (Direction::LEFT):
            right = entity_.getLeft();
            left = std::max(0, right - movableEntity.getSpeed());
            top = entity_.getTop() - 1;
            down = top + entity_.getHeight();
            break;
        case (Direction::RIGHT):
            left = entity_.getLeft() + entity_.getWidth();
            right =
                std::min(model_.getWidth(), left + movableEntity.getSpeed());
            top = entity_.getTop() - 1;
            down = top + entity_.getHeight();
            break;
    }

    std::unordered_set<Entity *> buff;
    buff.reserve(movableEntity.getSpeed());
    for (int row = down; row > top; row--) {
        for (int col = left; col < right; col++) {
            buff.insert(&model_.getByCoords(col, row));
        }
    }

    return {buff.begin(), buff.end()};
}

void TankHandler::shoot() {
    // TODO lock model
    if (model_.getTick() <= lastShootTick + RELOAD_TICKS) {
        return;
    }

    lastShootTick = model_.getTick();

    auto &tank = dynamic_cast<Tank &>(entity_);

    static const std::unordered_map<Direction, std::pair<int, int>> error{
        {Tanks::model::Direction::UP, {-5, -9}},
        {Tanks::model::Direction::DOWN, {-5, 3}},
        {Tanks::model::Direction::RIGHT, {4, -12}},
        {Tanks::model::Direction::LEFT, {-11, -12}}};

    static const std::unordered_map<Direction, int> DCOL = {
        {Tanks::model::Direction::UP, entity_.getWidth() / 2},
        {Tanks::model::Direction::DOWN, entity_.getWidth() / 2},
        {Tanks::model::Direction::RIGHT, entity_.getWidth()},
        {Tanks::model::Direction::LEFT, -1}};

    static const std::unordered_map<Direction, int> DROW = {
        {Tanks::model::Direction::UP, -1},
        {Tanks::model::Direction::DOWN, entity_.getHeight()},
        {Tanks::model::Direction::RIGHT, entity_.getHeight() / 2},
        {Tanks::model::Direction::LEFT, entity_.getHeight() / 2}};

    auto dir = tank.getDirection();
    model_.addEntity(std::make_unique<Projectile>(
        entity_.getLeft() + DCOL.at(dir) + error.at(dir).first,
        entity_.getTop() + DROW.at(dir) + error.at(dir).second, dir, model_,
        model_.getCurrentId()));
}

TankHandler::TankHandler(GameModel &model, Tank &entity)
    : MovableHandler(model, entity) {
}

void TankHandler::move(Direction dir, int speed) {
    // TODO lock model
    if (model_.getTick() <= std::max(lastShootTick, lastMoveTick)) {
        return;
    }

    lastMoveTick = model_.getTick();
    MovableHandler::move(dir, speed);
}

ProjectileHandler::ProjectileHandler(GameModel &model, MovableEntity &entity)
    : MovableHandler(model, entity) {
}

bool ProjectileHandler::isBreakOnNextTick() {
    auto &bullet = dynamic_cast<Projectile &>(entity_);
    auto closest = nearest(bullet.getDirection(), [](const Entity *entity) {
        return entity->getStrength() > 0;
    });

    for (auto *entity : closest) {
        destroyByBullet(*entity);
    }
    if (!closest.empty()) {
        model_.removeEntity(bullet);
        return true;
    }
    return false;
}

void ProjectileHandler::destroyByBullet(Entity &other) {
    if (other.getStrength() > entity_.getStrength() ||
        other.getStrength() == 0) {
        return;
    }

    if (dynamic_cast<Block *>(&other) == nullptr) {
        model_.removeEntity(other);
        return;
    }
    int left = other.getLeft();
    int top = other.getTop();
    model_.removeEntity(other);
    model_.addEntity(std::make_unique<Floor>(left, top, model_.getCurrentId()));
}

bool ProjectileHandler::isBreakOnCreation() {
    auto &bullet = dynamic_cast<Projectile &>(entity_);
    assert(bullet.snapshotBackground().empty());  // true only on creation
    bool survive = true;

    for (int row = bullet.getTop(); row < bullet.getTop() + bullet.getHeight();
         row++) {
        for (int col = bullet.getLeft();
             col < bullet.getLeft() + bullet.getWidth(); col++) {
            auto &entity = model_.getByCoords(col, row);
            if (entity.getStrength() != 0) {  // TODO .doInteract
                dynamic_cast<ProjectileHandler *>(model_.handlers_[&bullet])
                    ->destroyByBullet(entity);
                survive = false;
            }
        }
    }

    return !survive;
}
}  // namespace Tanks::model
