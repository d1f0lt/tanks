#include "model/grouped_entities.h"

namespace Tanks::model {
void GroupedEntities::insert(Entity &entity) {
    // 0 - usual blocks
    // 1 - bot-Tanks
    // 2 - playable Tanks
    // 3 - bullets
    // 4 - grass
    // TODO make it independent from order in EntityType
    entities[static_cast<unsigned>(entity.getType())].emplace_back(&entity);
}

void GroupedEntities::erase(Entity &entity) {
    entities[0].erase(
        std::find(entities[0].begin(), entities[0].end(), &entity));
}

const std::vector<std::vector<const Entity *>> &GroupedEntities::getAll()
    const {
    return entities;
}

GroupedEntities::GroupedEntities()
    : entities(25) {  // TODO create as many as need
}

}  // namespace Tanks::model
