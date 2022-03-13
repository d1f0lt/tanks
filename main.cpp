#include <cassert>
#include "blocks.h"
#include "game_model.h"

int main() {
    {
        Tanks::model::GameModel model;
        model.loadLevel(1);
        auto &brick00 = model.getEntityByCoords(0, 0);
        auto &tank = model.spawnPlayableTank(1, 0);
        assert(static_cast<Tanks::model::Entity *>(&tank) ==
               &model.getEntityByCoords(1, 0));
        assert(brick00.intersect(tank));
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

    {
        Tanks::model::GameModel model;
        model.loadLevel(1);
        auto &realTank = model.spawnPlayableTank(0, 0);
        auto left = realTank.whatsOn(Tanks::model::Direction::LEFT);
        assert(left.empty());

        auto right = realTank.whatsOn(Tanks::model::Direction::RIGHT);
        assert(right[0] == right.back());
        assert(right[0]->getType() == Tanks::model::EntityType::LEFT_UP_CORNER);

        // [0,48) : Tank
        // [0,56) : Block
        for (int i = 0; i < 9; i++) {
            realTank.move(Tanks::model::Direction::DOWN);
        }
        right = realTank.whatsOn(Tanks::model::Direction::RIGHT);
        assert(right[0] == right[realTank.getHeight() - 2]);
        assert(right[0] != right.back());
        assert(right.back()->getType() ==
               Tanks::model::EntityType::VERTICAL_BORDER);
    }

    {
        Tanks::model::GameModel model;
        model.loadLevel(1);
        {
            auto &block = static_cast<Tanks::model::Block &>(
                model.getEntityByCoords(0, 0));
            assert(block.isBulletPassable() == false);
            assert(block.isTankPassable() == false);
            assert(block.isDestroyable() == false);
        }
        {
            auto &block = static_cast<Tanks::model::Block &>(
                model.getEntityByCoords(100, 100));
            assert(block.getType() == Tanks::model::EntityType::FLOOR);
            assert(block.isDestroyable() == false);
            assert(block.isTankPassable() == true);
            assert(block.isBulletPassable() == true);
        }
    }

    return 0;
}
