#include "model/game_model.h"
#include <cassert>
#include <fstream>
#include <iostream>
#include <thread>
#include <unordered_map>
#include "model/blocks.h"
#include "model/projectile.h"

namespace Tanks::model {
Entity &GameModel::getEntityByCoords(int col, int row) {
    return map.getEntityByCoords(col, row);
}

void GameModel::nextTick() {
    for (auto *entity :
         groupedEntities
             .snapshotAll()[static_cast<unsigned>(EntityType::BOT_TANK)]) {
        auto *tank = dynamic_cast<BotTank *>(entity);
        assert(tank != nullptr);
        handlers[tank]->move(tank->getDirection());
    }

    for (auto *entity :
         groupedEntities
             .snapshotAll()[static_cast<unsigned>(EntityType::BULLET)]) {
        auto *bullet = dynamic_cast<Projectile *>(entity);
        assert(bullet != nullptr);
        handlers[bullet]->move(bullet->getDirection());

        for (auto &row : bullet->snapshotBackground()) {
            for (auto *ent : row) {
                if (ent->isDestroyable()) {
                    removeEntity(*const_cast<Entity *>(ent));
                }
            }
        }
    }
}

Entity &GameModel::addEntity(std::unique_ptr<Entity> entity) {
    map.insert(*entity);
    groupedEntities.insert(*entity);
    return entityHolder.insert(std::move(entity));
}

void GameModel::removeEntity(Entity &entity) {
    if (auto *foreground = dynamic_cast<ForegroundEntity *>(&entity)) {
        handlers[foreground]->restoreBackground();
    }

    handlers.erase(&entity);
    map.erase(entity);
    groupedEntities.erase(entity);
}

PlayableTank &GameModel::spawnPlayableTank(const int left, const int top) {
    assert(left + TANK_SIZE < map.getWidth());
    assert(top + TANK_SIZE < map.getHeight());

    for (int row = top; row < top + TANK_SIZE; row++) {
        for (int col = left; col < left + TANK_SIZE; col++) {
            assert(getEntityByCoords(col, row).isTankPassable());
        }
    }

    return static_cast<PlayableTank &>(addEntity(
        std::make_unique<PlayableTank>(left, top, Direction::UP, *this)));
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

    map = GameMap();
    groupedEntities = GroupedEntities();
    entityHolder = EntityHolder();

    for (int row = 0; row < MAP_HEIGHT; ++row) {
        std::getline(file, str);
        for (int col = 0, realCol = 0; col < MAP_WIDTH * 2 - 1;
             col += 2, realCol++) {  // skipping delimiter

            addEntity(std::make_unique<Block>(realCol * TILE_SIZE,
                                              row * TILE_SIZE,
                                              CHAR_TO_TYPE.at(str[col])));
        }
    }
}

int GameModel::getWidth() const {
    return map.getWidth();
}

int GameModel::getHeight() const {
    return map.getHeight();
}

}  // namespace Tanks::model
