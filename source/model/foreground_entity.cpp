#include "model/foreground_entity.h"
#include "model/handler.h"

namespace Tanks::model {

ForegroundEntity::ForegroundEntity(int left,
                                   int top,
                                   int entityId,
                                   std::unique_ptr<ForegroundHandler> handler)
    : Entity(left, top, entityId, std::move(handler)) {
}

std::vector<const Entity *> ForegroundEntity::look(Direction direction) const {
    return dynamic_cast<MovableHandler &>(getHandler()).look(direction);
}

std::vector<std::vector<const Entity *>> ForegroundEntity::snapshotBackground()
    const {
    return dynamic_cast<ForegroundHandler &>(getHandler()).snapshotBackground();
}

}  // namespace Tanks::model
