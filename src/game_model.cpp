#include "game_model.h"

namespace tanks::model {
void IdMap::removeEntityById(int id) {
    idMap.erase(id);
}

void IdMap::addEntity(Entity *entity) {
    idMap.emplace(entity->getId(), entity);
}

Entity *IdMap::getEntityById(int id) {
    if (idMap.count(id) == 0) {
        return nullptr;
    }
    return idMap[id];
}

Entity *GameModel::getEntityById(int id) {
    return idMap.getEntityById(id);
}

Entity *GameModel::getEntityByCoords(const sf::Vector2<int> &coords) {
    return map.getEntityByCoords(coords);
}

DrawIterator GameModel::getIterator() const {
    return groupedEntities.getIterator();
}

void GameModel::addEntity(std::unique_ptr<Entity> entity) {
    // TODO : store pointer somewhere
    map.addEntity(entity.get());
    idMap.addEntity(entity.get());
    groupedEntities.addEntity(entity.get());
}

void GameModel::removeEntityById(int id) {
    map.removeEntity(idMap.getEntityById(id));
    idMap.removeEntityById(id);
}

}  // namespace tanks::model
