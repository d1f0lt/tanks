//
// Created by ilma4 on 2/18/22.
//

#include "entity_holder.h"

namespace tanks::model {
void EntityHolder::addEntity(std::unique_ptr<Entity> entity) {
    entityHolder[entity->getId()] = std::move(entity);
}

void EntityHolder::removeEntity(int id) {
    entityHolder.erase(id);
}
}  // namespace tanks::model