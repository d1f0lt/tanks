//
// Created by ilma4 on 2/18/22.
//

#pragma once
#ifndef TANKS_ENTITY_HOLDER_H
#define TANKS_ENTITY_HOLDER_H

#include <map>
#include "entity.h"
#include "memory"

namespace tanks::model {
class EntityHolder {
public:
    void addEntity(std::unique_ptr<Entity> entity);

    void removeEntity(int id);

private:
    std::map<int, std::unique_ptr<Entity>> entityHolder;
};
}  // namespace tanks::model

#endif  // TANKS_ENTITY_HOLDER_H
