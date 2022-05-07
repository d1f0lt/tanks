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
    entities_[static_cast<unsigned>(entity.getType())].emplace_back(&entity);
}

void GroupedEntities::erase(Entity &entity) {
    auto type = static_cast<unsigned>(entity.getType());
    assert(std::find(entities_[type].begin(), entities_[type].end(), &entity) !=
           entities_[type].end());

    entities_[type].erase(
        std::find(entities_[type].begin(), entities_[type].end(), &entity));
}

std::vector<std::vector<Entity *>> GroupedEntities::snapshotAll() const {
    return entities_;
}

const std::vector<std::vector<Entity *>> &GroupedEntities::getAllByLink()
    const {
    return entities_;
}

}  // namespace Tanks::model
