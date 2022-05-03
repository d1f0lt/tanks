#include "model/foreground_entity.h"
#include <cassert>

namespace Tanks::model {
ForegroundEntity::ForegroundEntity(int left,
                                   int top,
                                   std::unique_ptr<BasicHandler> handler_)
    : Entity(left, top), handler(std::move(handler_)) {
}

std::vector<const Entity *> ForegroundEntity::look(Direction direction) const {
    return handler->look(direction);
}

void ForegroundEntity::restoreBackground() {
    handler->restoreBackground();
}

void ForegroundEntity::setBackground() {
    handler->setBackground();
}

std::vector<std::vector<const Entity *>> ForegroundEntity::snapshotBackground()
    const {
    return background;
}
}  // namespace Tanks::model
