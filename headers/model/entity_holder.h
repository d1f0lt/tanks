#ifndef TANKS_ENTITY_HOLDER_H
#define TANKS_ENTITY_HOLDER_H

#include <map>
#include <memory>
#include <vector>
#include "entity.h"

namespace Tanks::model {
class EntityHolder {
public:
    Entity &insert(std::unique_ptr<Entity> entity);
    void erase(Entity &entity);

private:
    std::vector<std::unique_ptr<Entity>> buffer_;
};
}  // namespace Tanks::model

#endif  // TANKS_ENTITY_HOLDER_H
