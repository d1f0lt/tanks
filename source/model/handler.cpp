#include "model/handler.h"
#include <model/game_model.h>
#include <cassert>
//#include "model/tank.h"
#include "model/projectile.h"

namespace Tanks::model {
BasicHandler::BasicHandler(GameModel &model_) : model(model_) {
}

void BasicHandler::move(MovableEntity &entity,
                        Tanks::model::Direction direction) {
    assert(false);
}

void BasicHandler::shoot(Tank &entity) {
    assert(false);
}
std::vector<const Entity *> BasicHandler::look(const Entity &entity,
                                               Tanks::model::Direction dir) {
    if (dir == Direction::DOWN) {
        if (entity.getTop() + entity.getHeight() == model.map.getHeight() - 1) {
            return {};
        }

        std::vector<const Entity *> res(entity.getWidth());
        for (int col = entity.getLeft();
             col < entity.getLeft() + entity.getWidth(); col++) {
            res[col - entity.getLeft()] = &model.map.getEntityByCoords(
                col, entity.getTop() + entity.getHeight());
        }
        return res;
    } else if (dir == Direction::UP) {
        if (entity.getTop() == 0) {
            return {};
        }

        std::vector<const Entity *> res(entity.getWidth());
        for (int col = entity.getLeft();
             col < entity.getLeft() + entity.getWidth(); col++) {
            res[col - entity.getLeft()] =
                &model.map.getEntityByCoords(col, entity.getTop() - 1);
        }
        return res;
    } else if (dir == Direction::RIGHT) {
        if (entity.getLeft() + entity.getWidth() == model.map.getWidth() - 1) {
            return {};
        }
        std::vector<const Entity *> res(entity.getHeight());
        for (int row = entity.getTop();
             row < entity.getTop() + entity.getHeight(); row++) {
            res[row - entity.getTop()] = &model.map.getEntityByCoords(
                entity.getLeft() + entity.getWidth(), row);
        }
        return res;
    } else {
        assert(dir == Direction::LEFT);
        std::vector<const Entity *> res(entity.getHeight());
        if (entity.getLeft() == 0) {
            return {};
        }
        for (int row = entity.getTop();
             row < entity.getTop() + entity.getHeight(); row++) {
            res[row - entity.getTop()] =
                &model.map.getEntityByCoords(entity.getLeft() - 1, row);
        }
        return res;
    }
}

void ForegroundHandler::restoreBackground(ForegroundEntity &entity) {
    for (int y = 0; y < entity.getHeight(); y++) {
        for (int x = 0; x < entity.getWidth(); x++) {
            model.map.insert(const_cast<Entity &>(*entity.background[y][x]));
            entity.background[y][x] = nullptr;
        }
    }
}

void ForegroundHandler::setBackground(ForegroundEntity &entity) {
    int y0 = entity.getTop();
    int x0 = entity.getLeft();
    for (int y = entity.getTop(); y < entity.getTop() + entity.getHeight();
         y++) {
        for (int x = entity.getLeft(); x < entity.getLeft() + entity.getWidth();
             x++) {
            entity.background[y - y0][x - x0] = &model.getEntityByCoords(x, y);
        }
    }
    model.map.insert(entity);
}

ForegroundHandler::ForegroundHandler(GameModel &model_) : BasicHandler(model_) {
}

MovableHandler::MovableHandler(GameModel &model_) : ForegroundHandler(model_) {
}

void MovableHandler::move(MovableEntity &entity,
                          Tanks::model::Direction direction) {
    // TODO lock model
    entity.setDirection(direction);
    entity.restoreBackground();
    switch (direction) {
        case Direction::UP:
            entity.setTop(entity.getTop() - 1);
            break;
        case Direction::LEFT:
            entity.setLeft(entity.getLeft() - 1);
            break;
        case Direction::DOWN:
            entity.setTop(entity.getTop() + 1);
            break;
        case Direction::RIGHT:
            entity.setLeft(entity.getLeft() + 1);
            break;
    }
    entity.setBackground();
}

TankHandler::TankHandler(GameModel &model_) : MovableHandler(model_) {
}

void TankHandler::shoot(Tank &entity) {
    const std::unordered_map<Direction, int> DCOL = {
        {Tanks::model::Direction::UP, entity.getWidth() / 2},
        {Tanks::model::Direction::DOWN, entity.getWidth() / 2},
        {Tanks::model::Direction::RIGHT, entity.getWidth()},
        {Tanks::model::Direction::LEFT, -1}};

    const std::unordered_map<Direction, int> DROW = {
        {Tanks::model::Direction::UP, -1},
        {Tanks::model::Direction::DOWN, entity.getHeight()},
        {Tanks::model::Direction::RIGHT, entity.getHeight() / 2},
        {Tanks::model::Direction::LEFT, entity.getHeight() / 2}};

    model.addEntity(std::make_unique<Projectile>(
        entity.getLeft() + DCOL.at(entity.getDirection()),
        entity.getTop() + DROW.at(entity.getDirection()), entity.getDirection(),
        std::make_unique<MovableHandler>(model)));
}
}  // namespace Tanks::model
