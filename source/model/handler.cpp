#include "model/handler.h"
#include <model/game_model.h>
#include <cassert>
#include <unordered_set>
#include "model/projectile.h"

namespace Tanks::model {
BasicHandler::BasicHandler(GameModel &model_, Entity &entity_)
    : model(model_), entity(entity_) {
    assert(model.handlers.count(&entity) == 0);
    model.handlers[&entity] = this;
}

void BasicHandler::move(Direction, int) {
    assert(false);
}

void BasicHandler::shoot() {
    assert(false);
}

std::vector<const Entity *> BasicHandler::look(Direction) {
    assert(false);
    return {};
}

std::vector<const Entity *> MovableHandler::look(Direction direction) {
    auto &real_entity = static_cast<MovableEntity &>(entity);
    // retuns square [left, right) x [down, top)
    int left = -1;
    int top = -1;
    int right = -1;
    int down = -1;
    switch (direction) {
        case (Direction::UP):
            left = entity.getLeft();
            right = left + entity.getWidth();
            down = entity.getTop() - 1;
            top = std::max(-1, down - real_entity.getSpeed());
            break;
        case (Direction::DOWN):
            left = entity.getLeft();
            right = left + entity.getWidth();
            top = entity.getTop() + entity.getHeight() - 1;
            down =
                std::min(model.getHeight() - 1, top + real_entity.getSpeed());
            break;
        case (Direction::LEFT):
            right = entity.getLeft();
            left = std::max(0, right - real_entity.getSpeed());
            top = entity.getTop() - 1;
            down = top + entity.getHeight();
            break;
        case (Direction::RIGHT):
            left = entity.getLeft() + entity.getWidth();
            right = std::min(model.getWidth(), left + real_entity.getSpeed());
            top = entity.getTop() - 1;
            down = top + entity.getHeight();
            break;
    }

    std::vector<const Entity *> res;
    for (int row = down; row > top; row--) {
        for (int col = left; col < right; col++) {
            res.push_back(&model.getByCoords(col, row));
        }
    }

    return res;
}

void ForegroundHandler::restoreBackground() {
    auto &real_entity = dynamic_cast<ForegroundEntity &>(entity);

    std::unordered_set<const Entity *> restored;

    for (int y = 0; y < entity.getHeight(); y++) {
        for (int x = 0; x < entity.getWidth(); x++) {
            if (restored.count(real_entity.background[y][x]) != 0) {
                real_entity.background[y][x] = nullptr;
                continue;
            }
            restored.insert(real_entity.background[y][x]);
            model.map.insert(
                const_cast<Entity &>(*real_entity.background[y][x]));
            real_entity.background[y][x] = nullptr;
        }
    }
}

void ForegroundHandler::setBackground() {
    auto &real_entity = dynamic_cast<ForegroundEntity &>(entity);
    real_entity.background.resize(
        entity.getHeight(),
        std::vector<const Entity *>(entity.getWidth(), nullptr));

    int y0 = entity.getTop();
    int x0 = entity.getLeft();
    for (int y = entity.getTop(); y < entity.getTop() + entity.getHeight();
         y++) {
        for (int x = entity.getLeft(); x < entity.getLeft() + entity.getWidth();
             x++) {
            real_entity.background[y - y0][x - x0] = &model.getByCoords(x, y);
        }
    }
    model.map.insert(entity);
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
    auto &real_entity = dynamic_cast<MovableEntity &>(entity);

    real_entity.setDirection(direction);
    real_entity.restoreBackground();

    int dist = speed;
    for (const auto *object : real_entity.look(direction)) {
        if (!real_entity.canPass(*object)) {
            dist = std::min(dist, real_entity.dist(*object));
        }
    }

    switch (direction) {
        case Direction::UP:
            real_entity.setTop(entity.getTop() - dist);
            break;
        case Direction::LEFT:
            real_entity.setLeft(entity.getLeft() - dist);
            break;
        case Direction::DOWN:
            real_entity.setTop(entity.getTop() + dist);
            break;
        case Direction::RIGHT:
            real_entity.setLeft(entity.getLeft() + dist);
            break;
    }
    real_entity.setBackground();
}

void TankHandler::shoot() {
    auto &real_entity = dynamic_cast<Tank &>(entity);

    static const std::unordered_map<Direction, int> DCOL = {
        {Tanks::model::Direction::UP, entity.getWidth() / 2},
        {Tanks::model::Direction::DOWN, entity.getWidth() / 2},
        {Tanks::model::Direction::RIGHT, entity.getWidth()},
        {Tanks::model::Direction::LEFT, -1}};

    static const std::unordered_map<Direction, int> DROW = {
        {Tanks::model::Direction::UP, -1},
        {Tanks::model::Direction::DOWN, entity.getHeight()},
        {Tanks::model::Direction::RIGHT, entity.getHeight() / 2},
        {Tanks::model::Direction::LEFT, entity.getHeight() / 2}};

    model.addEntity(std::make_unique<Projectile>(
        entity.getLeft() + DCOL.at(real_entity.getDirection()),
        entity.getTop() + DROW.at(real_entity.getDirection()),
        real_entity.getDirection(), model));
}

TankHandler::TankHandler(GameModel &model_, Tank &entity)
    : MovableHandler(model_, entity) {
}
}  // namespace Tanks::model
