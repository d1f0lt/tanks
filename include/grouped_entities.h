//
// Created by ilma4 on 2/18/22.
//

#pragma once
#ifndef TANKS_GROUPED_ENTITIES_H
#define TANKS_GROUPED_ENTITIES_H

#include "entity.h"

namespace tanks::model {
struct DrawIterator {};

class GroupedEntities {
public:
    void addEntity(Entity *entity);

    void removeEntity(Entity *entity);

    [[nodiscard]] DrawIterator getIterator() const;

private:
    std::vector<std::vector<const Entity *>> entities;
};

}  // namespace tanks::model

#endif  // TANKS_GROUPED_ENTITIES_H
