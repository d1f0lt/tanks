#include "model/grouped_entities.h"
#include <cassert>

namespace Tanks::model {
void GroupedEntities::insert(Entity &entity) {
    auto type = static_cast<unsigned>(entity.getType());
    entities_[type].emplace_back(&entity);
}

void GroupedEntities::erase(Entity &entity) {
    auto type = static_cast<unsigned>(entity.getType());
    for (auto iter = entities_[type].begin(); iter != entities_[type].end();
         iter++) {
        if (*iter == &entity) {
            entities_[type].erase(iter);
            break;
        }
    }
}

std::vector<std::vector<Entity *>> GroupedEntities::snapshotAll() const {
    return entities_;
}

const std::vector<std::vector<Entity *>> &GroupedEntities::getAllByLink()
    const {
    return entities_;
}

}  // namespace Tanks::model
