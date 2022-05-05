#include "model/grouped_entities.h"
#include <cassert>

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
    auto i = static_cast<unsigned>(entity.getType());
    assert(std::find(entities[i].begin(), entities[i].end(), &entity) !=
           entities[i].end());

    entities[i].erase(
        std::find(entities[i].begin(), entities[i].end(), &entity));
}

const std::vector<std::vector<Entity *>> &GroupedEntities::snapshotAll() const {
    return entities;
}

GroupedEntities::GroupedEntities()
    : entities(25) {  // TODO create as many as need
}

}  // namespace Tanks::model
