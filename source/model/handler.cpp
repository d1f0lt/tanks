#include "model/handler.h"
#include <model/game_model.h>
#include <cassert>
#include "model/projectile.h"

namespace Tanks::model {
BasicHandler::BasicHandler(GameModel &model_, Entity &entity_)
    : model(model_), entity(entity_) {
    assert(model.handlers.count(&entity) == 0);
    model.handlers[&entity] = this;
}

void BasicHandler::move([[maybe_unused]] Direction direction) {
    assert(false);
}

void BasicHandler::shoot() {
    assert(false);
}

std::vector<const Entity *> BasicHandler::look(Direction direction) {
    if (direction == Direction::DOWN) {
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
    } else if (direction == Direction::UP) {
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
    } else if (direction == Direction::RIGHT) {
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
        assert(direction == Direction::LEFT);
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

void ForegroundHandler::restoreBackground() {
    auto &real_entity = static_cast<ForegroundEntity &>(entity);

    for (int y = 0; y < entity.getHeight(); y++) {
        for (int x = 0; x < entity.getWidth(); x++) {
            model.map.insert(
                const_cast<Entity &>(*real_entity.background[y][x]));
            real_entity.background[y][x] = nullptr;
        }
    }
}

void ForegroundHandler::setBackground() {
    auto &real_entity = static_cast<ForegroundEntity &>(entity);

    int y0 = entity.getTop();
    int x0 = entity.getLeft();
    for (int y = entity.getTop(); y < entity.getTop() + entity.getHeight();
         y++) {
        for (int x = entity.getLeft(); x < entity.getLeft() + entity.getWidth();
             x++) {
            real_entity.background[y - y0][x - x0] =
                &model.getEntityByCoords(x, y);
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

void MovableHandler::move(Direction direction) {
    // TODO lock model
    auto &real_entity = static_cast<MovableEntity &>(entity);

    real_entity.setDirection(direction);
    real_entity.restoreBackground();
    for (int i = 0; i < real_entity.getSpeed(); i++) {
        switch (direction) {
            case Direction::UP:
                real_entity.setTop(entity.getTop() - 1);
                break;
            case Direction::LEFT:
                real_entity.setLeft(entity.getLeft() - 1);
                break;
            case Direction::DOWN:
                real_entity.setTop(entity.getTop() + 1);
                break;
            case Direction::RIGHT:
                real_entity.setLeft(entity.getLeft() + 1);
                break;
        }
    }
    real_entity.setBackground();
}

void TankHandler::shoot() {
    auto &real_entity = static_cast<Tank &>(entity);

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
        entity.getLeft() + DCOL.at(real_entity.getDirection()),
        entity.getTop() + DROW.at(real_entity.getDirection()),
        real_entity.getDirection(), model));
}

TankHandler::TankHandler(GameModel &model_, Tank &entity)
    : MovableHandler(model_, entity) {
}
}  // namespace Tanks::model
