#include "grouped_entities.h"

namespace tanks::model {
// TODO: make here order
void GroupedEntities::addEntity(Entity *entity) {
    entities[0].push_back(entity);
}

void GroupedEntities::removeEntity(Entity *entity) {
    entities[0].erase(
        std::find(entities[0].begin(), entities[0].end(), entity));
}

DrawIterator GroupedEntities::getIterator() const {
    // TODO: make it work
    return DrawIterator();
}
}  // namespace tanks::model
