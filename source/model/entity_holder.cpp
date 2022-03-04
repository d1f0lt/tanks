#include "entity_holder.h"

namespace Tanks::model {
Entity &EntityHolder::insert(std::unique_ptr<Entity> entity) {
    Entity &res = *entity;
    buffer.emplace_back(std::move(entity));
    return res;
}

void EntityHolder::remove(Entity &entity) {
    for (unsigned i = 0; i < buffer.size(); i++) {
        if (&entity == buffer[i].get()) {
            std::swap(buffer[i], buffer.back());
            buffer.pop_back();
            break;
        }
    }
}
}  // namespace Tanks::model
