#include "model/foreground_entity.h"
#include <cassert>

namespace Tanks::model {
ForegroundEntity::ForegroundEntity(int left,
                                   int top,
                                   std::unique_ptr<ForegroundHandler> handler)
    : Entity(left, top), handler_(std::move(handler)) {
}

ForegroundEntity::ForegroundEntity(int left,
                                   int top,
                                   int id,
                                   std::unique_ptr<ForegroundHandler> handler)
    : Entity(left, top, id), handler_(std::move(handler)) {
}

std::vector<const Entity *> ForegroundEntity::look(Direction direction) const {
    return dynamic_cast<MovableHandler &>(*handler_).look(direction);
}

std::vector<std::vector<const Entity *>> ForegroundEntity::snapshotBackground()
    const {
    return background_;
}

}  // namespace Tanks::model
