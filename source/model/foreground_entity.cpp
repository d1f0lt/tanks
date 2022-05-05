#include "model/foreground_entity.h"
#include <cassert>

namespace Tanks::model {
ForegroundEntity::ForegroundEntity(int left,
                                   int top,
                                   std::unique_ptr<BasicHandler> handler_)
    : Entity(left, top), handler(std::move(handler_)) {
}

ForegroundEntity::ForegroundEntity(int left,
                                   int top,
                                   int id,
                                   std::unique_ptr<BasicHandler> handler)
    : Entity(left, top, id), handler(std::move(handler)) {
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
