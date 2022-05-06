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
    auto &movableEntity = static_cast<MovableEntity &>(entity_);
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

    std::unordered_set<const Entity *> buff;
    buff.reserve(movableEntity.getSpeed());
    for (int row = down; row > top; row--) {
        for (int col = left; col < right; col++) {
            buff.insert(&model_.getByCoords(col, row));
        }
    }

    return std::vector(buff.begin(), buff.end());
}

void ForegroundHandler::restoreBackground() {
    auto &real_entity = dynamic_cast<ForegroundEntity &>(entity_);

    std::unordered_set<const Entity *> restored;

    for (int y = 0; y < entity_.getHeight(); y++) {
        for (int x = 0; x < entity_.getWidth(); x++) {
            if (restored.count(real_entity.background_[y][x]) != 0) {
                real_entity.background_[y][x] = nullptr;
                continue;
            }
            restored.insert(real_entity.background_[y][x]);
            model_.map_.insert(
                const_cast<Entity &>(*real_entity.background_[y][x]));
            real_entity.background_[y][x] = nullptr;
        }
    }
}

void ForegroundHandler::setBackground() {
    auto &real_entity = dynamic_cast<ForegroundEntity &>(entity_);
    real_entity.background_.resize(
        entity_.getHeight(),
        std::vector<const Entity *>(entity_.getWidth(), nullptr));

    int y0 = entity_.getTop();
    int x0 = entity_.getLeft();
    for (int y = entity_.getTop(); y < entity_.getTop() + entity_.getHeight();
         y++) {
        for (int x = entity_.getLeft();
             x < entity_.getLeft() + entity_.getWidth(); x++) {
            real_entity.background_[y - y0][x - x0] = &model_.getByCoords(x, y);
        }
    }
    model_.map_.insert(entity_);
}

ForegroundHandler::ForegroundHandler(GameModel &model_,
                                     ForegroundEntity &entity)
    : BasicHandler(model_, entity) {
}

MovableHandler::MovableHandler(GameModel &model_, MovableEntity &entity)
    : ForegroundHandler(model_, entity) {
}

void MovableHandler::move(Direction direction, int speed) {
    // TODO lock model
    auto &movable = dynamic_cast<MovableEntity &>(entity_);

    movable.setDirection(direction);
    restoreBackground();

    auto bullets = nearest(direction, [](const Entity *entity) {
        return dynamic_cast<const Projectile *>(entity) != nullptr;
    });

    if (!bullets.empty()) {
        for (const auto *entity : bullets) {
            model_.removeEntity(const_cast<Entity &>(*entity));
        }
        model_.removeEntity(entity_);
        return;
    }

    int dist = speed;
    auto block = nearest(direction, [&movable](const Entity *entity) {
        return !movable.canPass(*entity);
    });
    if (!block.empty()) {
        dist = std::min(dist, entity_.dist(*block[0]) - 1);
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

void TankHandler::shoot() {
    auto &tank = dynamic_cast<Tank &>(entity_);

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

    model_.addEntity(std::make_unique<Projectile>(
        entity_.getLeft() + DCOL.at(tank.getDirection()),
        entity_.getTop() + DROW.at(tank.getDirection()), tank.getDirection(),
        model_));
}

TankHandler::TankHandler(GameModel &model_, Tank &entity)
    : MovableHandler(model_, entity) {
}

ProjectileHandler::ProjectileHandler(GameModel &model, MovableEntity &entity)
    : MovableHandler(model, entity) {
}

bool ProjectileHandler::isBreakOnNextTick() {
    auto &bullet = dynamic_cast<Projectile &>(entity_);
    auto closest = nearest(bullet.getDirection(), [](const Entity *entity) {
        return entity->getStrength() > 0;
    });

    for (const auto *entity : closest) {
        destroyByBullet(const_cast<Entity &>(*entity));
    }
    if (!closest.empty()) {
        model_.removeEntity(bullet);
        return true;
    }
    return false;
}

void ProjectileHandler::destroyByBullet(Entity &other) {
    if (other.getStrength() > entity_.getStrength()) {
        return;
    }

    if (dynamic_cast<Block *>(&other) == nullptr) {
        model_.removeEntity(other);
        return;
    }
    int left = other.getLeft();
    int top = other.getTop();
    model_.removeEntity(other);
    model_.addEntity(std::make_unique<Floor>(left, top));
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
