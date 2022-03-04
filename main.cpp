#include <cassert>
#include "game_model.h"

int main() {
    {
        Tanks::model::GameModel model;
        model.loadLevel(1);
        auto &brick00 = model.getEntityByCoords(0, 0);
        auto &tank = model.spawnPlayableTank(1, 0);
        assert(static_cast<Tanks::model::Entity *>(&tank) ==
               &model.getEntityByCoords(1, 0));
    }

    {
        Tanks::model::GameModel model;
        model.loadLevel(1);
        Tanks::model::Entity *brick00 = &model.getEntityByCoords(0, 0);
        auto &real_tank = model.spawnPlayableTank(0, 0);
        auto &tank = static_cast<Tanks::model::Entity &>(real_tank);
        assert(&tank == &model.getEntityByCoords(0, 0));

        real_tank.move(Tanks::model::Direction::DOWN);
        Tanks::model::Entity &ptr2 = model.getEntityByCoords(0, 0);

        assert(brick00 == &model.getEntityByCoords(0, 0));
    }

    return 0;
}
