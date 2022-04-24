#ifndef TANKS_FOREGROUND_ENTITY_H
#define TANKS_FOREGROUND_ENTITY_H

#include <memory>
#include "entity.h"
#include "model/handler.h"

namespace Tanks::model {
class ForegroundEntity : public Entity {
    friend ForegroundHandler;

public:
    ForegroundEntity(int left,
                     int top,
                     int width,
                     int height,
                     EntityType type,
                     std::unique_ptr<BasicHandler> handler_);

    [[nodiscard]] std::vector<const Entity *> look(Direction direction) const;

    [[nodiscard]] std::vector<std::vector<const Entity *>> snapshotBackground()
        const;

protected:
    void restoreBackground();
    void setBackground();

protected:
    std::unique_ptr<BasicHandler> handler;
    std::vector<std::vector<const Entity *>> background;
};

}  // namespace Tanks::model

#endif  // TANKS_FOREGROUND_ENTITY_H
