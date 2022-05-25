#include "model/entity_holder.h"
#include <cassert>

namespace Tanks::model {
Entity &EntityHolder::insert(std::unique_ptr<Entity> entity) {
    buffer_.emplace_back(std::move(entity));
    return *buffer_.back();
}

void EntityHolder::erase(Entity &entity) {
    for (unsigned i = 0; i < buffer_.size(); i++) {
        if (&entity == buffer_[i].get()) {
            std::swap(buffer_[i], buffer_.back());
            buffer_.pop_back();
            return;
        }
    }
    assert(false && "Entity not found!");
}
}  // namespace Tanks::model
