#include "model/game_model.h"
#include <cassert>
#include <fstream>
#include <iostream>
#include <mutex>
#include <optional>
#include <string>
#include <thread>
#include <unordered_map>
#include "model/blocks.h"
#include "model/projectile.h"

namespace Tanks::model {
GameModel::GameModel()
    : isFinished_(std::make_shared<std::atomic<bool>>(false)) {
}

Entity &GameModel::getByCoords(int col, int row) {
    return map_.getEntityByCoords(col, row);
}

void GameModel::addEntity(std::unique_ptr<Entity> entity) {
    if (auto *bullet = dynamic_cast<Projectile *>(entity.get())) {
        if (dynamic_cast<ProjectileHandler *>(handlers_[bullet])
                ->isBreakOnCreation()) {
            return;
        }
    }

    if (auto *tankHandler = dynamic_cast<TankHandler *>(&getHandler(*entity))) {
        tankHandler->applyBonusesInBackground();
    } else if (auto *in_foreground =
                   dynamic_cast<ForegroundEntity *>(entity.get())) {
        dynamic_cast<ForegroundHandler &>(*handlers_[in_foreground])
            .setBackground();
    }

    byId_.emplace(entity->getId(), entity.get());
    map_.insert(*entity);
    groupedEntities_.insert(*entity);
    entityHolder_.insert(std::move(entity));
}

void GameModel::eraseEntity(Entity &entity) {
    assert(map_.checkRemoved(entity));
    handlers_.erase(&entity);
    groupedEntities_.erase(entity);
    byId_.erase(entity.getId());
    entityHolder_.erase(entity);
}

void GameModel::loadLevel(int level) {
    loadLevel("../levels/level" + std::to_string(level) + ".csv");
}

void GameModel::loadLevel(const std::string &filename) {
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

    std::ifstream file(filename);

    assert(file.is_open() && "Unable to open map texture file");
    std::string str;

    for (int row = 0; row < MAP_HEIGHT; ++row) {
        std::getline(file, str);
        for (int col = 0, realCol = 0; col < MAP_WIDTH * 2 - 1;
             col += 2, realCol++) {  // skipping delimiter

            switch (CHAR_TO_TYPE.at(str[col])) {
                case (EntityType::BRICK):
                    addEntity(std::make_unique<Brick>(realCol * TILE_SIZE,
                                                      row * TILE_SIZE,
                                                      getIncrId(), *this));
                    break;
                case (EntityType::FLOOR):
                    addEntity(std::make_unique<Floor>(realCol * TILE_SIZE,
                                                      row * TILE_SIZE,
                                                      getIncrId(), *this));
                    break;
                case (EntityType::GRASS):
                    break;
                case (EntityType::STEEL):
                    addEntity(std::make_unique<Steel>(realCol * TILE_SIZE,
                                                      row * TILE_SIZE,
                                                      getIncrId(), *this));
                    break;
                case (EntityType::WATER):
                    addEntity(std::make_unique<Water>(realCol * TILE_SIZE,
                                                      row * TILE_SIZE,
                                                      getIncrId(), *this));
                    break;
                default:
                    addEntity(std::make_unique<LevelBorder>(
                        realCol * TILE_SIZE, row * TILE_SIZE,
                        CHAR_TO_TYPE.at(str[col]), getIncrId(), *this));
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

std::optional<std::reference_wrapper<Entity>> GameModel::getById(int entityId) {
    if (byId_.count(entityId) == 0) {
        return std::nullopt;
    }
    return *byId_[entityId];
}

std::vector<const Entity *> GameModel::getAll(EntityType type) const {
    auto vec = groupedEntities_.snapshotAll()[static_cast<unsigned>(type)];
    return {vec.begin(), vec.end()};
}

int GameModel::getTick() const {
    return currentTick_;
}

std::vector<std::vector<const Entity *>> GameModel::getAll() const {
    const auto &all = groupedEntities_.getAllByLink();
    std::vector<std::vector<const Entity *>> res;
    for (const auto &line : all) {
        res.emplace_back(  // NOLINT
            line.begin(), line.end());
    }
    return res;
}

void GameModel::nextTick() {
    if (isFinished()) {
        return;
    }
    wasShootThisTurn_ = false;
    wasDestroyedBlockThisTurn_ = false;
    //    std::this_thread::sleep_for(std::chrono::milliseconds(1));
    //    std::unique_lock lock(getMutex());
    executeAllEvents();
    moveBullets();
    currentTick_++;
}

BasicHandler &GameModel::getHandler(Entity &entity) {
    return *handlers_.at(&entity);
}

const std::vector<std::vector<Entity *>> &GameModel::getAllByLink() {
    return groupedEntities_.getAllByLink();
}

bool GameModel::executeEvent(Event &event) {
    return event.acceptExecutor(EventExecutor(*this));
}

bool GameModel::wasShootThisTurn() const {
    return wasShootThisTurn_;
}

void GameModel::moveBullets() {
    auto type = static_cast<unsigned>(EntityType::BULLET);
    const auto &all = getAllByLink()[type];
    for (unsigned i = 0; i < all.size(); i++) {  // bullet can be destroyed
        auto *bullet = dynamic_cast<Projectile *>(all[i]);
        assert(bullet != nullptr);
        assert(getById(bullet->getId()));

        dynamic_cast<ProjectileHandler &>(getHandler(*bullet))
            .interactOnNextTick();
    }
}

IncrId GameModel::getIncrId() {
    return currentId_++;
}

GameMap &GameModel::getMap() {
    return map_;
}

int GameModel::getRnd() {
    return std::abs(static_cast<int>(rnd()));
}

std::shared_mutex &GameModel::getMutex() const {
    return *sharedMutex_;
}

bool GameModel::wasDestroyedBlockThisTurn() const {
    return wasDestroyedBlockThisTurn_;
}
void GameModel::setFinished() {
    *isFinished_ = true;
}

bool GameModel::isFinished() const {
    return *isFinished_;
}

// TODO
// PlayerSkillsTmp GameModel::getPlayerSkills(int id) {
//    return playersSockets_[id];
//}

}  // namespace Tanks::model
