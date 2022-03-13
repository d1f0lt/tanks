#include "model/entity_holder.h"

namespace Tanks::model {
Entity &EntityHolder::insert(std::unique_ptr<Entity> entity) {
    buffer.emplace_back(std::move(entity));
    return *buffer.back();
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
