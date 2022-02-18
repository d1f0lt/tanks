#pragma once

#include "entity.h"
#include "entity_holder.h"
#include "game_map.h"
#include "grouped_entities.h"

namespace tanks::model {
class IdMap {
public:
    void removeEntityById(int id);

    void addEntity(Entity *entity);

    [[nodiscard]] Entity *getEntityById(int id);

private:
    std::map<int, Entity *> idMap;
};

class GameModel {
public:
    [[nodiscard]] Entity *getEntityById(int id);

    [[nodiscard]] Entity *getEntityByCoords(const sf::Vector2<int> &coords);

    [[nodiscard]] DrawIterator getIterator() const;

private:
    void addEntity(std::unique_ptr<Entity> entity);

    void removeEntityById(int id);

    GameMap map;
    IdMap idMap;
    GroupedEntities groupedEntities;
    EntityHolder entityHolder;
};
}  // namespace tanks::model
