#ifndef TANKS_GROUPED_ENTITIES_H
#define TANKS_GROUPED_ENTITIES_H

#include <unordered_map>
#include <vector>
#include "entity.h"

namespace Tanks::model {
class GroupedEntities {
public:
    void insert(Entity &entity);
    void erase(Entity &entity);

    [[nodiscard]] std::vector<std::vector<Entity *>> snapshotAll() const;

    [[nodiscard]] const std::vector<std::vector<Entity *>> &getAllByLink()
        const;

private:
    std::vector<std::vector<Entity *>> entities_{25};
};

}  // namespace Tanks::model

#endif  // TANKS_GROUPED_ENTITIES_H
