#ifndef TANKS_ENTITY_HOLDER_H
#define TANKS_ENTITY_HOLDER_H

#include <map>
#include <vector>
#include "entity.h"
#include "memory"

namespace Tanks::model {
class EntityHolder {
public:
    Entity &insert(std::unique_ptr<Entity> entity);
    void remove(Entity &entity);

private:
    std::vector<std::unique_ptr<Entity>> buffer;
};
}  // namespace Tanks::model

#endif  // TANKS_ENTITY_HOLDER_H
