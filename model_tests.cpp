#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include <cassert>
#include "doctest.h"
#include "model/blocks.h"
#include "model/game_model.h"
#include "model/projectile.h"

const std::array<Tanks::model::Direction, 4> DIRECTIONS = {
    Tanks::model::Direction::UP, Tanks::model::Direction::DOWN,
    Tanks::model::Direction::LEFT, Tanks::model::Direction::RIGHT};

TEST_CASE("Game creation") {
    Tanks::model::GameModel model;
    model.loadLevel(1);
    auto &brick00 = model.getEntityByCoords(0, 0);
    auto &tank = model.spawnPlayableTank(1, 0);
    CHECK(static_cast<Tanks::model::Entity *>(&tank) ==
          &model.getEntityByCoords(1, 0));
    CHECK(brick00.intersect(tank));
}

TEST_CASE("Single move") {
    Tanks::model::GameModel model;
    model.loadLevel(1);
    Tanks::model::Entity *brick00 = &model.getEntityByCoords(0, 0);
    auto &real_tank = model.spawnPlayableTank(0, 0);
    auto &tank = static_cast<Tanks::model::Entity &>(real_tank);
    CHECK(&tank == &model.getEntityByCoords(0, 0));

    real_tank.move(Tanks::model::Direction::DOWN);
    Tanks::model::Entity &ptr2 = model.getEntityByCoords(0, 0);
    model.nextTick();

    CHECK(brick00 == &model.getEntityByCoords(0, 0));
}

TEST_CASE("multiple moves") {
    Tanks::model::GameModel model;
    model.loadLevel(1);
    auto &realTank = model.spawnPlayableTank(0, 0);
    auto left = realTank.look(Tanks::model::Direction::LEFT);
    CHECK(left.empty());

    auto right = realTank.look(Tanks::model::Direction::RIGHT);
    CHECK(right[0] == right.back());
    CHECK(right[0]->getType() == Tanks::model::EntityType::LEFT_UP_CORNER);

    // [0,48) : Tank
    // [0,56) : Block
    for (int i = 0; i < 9; i++) {
        realTank.move(Tanks::model::Direction::DOWN);
    }
    model.nextTick();  // TODO make impossible more than one move
    right = realTank.look(Tanks::model::Direction::RIGHT);
    CHECK(right[0] == right[realTank.getHeight() - 2]);
    CHECK(right[0] != right.back());
    CHECK(right.back()->getType() == Tanks::model::EntityType::VERTICAL_BORDER);
}

TEST_CASE("Block check") {
    Tanks::model::GameModel model;
    model.loadLevel(1);
    {
        auto &block =
            static_cast<Tanks::model::Block &>(model.getEntityByCoords(0, 0));
        CHECK(block.isBulletPassable() == false);
        CHECK(block.isTankPassable() == false);
        CHECK(block.isDestroyable() == false);
    }
    {
        auto &block = static_cast<Tanks::model::Block &>(
            model.getEntityByCoords(100, 100));
        CHECK(block.getType() == Tanks::model::EntityType::FLOOR);
        CHECK(block.isDestroyable() == false);
        CHECK(block.isTankPassable() == true);
        CHECK(block.isBulletPassable() == true);
    }
}

TEST_CASE("Look for grass") {
    Tanks::model::GameModel model;
    model.loadLevel(1);
    auto &tank = model.spawnPlayableTank(80, 80);

    for (auto dir : DIRECTIONS) {
        for (auto &col : tank.look(dir)) {
            CHECK(col != nullptr);
            CHECK(col->getType() == Tanks::model::EntityType::FLOOR);
        }
    }
}

TEST_CASE("Tank simple shoot") {
    using namespace Tanks::model;

    GameModel model;
    model.loadLevel(1);
    auto &tank = model.spawnPlayableTank(Tanks::TILE_SIZE, Tanks::TILE_SIZE);
    tank.setDirection(Tanks::model::Direction::RIGHT);
    tank.shoot();
    auto &bullet = static_cast<Projectile &>(
        model.getEntityByCoords(Tanks::TILE_SIZE + Tanks::TANK_SIZE,
                                Tanks::TILE_SIZE + Tanks::TANK_SIZE / 2));

    CHECK(bullet.getType() == Tanks::model::EntityType::BULLET);
    CHECK(bullet.getTop() == Tanks::TILE_SIZE + Tanks::TANK_SIZE / 2);
    CHECK(bullet.getLeft() == Tanks::TILE_SIZE + Tanks::TANK_SIZE);
    CHECK(bullet.getDirection() == tank.getDirection());

    model.nextTick();
    CHECK(bullet.getTop() == Tanks::TILE_SIZE + Tanks::TANK_SIZE / 2);
}
