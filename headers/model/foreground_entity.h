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
                              std::unique_ptr<ForegroundHandler> handler);

    explicit ForegroundEntity(int left,
                              int top,
                              int entityId,
                              std::unique_ptr<ForegroundHandler> handler);

    [[nodiscard]] std::vector<const Entity *> look(Direction direction) const;

    [[nodiscard]] std::vector<std::vector<const Entity *>> snapshotBackground()
        const;

protected:
    [[nodiscard]] BasicHandler &getHandler() const;

private:
    const std::unique_ptr<BasicHandler> handler_;
    std::vector<std::vector<const Entity *>> background_;
};

}  // namespace Tanks::model

#endif  // TANKS_FOREGROUND_ENTITY_H
