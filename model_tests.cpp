#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN

#include <array>
#include <cassert>
#include <iostream>
#include "doctest.h"
#include "model/blocks.h"
#include "model/game_model.h"
#include "model/projectile.h"

using namespace Tanks;
using namespace Tanks::model;

const std::array<Tanks::model::Direction, 4> DIRECTIONS = {
    Tanks::model::Direction::UP, Tanks::model::Direction::DOWN,
    Tanks::model::Direction::LEFT, Tanks::model::Direction::RIGHT};

TEST_CASE("Game creation") {
    Tanks::model::GameModel model;
    model.loadLevel(1);
    auto &brick00 = model.getByCoords(TILE_SIZE, TILE_SIZE);

    CHECK(brick00.getType() == EntityType::FLOOR);
}

TEST_CASE("Single move and checking background") {
    Tanks::model::GameModel model;
    model.loadLevel(1);
    Tanks::model::Entity *brick00 = &model.getByCoords(TILE_SIZE, TILE_SIZE);

    auto &real_tank = model.spawnPlayableTank(TILE_SIZE, TILE_SIZE);
    CHECK(real_tank.getLeft() == TILE_SIZE);
    CHECK(real_tank.getTop() == TILE_SIZE);
    auto &tank = static_cast<Tanks::model::Entity &>(real_tank);
    CHECK(&tank == &real_tank);

    for (auto &row : real_tank.snapshotBackground()) {
        for (auto *entity : row) {
            CHECK(entity != nullptr);
        }
    }

    real_tank.move(Tanks::model::Direction::DOWN, real_tank.getSpeed());
    Tanks::model::Entity &ptr2 = model.getByCoords(TILE_SIZE, TILE_SIZE);
    model.nextTick();

    CHECK(brick00 == &model.getByCoords(TILE_SIZE, TILE_SIZE));
}

TEST_CASE("multiple moves") {
    Tanks::model::GameModel model;
    model.loadLevel(1);
    auto &realTank = model.spawnPlayableTank(TILE_SIZE, TILE_SIZE);
    auto left = realTank.look(Tanks::model::Direction::LEFT);
    CHECK(left[0] == left.back());
    CHECK(left[0] == &model.getByCoords(0, TILE_SIZE));

    auto right = realTank.look(Tanks::model::Direction::RIGHT);
    for (int row = realTank.getTop();
         row < realTank.getTop() + realTank.getHeight(); row++) {
        for (int col = realTank.getLeft() + realTank.getWidth();
             col <
             realTank.getLeft() + realTank.getWidth() + realTank.getSpeed();
             col++) {
            CHECK(std::find(right.begin(), right.end(),
                            &model.getByCoords(col, row)) != right.end());
        }
    }

    // [0,48) : Tank
    // [0,56) : Block
    const int MOVES = 9;
    for (int i = 0; i < MOVES; i++) {
        realTank.move(Tanks::model::Direction::DOWN);
    }

    CHECK(realTank.getTop() == TILE_SIZE + MOVES * realTank.getSpeed());

    model.nextTick();
    right = realTank.look(Tanks::model::Direction::RIGHT);
    CHECK(right.back()->getType() == Tanks::model::EntityType::FLOOR);
}

