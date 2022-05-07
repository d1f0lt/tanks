#include "model/game_model.h"
#include <cassert>
#include <fstream>
#include <iostream>
#include <thread>
#include <unordered_map>
#include "model/blocks.h"
#include "model/projectile.h"

namespace Tanks::model {
Entity &GameModel::getByCoords(int col, int row) {
    return map_.getEntityByCoords(col, row);
}

void GameModel::nextTick() {
    for (auto *entity :
         groupedEntities_
             .getAllByLink()[static_cast<unsigned>(EntityType::BOT_TANK)]) {
        auto *tank = dynamic_cast<BotTank *>(entity);
        assert(tank != nullptr);
        dynamic_cast<MovableHandler &>(*handlers_[tank])
            .move(tank->getDirection(), tank->getSpeed());
    }

    for (auto *entity :
         groupedEntities_
             .getAllByLink()[static_cast<unsigned>(EntityType::BULLET)]) {
        auto *bullet = dynamic_cast<Projectile *>(entity);
        assert(bullet != nullptr);

        if (dynamic_cast<ProjectileHandler &>(*handlers_[bullet])
                .isBreakOnNextTick()) {
            continue;
        }
        dynamic_cast<MovableHandler &>(*handlers_[bullet])
            .move(bullet->getDirection(), bullet->getSpeed());
    }
    currentTick++;
}

void GameModel::addEntity(std::unique_ptr<Entity> entity) {
    if (auto *bullet = dynamic_cast<Projectile *>(entity.get())) {
        if (dynamic_cast<ProjectileHandler *>(handlers_[bullet])
                ->isBreakOnCreation()) {
            handlers_.erase(bullet);
            return;
        }
    }

    if (auto *in_foreground = dynamic_cast<ForegroundEntity *>(entity.get())) {
        dynamic_cast<ForegroundHandler &>(*handlers_[in_foreground])
            .setBackground();
    }

    byid_.emplace(entity->getId(), entity.get());
    map_.insert(*entity);
    groupedEntities_.insert(*entity);
    entityHolder_.insert(std::move(entity));
}

void GameModel::removeEntity(Entity &entity) {
    if (auto *foreground = dynamic_cast<ForegroundEntity *>(&entity)) {
        dynamic_cast<ForegroundHandler &>(*handlers_[foreground])
            .restoreBackground();
    }

    byid_.erase(entity.getId());
    handlers_.erase(&entity);
    groupedEntities_.erase(entity);
    entityHolder_.remove(entity);
}

PlayableTank &GameModel::spawnPlayableTank(int left, int top) {
    assert(left + TANK_SIZE < map_.getWidth());
    assert(top + TANK_SIZE < map_.getHeight());

    for (int row = top; row < top + TANK_SIZE; row++) {
        for (int col = left; col < left + TANK_SIZE; col++) {
            assert(getByCoords(col, row).isTankPassable());
        }
    }

    addEntity(std::make_unique<PlayableTank>(left, top, Direction::UP, *this));
    return dynamic_cast<PlayableTank &>(getByCoords(left, top));
}

void GameModel::loadLevel(int level) {
    const std::string currentLevel =
        "../levels/level" + std::to_string(level) + ".csv";

    const static std::unordered_map<char, EntityType> CHAR_TO_TYPE = {
        {'=', EntityType::HORIZONTAL_BORDER},
        {'|', EntityType::VERTICAL_BORDER},
        {'1', EntityType::BRICK},
        {'0', EntityType::STEEL},
        {' ', EntityType::FLOOR},
        {'~', EntityType::WATER},
        {'[', EntityType::LEFT_UP_CORNER},
        {']', EntityType::RIGHT_UP_CORNER},
        {'{', EntityType::LEFT_DOWN_CORNER},
        {'}', EntityType::RIGHT_DOWN_CORNER}};

    std::ifstream file(currentLevel);

    assert(file.is_open() && "Unable to open map texture file");
    std::string str;

    for (int row = 0; row < MAP_HEIGHT; ++row) {
        std::getline(file, str);
        for (int col = 0, realCol = 0; col < MAP_WIDTH * 2 - 1;
             col += 2, realCol++) {  // skipping delimiter

            switch (CHAR_TO_TYPE.at(str[col])) {
                case (EntityType::BRICK):
                    addEntity(std::make_unique<Brick>(realCol * TILE_SIZE,
                                                      row * TILE_SIZE));
                    break;
                case (EntityType::FLOOR):
                    addEntity(std::make_unique<Floor>(realCol * TILE_SIZE,
                                                      row * TILE_SIZE));
                    break;
                case (EntityType::GRASS):
                    break;
                case (EntityType::STEEL):
                    addEntity(std::make_unique<Steel>(realCol * TILE_SIZE,
                                                      row * TILE_SIZE));
                    break;
                case (EntityType::WATER):
                    addEntity(std::make_unique<Water>(realCol * TILE_SIZE,
                                                      row * TILE_SIZE));
                    break;
                default:
                    addEntity(std::make_unique<LevelBorder>(
                        realCol * TILE_SIZE, row * TILE_SIZE,
                        CHAR_TO_TYPE.at(str[col])));
                    break;
            }
        }
    }
}

int GameModel::getWidth() const {
    return map_.getWidth();
}

int GameModel::getHeight() const {
    return map_.getHeight();
}

Entity &GameModel::getById(int entityId) {
    assert(byid_.count(entityId) != 0);
    return *byid_[entityId];
}

std::vector<const Entity *> GameModel::getAll(EntityType type) {
    auto vec = groupedEntities_.snapshotAll()[static_cast<unsigned>(type)];
    return {vec.begin(), vec.end()};
}
int GameModel::getTick() const {
    return currentTick;
}

}  // namespace Tanks::model
