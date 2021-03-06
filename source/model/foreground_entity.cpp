#include "model/foreground_entity.h"
#include "model/handler.h"

namespace Tanks::model {

ForegroundEntity::ForegroundEntity(int left,
                                   int top,
                                   int entityId,
                                   std::unique_ptr<ForegroundHandler> handler)
    : Entity(left, top, entityId, std::move(handler)) {
    background_.reserve(16);
}

std::vector<const Entity *> ForegroundEntity::look(Direction direction) const {
    return dynamic_cast<MovableHandler &>(getHandler()).look(direction);
}

std::vector<int> ForegroundEntity::snapshotBackground() const {
    return dynamic_cast<ForegroundHandler &>(getHandler()).snapshotBackground();
}
// bool ForegroundEntity::isDieOnCreation() {
//     return false
// }

}  // namespace Tanks::model
