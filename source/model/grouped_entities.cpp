#include "model/grouped_entities.h"
#include <cassert>

namespace Tanks::model {
void GroupedEntities::insert(Entity &entity) {
    entities_[static_cast<unsigned>(entity.getType())].emplace_back(entity);
}

void GroupedEntities::erase(Entity &entity) {
    auto type = static_cast<unsigned>(entity.getType());
    for (auto iter = entities_[type].begin(); iter != entities_[type].end();
         iter++) {
        auto now = *iter;
        if (&now.get() == &entity) {
            entities_[type].erase(iter);
            return;
        }
    }
}

std::vector<std::vector<std::reference_wrapper<Entity>>>
GroupedEntities::snapshotAll() const {
    return entities_;
}

const std::vector<std::vector<std::reference_wrapper<Entity>>>
    &GroupedEntities::getAllByLink() {
    return entities_;
}

}  // namespace Tanks::model
