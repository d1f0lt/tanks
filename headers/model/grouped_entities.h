#ifndef TANKS_GROUPED_ENTITIES_H
#define TANKS_GROUPED_ENTITIES_H

#include "entity.h"

namespace Tanks::model {
class GroupedEntities {
public:
    explicit GroupedEntities();

    void insert(Entity &entity);
    void erase(Entity &entity);

    [[nodiscard]] const std::vector<std::vector<const Entity *>> &getAll()
        const;

private:
    std::vector<std::vector<const Entity *>> entities;
};

}  // namespace Tanks::model

#endif  // TANKS_GROUPED_ENTITIES_H