TEST_CASE("Block check") {
    Tanks::model::GameModel model;
    model.loadLevel(1);
    for (int row = 0; row < model.getHeight(); row++) {
        for (int col = 0; col < model.getWidth(); col++) {
            auto &entity = model.getByCoords(col, row);
            switch (entity.getType()) {
                case EntityType::FLOOR:
                    CHECK(entity.isTankPassable());
                    CHECK(entity.isBulletPassable());
                    break;
                case EntityType::BRICK:
                    CHECK_FALSE(entity.isTankPassable());
                    CHECK_FALSE(entity.isBulletPassable());
                    break;
                case EntityType::STEEL:
                    CHECK_FALSE(entity.isTankPassable());
                    CHECK_FALSE(entity.isBulletPassable());
                    break;
                case EntityType::WATER:
                    CHECK_FALSE(entity.isTankPassable());
                    CHECK(entity.isBulletPassable());
                    break;
            }
        }
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
    using namespace Tanks;

    GameModel model;
    model.loadLevel(1);
    auto &tank = model.spawnPlayableTank(Tanks::TILE_SIZE, Tanks::TILE_SIZE);
    tank.setDirection(Tanks::model::Direction::RIGHT);
    tank.shoot();
    auto &bullet = dynamic_cast<Projectile &>(
        model.getByCoords(Tanks::TILE_SIZE + Tanks::TANK_SIZE,
                          Tanks::TILE_SIZE + Tanks::TANK_SIZE / 2));

    CHECK(bullet.getType() == Tanks::model::EntityType::BULLET);
    CHECK(bullet.getTop() == Tanks::TILE_SIZE + Tanks::TANK_SIZE / 2);
    CHECK(bullet.getLeft() == Tanks::TILE_SIZE + Tanks::TANK_SIZE);
    CHECK(bullet.getDirection() == tank.getDirection());

    model.nextTick();
    CHECK(bullet.getTop() == Tanks::TILE_SIZE + Tanks::TANK_SIZE / 2);
    CHECK(bullet.getLeft() == TILE_SIZE + TANK_SIZE + bullet.getSpeed());
}

TEST_CASE("Shoot, bullet die") {
    using namespace Tanks::model;
    using namespace Tanks;

    GameModel model;
    model.loadLevel(1);
    auto &tank = model.spawnPlayableTank(Tanks::TILE_SIZE, Tanks::TILE_SIZE);
    tank.setDirection(Tanks::model::Direction::RIGHT);
    tank.shoot();
    auto &bullet = dynamic_cast<Projectile &>(
        model.getByCoords(Tanks::TILE_SIZE + Tanks::TANK_SIZE,
                          Tanks::TILE_SIZE + Tanks::TANK_SIZE / 2));

    CHECK(bullet.getType() == Tanks::model::EntityType::BULLET);
    CHECK(bullet.getTop() == Tanks::TILE_SIZE + Tanks::TANK_SIZE / 2);
    CHECK(bullet.getLeft() == Tanks::TILE_SIZE + Tanks::TANK_SIZE);
    CHECK(bullet.getDirection() == tank.getDirection());

    model.nextTick();
    CHECK(bullet.getTop() == Tanks::TILE_SIZE + Tanks::TANK_SIZE / 2);
    CHECK(bullet.getLeft() == TILE_SIZE + TANK_SIZE + bullet.getSpeed());
    for (int i = 0; i < 50; i++) {
        model.nextTick();
    }
}

TEST_CASE("3 Bullets destroy 3 bricks") {
    using namespace Tanks::model;
    using namespace Tanks;

    GameModel model;
    model.loadLevel(1);
    auto &tank =
        model.spawnPlayableTank(Tanks::TILE_SIZE, Tanks::TILE_SIZE * 2);

    tank.setDirection(Tanks::model::Direction::RIGHT);
    constexpr std::array<std::pair<int, int>, 3> COORDS = {
        std::pair{TILE_SIZE * 3, TILE_SIZE * 2},
        {TILE_SIZE * 7, TILE_SIZE * 2},
        {TILE_SIZE * 13, TILE_SIZE * 2}};
    for (auto [x, y] : COORDS) {
        tank.shoot();
        auto &bullet = dynamic_cast<Projectile &>(
            model.getByCoords(tank.getLeft() + tank.getWidth(),
                              tank.getTop() + tank.getHeight() / 2));

        auto &brick = model.getByCoords(x, y);
        int t = bullet.dist(brick) / BULLET_SPEED +
                (bullet.dist(brick) % BULLET_SPEED != 0);
        for (int i = 0; i < t; i++) {
            auto &brickNow = model.getByCoords(x, y);
            CHECK(&brick == &brickNow);
            model.nextTick();
        }
        auto &floor = model.getByCoords(x, y);
        CHECK(floor.getType() == Tanks::model::EntityType::FLOOR);
    }
}

TEST_CASE("Bullet fly above water") {
    using namespace Tanks::model;
    using namespace Tanks;
    using Tanks::model::Direction;
    using Tanks::model::EntityType;

    GameModel model;
    model.loadLevel(1);
    auto &tank = model.spawnPlayableTank(TILE_SIZE * 4, TILE_SIZE * 15);

    tank.setDirection(Tanks::model::Direction::RIGHT);
    auto &brick = model.getByCoords(TILE_SIZE * 19, TILE_SIZE * 15);
    CHECK(brick.getType() == EntityType::BRICK);
    auto &water = model.getByCoords(TILE_SIZE * 15, TILE_SIZE * 15);
    CHECK(water.getType() == Tanks::model::EntityType::WATER);
    tank.shoot();
    auto &bullet = dynamic_cast<Projectile &>(
        model.getByCoords(tank.getLeft() + tank.getWidth(),
                          tank.getTop() + tank.getHeight() / 2));

    int tWater = bullet.dist(water) / BULLET_SPEED +
                 (bullet.dist(water) % BULLET_SPEED != 0);
    for (int i = 0; i < tWater; i++) {
        auto &waterNow = model.getByCoords(
            TILE_SIZE * 15, TILE_SIZE * 15 + tank.getHeight() / 2);
        CHECK(&water == &waterNow);
        model.nextTick();
    }

    auto &notWaterNow = model.getByCoords(
        TILE_SIZE * 15 + 2, TILE_SIZE * 15 + tank.getHeight() / 2);
    CHECK(&notWaterNow == &bullet);

    int tBrick = bullet.dist(brick) / BULLET_SPEED +
                 (bullet.dist(brick) % BULLET_SPEED != 0);
    for (int i = 0; i < tBrick; i++) {
        auto &brickNow = model.getByCoords(TILE_SIZE * 19, TILE_SIZE * 15);
        CHECK(&brick == &brickNow);
        model.nextTick();
    }

    auto &waterNow = model.getByCoords(TILE_SIZE * 15 + 1,
                                       TILE_SIZE * 15 + tank.getHeight() / 2);
    CHECK(&waterNow == &water);

    auto &floor = model.getByCoords(TILE_SIZE * 19, TILE_SIZE * 15);
    CHECK(floor.getType() == Tanks::model::EntityType::FLOOR);
}
