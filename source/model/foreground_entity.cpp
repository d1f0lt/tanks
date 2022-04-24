#include "model/foreground_entity.h"

namespace Tanks::model {
ForegroundEntity::ForegroundEntity(int left,
                                   int top,
                                   int width,
                                   int height,
                                   EntityType type_,
                                   std::unique_ptr<BasicHandler> handler_)
    : Entity(left, top, width, height, type_),
      handler(std::move(handler_)),
      background(height, std::vector<const Entity *>(width, nullptr)) {
    setBackground();
}

std::vector<const Entity *> ForegroundEntity::look(Direction direction) const {
    return handler->look(*this, direction);
}

void ForegroundEntity::restoreBackground() {
    handler->restoreBackground(*this);
}

void ForegroundEntity::setBackground() {
    handler->setBackground(*this);
}

std::vector<std::vector<const Entity *>> ForegroundEntity::snapshotBackground()
    const {
    return background;
}

}  // namespace Tanks::model
