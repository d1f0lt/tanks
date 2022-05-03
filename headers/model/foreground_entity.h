#ifndef TANKS_FOREGROUND_ENTITY_H
#define TANKS_FOREGROUND_ENTITY_H

#include <memory>
#include <vector>
#include "entity.h"
#include "model/handler.h"

namespace Tanks::model {
class ForegroundEntity : public Entity {
    friend ForegroundHandler;

public:
    explicit ForegroundEntity(int left,
                              int top,
                              std::unique_ptr<BasicHandler> handler_);

    explicit ForegroundEntity(int left,
                              int top,
                              int id,
                              std::unique_ptr<BasicHandler> handler);

    [[nodiscard]] std::vector<const Entity *> look(Direction direction) const;

    [[nodiscard]] std::vector<std::vector<const Entity *>> snapshotBackground()
        const;

protected:
    void restoreBackground();
    void setBackground();
    std::unique_ptr<BasicHandler> handler;

private:
    std::vector<std::vector<const Entity *>> background;
};

}  // namespace Tanks::model

#endif  // TANKS_FOREGROUND_ENTITY_H
