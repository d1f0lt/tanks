#include "model/grouped_entities.h"
#include <cassert>

namespace Tanks::model {
void GroupedEntities::insert(Entity &entity) {
    auto type = static_cast<unsigned>(entity.getType());
    entities_[type].emplace_back(&entity);
    idToIterator_.emplace(entity.getId(),
                          std::tuple{type, entities_[type].end() - 1});
}

void GroupedEntities::erase(Entity &entity) {
    int entityId = entity.getId();
    auto [i, iter] = idToIterator_[entityId];
    entities_[i].erase(iter);
    idToIterator_.erase(i);
}

std::vector<std::vector<Entity *>> GroupedEntities::snapshotAll() const {
    return entities_;
}

const std::vector<std::vector<Entity *>> &GroupedEntities::getAllByLink()
    const {
    return entities_;
}

}  // namespace Tanks::model
