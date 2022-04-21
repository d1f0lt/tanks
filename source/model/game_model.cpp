#include "model/game_model.h"
#include <cassert>
#include <fstream>
#include "model/blocks.h"
#include "model/projectile.h"

namespace Tanks::model {
Entity &GameModel::getEntityByCoords(int col, int row) {
    return map.getEntityByCoords(col, row);
}

void GameModel::nextTick() {
    while (!que.empty()) {
        que.front()->execute(*this);
        que.pop();
    }

    for (auto &bot_tank :
         groupedEntities
             .getAll()[static_cast<unsigned>(EntityType::BOT_TANK)]) {
        ((MovableEntity &)bot_tank)
            .move(((MovableEntity &)bot_tank)
                      .getDirection());  // TODO normal casts
    }
}

Entity &GameModel::addEntity(std::unique_ptr<Entity> entity) {
    map.insert(*entity);
    groupedEntities.insert(*entity);
    return entityHolder.insert(std::move(entity));
}

void GameModel::removeEntity(Entity &entity) {
    map.erase(entity);
    groupedEntities.erase(entity);
}

PlayableTank &GameModel::spawnPlayableTank(int left, int top) {
    return static_cast<PlayableTank &>(addEntity(
        std::make_unique<PlayableTank>(left, top, Direction::UP, map, *this)));
}

/*
GameModel::GameModel(const std::string &filename) {
    std::ifstream is(filename);
}*/

void GameModel::loadLevel(int level) {
    const std::string currentLevel =
        "../levels/level" + std::to_string(level) + ".csv";

    static std::unordered_map<char, EntityType> charToEnum = {
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

    std::fstream file(currentLevel);
    assert(file.is_open() && "Unable to open map texture file");
    std::string str;

    map = GameMap();
    groupedEntities = GroupedEntities();
    entityHolder = EntityHolder();

    for (int row = 0; row < MAP_HEIGHT; ++row) {
        std::getline(file, str);
        for (int col = 0, realCol = 0; col < MAP_WIDTH * 2 - 1;
             col += 2, realCol++) {  // skipping delimiter

            auto real_entity = std::make_unique<Block>(
                realCol * TILE_SIZE, row * TILE_SIZE, charToEnum[str[col]]);

            addEntity(std::move(real_entity));
        }
    }
}

void GameModel::moveEntity(MovableEntity &entity, Direction direction) {
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

void GameModel::spawnBullet(int left, int top, Direction dir) {
    auto a = std::make_unique<Projectile>(left, top, dir, map);
}

}  // namespace Tanks::model
