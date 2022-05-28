#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN

#include <array>
#include <boost/asio/connect.hpp>
#include <thread>
#include "doctest.h"
#include "model/client_game_model.h"
#include "model/projectile.h"
#include "model/server_game_model.h"
#include "model/tank.h"

using namespace Tanks;
using namespace Tanks::model;
using namespace boost::asio::ip;

const std::array<Tanks::model::Direction, 4> DIRECTIONS = {
    Tanks::model::Direction::UP, Tanks::model::Direction::DOWN,
    Tanks::model::Direction::LEFT, Tanks::model::Direction::RIGHT};

namespace {
class DebugServer : public ServerModel {
public:
    using GameModel::getHandler;
};
}  // namespace

TEST_CASE("Game creation") {
    Tanks::model::ServerModel model;
    model.loadLevel(1);
    auto &brick00 = model.getByCoords(TILE_SIZE, TILE_SIZE);

    CHECK(brick00.getType() == EntityType::FLOOR);
}

#define ADD_PLAYER(model, i)                                           \
    tcp::socket server##i(io_context);                                 \
    auto connection##i =                                               \
        std::thread([&]() { server##i = acceptor.accept(); });         \
                                                                       \
    tcp::socket client##i(io_context);                                 \
    boost::asio::connect(                                              \
        client##i,                                                     \
        tcp::resolver(io_context).resolve(acceptor.local_endpoint())); \
    connection##i.join();                                              \
    auto id##i = model.addPlayer(server);                              \
    model.nextTick();                                                  \
    auto &tank##i = dynamic_cast<Tank &>(model.getById(id##i)->get()); \
    auto &handler##i = dynamic_cast<TankHandler &>(model.getHandler(tank##i));

#define INIT_GAME()                                                  \
    DebugServer serverModel;                                         \
    serverModel.loadLevel(1);                                        \
    boost::asio::io_context io_context;                              \
    tcp::acceptor acceptor(io_context, tcp::endpoint(tcp::v4(), 0)); \
    ADD_PLAYER(serverModel, );                                       \
    ClientModel clientModel(id, std::move(client));                  \
    clientModel.loadLevel(1);                                        \
    clientModel.nextTick();

[[nodiscard]] bool isSame(const Tank &a, const Tank &b) {
}

bool operator==(const Entity &a, const Entity &b) {
    if (a.getId() != b.getId()) {
        return false;
    }
    if (a.getType() != b.getType()) {
        return false;
    }
    if (a.getTop() != b.getTop() || a.getLeft() != b.getLeft()) {
        return false;
    }
    if (a.getHeight() != b.getHeight() || a.getWidth() != b.getWidth()) {
        return false;
    }
    if (a.getStrength() != b.getStrength()) {
        return false;
    }
    if (a.isTankPassable() != b.isTankPassable() ||
        a.isBulletPassable() != b.isBulletPassable()) {
        return false;
    }
    return true;
}

[[nodiscard]] bool operator!=(const Entity &a, const Entity &b) {
    return !(a == b);
}

[[nodiscard]] static bool isSynced(GameModel &model1, GameModel &model2) {
    auto all1 = model1.getAll();
    auto all2 = model2.getAll();
    for (const auto &vec : all1) {
        for (const auto *ptr : vec) {
            const auto &entity1 = *ptr;
            auto &entity2 = model2.getById(entity1.getId())->get();
            if (entity1 != entity2) {
                return false;
            }
        }
    }
    return true;
}

TEST_CASE("Single move and checking background") {
    Tanks::model::ServerModel serverModel;
    serverModel.loadLevel(1);
    Tanks::model::Entity *brick00 =
        &serverModel.getByCoords(TILE_SIZE, TILE_SIZE);

    boost::asio::io_context io_context;
    tcp::acceptor acceptor(io_context, tcp::endpoint(tcp::v4(), 0));
    tcp::socket server(io_context);
    auto connection = std::thread([&]() { server = acceptor.accept(); });

    tcp::socket client(io_context);
    boost::asio::connect(
        client, tcp::resolver(io_context).resolve(acceptor.local_endpoint()));
    connection.join();

    int id = serverModel.addPlayer(server);

    ClientModel clientModel(id, std::move(client));
    clientModel.loadLevel(1);

    serverModel.nextTick();
    clientModel.nextTick();
    auto &tank = dynamic_cast<Tank &>(serverModel.getById(id)->get());

    CHECK(tank.getLeft() == TILE_SIZE);
    CHECK(tank.getTop() == TILE_SIZE);

    for (const auto &row : tank.snapshotBackground()) {
        for (const auto *entity : row) {
            CHECK(entity != nullptr);
        }
    }

    auto controller = clientModel.getHandler();

    controller.move(Tanks::model::Direction::DOWN, tank.getSpeed());

    Tanks::model::Entity &ptr2 = serverModel.getByCoords(TILE_SIZE, TILE_SIZE);
    serverModel.nextTick();
    clientModel.nextTick();

    CHECK(tank.getTop() == TILE_SIZE + tank.getSpeed());
    CHECK(brick00 == &serverModel.getByCoords(TILE_SIZE, TILE_SIZE));
}

TEST_CASE("multiple moves") {
    INIT_GAME();

    int speed = tank.getSpeed();
    auto left = tank.look(Tanks::model::Direction::LEFT);
    CHECK(left[0] == left.back());
    CHECK(left[0] == &serverModel.getByCoords(0, TILE_SIZE));

    auto right = tank.look(Tanks::model::Direction::RIGHT);
    for (int row = tank.getTop(); row < tank.getTop() + tank.getHeight();
         row++) {
        for (int col = tank.getLeft() + tank.getWidth();
             col < tank.getLeft() + tank.getWidth() + tank.getSpeed(); col++) {
            CHECK(std::find(right.begin(), right.end(),
                            &serverModel.getByCoords(col, row)) != right.end());
        }
    }

    // [0,48) : Tank
    // [0,56) : Block
    auto controller = clientModel.getHandler();
    const int MOVES = 9;
    for (int i = 0; i < MOVES; i++) {
        controller.move(Tanks::model::Direction::DOWN, speed);
        std::this_thread::sleep_for(std::chrono::milliseconds(50));
        serverModel.nextTick();
        clientModel.nextTick();
        CHECK(isSynced(serverModel, clientModel));
        CHECK(tank.getTop() == TILE_SIZE + (i + 1) * tank.getSpeed());
    }

    serverModel.nextTick();
    clientModel.nextTick();

    CHECK(tank.getTop() == TILE_SIZE + MOVES * tank.getSpeed());
    right = tank.look(Tanks::model::Direction::RIGHT);
    CHECK(right.back()->getType() == Tanks::model::EntityType::FLOOR);
}

TEST_CASE("Can't do 2 moves on 1 tick") {
    INIT_GAME();
    handler.setPosition(TILE_SIZE, TILE_SIZE);

    int left = tank.getLeft();
    int top = tank.getTop();
    handler.move(Tanks::model::Direction::RIGHT);
    int left1 = tank.getLeft();
    int top1 = tank.getTop();
    handler.move(Tanks::model::Direction::RIGHT);
    CHECK(left1 == tank.getLeft());
    CHECK(top1 == tank.getTop());
    serverModel.nextTick();
    CHECK(left1 == tank.getLeft());
    CHECK(top1 == tank.getTop());
}
TEST_CASE("Can move and shoot on 1 tick") {
    INIT_GAME();

    int left = tank.getLeft(), top = tank.getTop();
    handler.move(Tanks::model::Direction::RIGHT);
    int left1 = tank.getLeft(), top1 = tank.getTop();
    handler.move(Tanks::model::Direction::RIGHT);
    CHECK(left1 == left + tank.getSpeed());
    CHECK(top1 == top);
    handler.shoot();
    CHECK(left1 == left + tank.getSpeed());
    CHECK(top1 == top);
}

TEST_CASE("Can't shoot then move on 1 tick") {
    INIT_GAME();
    handler.setPosition(TILE_SIZE, TILE_SIZE);
    int left = tank.getLeft(), top = tank.getTop();
    handler.move(Tanks::model::Direction::RIGHT, 0);
    CHECK(serverModel.getTick() == 1);
    handler.shoot();
    CHECK(serverModel.getTick() == 1);
    handler.move(Tanks::model::Direction::RIGHT);
    CHECK(left == tank.getLeft());
    CHECK(top == tank.getTop());
    serverModel.nextTick();
    CHECK(serverModel.getTick() == 2);
}

TEST_CASE("Block check") {
    INIT_GAME();
    for (int row = 0; row < serverModel.getHeight(); row++) {
        for (int col = 0; col < serverModel.getWidth(); col++) {
            auto &entity = serverModel.getByCoords(col, row);
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
    INIT_GAME();
    handler.setPosition(80, 80);

    for (auto dir : DIRECTIONS) {
        for (auto &col : tank.look(dir)) {
            CHECK(col != nullptr);
            CHECK(col->getType() == Tanks::model::EntityType::FLOOR);
        }
    }
}

TEST_CASE("Tank simple shoot") {
    INIT_GAME();
    handler.setPosition(TILE_SIZE, TILE_SIZE);
    handler.move(Tanks::model::Direction::RIGHT, 0);
    handler.shoot();
    auto &bullet = dynamic_cast<Projectile &>(
        serverModel.getByCoords(Tanks::TILE_SIZE + Tanks::TANK_SIZE,
                                Tanks::TILE_SIZE + Tanks::TANK_SIZE / 2));

    CHECK(bullet.getType() == Tanks::model::EntityType::BULLET);
    CHECK(bullet.getTop() == Tanks::TILE_SIZE + Tanks::TANK_SIZE / 2);
    CHECK(bullet.getLeft() == Tanks::TILE_SIZE + Tanks::TANK_SIZE);
    CHECK(bullet.getDirection() == tank.getDirection());

    serverModel.nextTick();
    CHECK(bullet.getTop() == Tanks::TILE_SIZE + Tanks::TANK_SIZE / 2);
    CHECK(bullet.getLeft() == TILE_SIZE + TANK_SIZE + bullet.getSpeed());
}

TEST_CASE("Shoot, bullet die") {
    INIT_GAME();
    handler.setPosition(TILE_SIZE, TILE_SIZE);
    handler.setDirection(Tanks::model::Direction::RIGHT);
    handler.shoot();
    auto &bullet = dynamic_cast<Projectile &>(
        serverModel.getByCoords(Tanks::TILE_SIZE + Tanks::TANK_SIZE,
                                Tanks::TILE_SIZE + Tanks::TANK_SIZE / 2));

    CHECK(bullet.getType() == Tanks::model::EntityType::BULLET);
    CHECK(bullet.getTop() == Tanks::TILE_SIZE + Tanks::TANK_SIZE / 2);
    CHECK(bullet.getLeft() == Tanks::TILE_SIZE + Tanks::TANK_SIZE);
    CHECK(bullet.getDirection() == tank.getDirection());

    serverModel.nextTick();
    CHECK(bullet.getTop() == Tanks::TILE_SIZE + Tanks::TANK_SIZE / 2);
    CHECK(bullet.getLeft() == TILE_SIZE + TANK_SIZE + bullet.getSpeed());
    for (int i = 0; i < 50; i++) {
        serverModel.nextTick();
    }
}

TEST_CASE("3 Bullets destroy 3 bricks") {
    INIT_GAME();
    handler.setPosition(TILE_SIZE, TILE_SIZE * 2);

    handler.setDirection(Tanks::model::Direction::RIGHT);
    constexpr std::array<std::pair<int, int>, 3> COORDS = {
        std::pair{TILE_SIZE * 3, TILE_SIZE * 2},
        {TILE_SIZE * 7, TILE_SIZE * 2},
        {TILE_SIZE * 13, TILE_SIZE * 2}};
    for (auto [x, y] : COORDS) {
        handler.shoot();
        auto &bullet = dynamic_cast<Projectile &>(
            serverModel.getByCoords(tank.getLeft() + tank.getWidth(),
                                    tank.getTop() + tank.getHeight() / 2));

        auto &brick = serverModel.getByCoords(x, y);
        int t = bullet.dist(brick) / BULLET_SPEED +
                (bullet.dist(brick) % BULLET_SPEED != 0);
        for (int i = 0; i < t; i++) {
            auto &brickNow = serverModel.getByCoords(x, y);
            CHECK(&brick == &brickNow);
            serverModel.nextTick();
        }
        auto &floor = serverModel.getByCoords(x, y);
        CHECK(floor.getType() == Tanks::model::EntityType::FLOOR);
        for (int i = 0; i < RELOAD_TICKS; i++) {
            serverModel.nextTick();
        }
    }
}

TEST_CASE("Bullet fly above water") {
    INIT_GAME();
    handler.setPosition(TILE_SIZE * 4, TILE_SIZE * 15);

    handler.setDirection(Tanks::model::Direction::RIGHT);
    auto &brick = serverModel.getByCoords(TILE_SIZE * 19, TILE_SIZE * 15);
    CHECK(brick.getType() == EntityType::BRICK);
    auto &water = serverModel.getByCoords(TILE_SIZE * 15, TILE_SIZE * 15);
    CHECK(water.getType() == Tanks::model::EntityType::WATER);
    handler.shoot();
    auto &bullet = dynamic_cast<Projectile &>(
        serverModel.getByCoords(tank.getLeft() + tank.getWidth(),
                                tank.getTop() + tank.getHeight() / 2));

    int tWater = bullet.dist(water) / BULLET_SPEED +
                 (bullet.dist(water) % BULLET_SPEED != 0);
    for (int i = 0; i < tWater; i++) {
        auto &waterNow = serverModel.getByCoords(
            TILE_SIZE * 15, TILE_SIZE * 15 + tank.getHeight() / 2);
        CHECK(&water == &waterNow);
        serverModel.nextTick();
    }

    int tBrick = bullet.dist(brick) / BULLET_SPEED +
                 (bullet.dist(brick) % BULLET_SPEED != 0);
    for (int i = 0; i < tBrick; i++) {
        auto &brickNow =
            serverModel.getByCoords(TILE_SIZE * 19, TILE_SIZE * 15);
        CHECK(&brick == &brickNow);
        serverModel.nextTick();
    }

    auto &waterNow = serverModel.getByCoords(
        TILE_SIZE * 15 + 1, TILE_SIZE * 15 + tank.getHeight() / 2);
    CHECK(&waterNow == &water);

    auto &floor = serverModel.getByCoords(TILE_SIZE * 19, TILE_SIZE * 15);
    CHECK(floor.getType() == Tanks::model::EntityType::FLOOR);
}

TEST_CASE("Bullet destroy on creation") {
    INIT_GAME();
    constexpr int TANK_LEFT = TILE_SIZE * 3;
    constexpr int TANK_TOP = TILE_SIZE * 1 + TILE_SIZE - TANK_SIZE;
    handler.setPosition(TANK_LEFT, TANK_TOP);
    handler.setDirection(Tanks::model::Direction::DOWN);

    auto &brick = serverModel.getByCoords(TILE_SIZE * 3, TILE_SIZE * 2);
    CHECK(brick.getType() == EntityType::BRICK);

    handler.shoot();

    auto &floor = serverModel.getByCoords(TILE_SIZE * 3, TILE_SIZE * 2);
    CHECK(floor.getType() == Tanks::model::EntityType::FLOOR);
}

TEST_CASE("Shoot static tank") {
    INIT_GAME();
    ADD_PLAYER(serverModel, 2);

    handler.setPosition(TILE_SIZE * 2, TILE_SIZE);
    auto &tank1 = tank;
    handler.setPosition(TILE_SIZE * 2, TILE_SIZE);
    handler2.setPosition(TILE_SIZE * 3, TILE_SIZE);

    handler.setDirection(Tanks::model::Direction::RIGHT);
    handler.shoot();
    serverModel.nextTick();
    auto &floor = serverModel.getByCoords(TILE_SIZE * 3, TILE_SIZE);
    CHECK(floor.getType() == EntityType::FLOOR);
}

TEST_CASE("tank move on bullet") {
    INIT_GAME();
    ADD_PLAYER(serverModel, 2);

    handler.setPosition(TILE_SIZE * 2, TILE_SIZE);
    handler2.setPosition(TILE_SIZE, TILE_SIZE);

    handler.setDirection(Tanks::model::Direction::LEFT);
    handler.shoot();
    handler2.move(Tanks::model::Direction::RIGHT);
    auto &floor = serverModel.getByCoords(TILE_SIZE, TILE_SIZE);
    CHECK(floor.getType() == EntityType::FLOOR);
}

TEST_CASE("Bullet destroy 2 blocks on creation") {
    INIT_GAME();
    handler.setPosition(TILE_SIZE * 10 + TILE_SIZE / 2, TILE_SIZE * 15);

    handler.setDirection(Tanks::model::Direction::DOWN);
    auto &brick1 = serverModel.getByCoords(TILE_SIZE * 10, TILE_SIZE * 16);
    auto &brick2 = serverModel.getByCoords(TILE_SIZE * 11, TILE_SIZE * 16);
    CHECK(brick1.getType() == EntityType::BRICK);
    CHECK(brick2.getType() == EntityType::BRICK);

    handler.shoot();

    serverModel.nextTick();

    auto &floor1 = serverModel.getByCoords(TILE_SIZE * 10, TILE_SIZE * 16);
    auto &floor2 = serverModel.getByCoords(TILE_SIZE * 11, TILE_SIZE * 16);
    CHECK(floor1.getType() == EntityType::FLOOR);
    CHECK(floor2.getType() == EntityType::FLOOR);
}

TEST_CASE("Bullet destroy 2 blocks on nextTick") {
    INIT_GAME();

    handler.setPosition(TILE_SIZE * 10 + TILE_SIZE / 2,
                        TILE_SIZE * 15 - BULLET_SIZE - 1);
    auto &brick1 = serverModel.getByCoords(TILE_SIZE * 10, TILE_SIZE * 16);
    auto &brick2 = serverModel.getByCoords(TILE_SIZE * 11, TILE_SIZE * 16);
    CHECK(brick1.getType() == EntityType::BRICK);
    CHECK(brick2.getType() == EntityType::BRICK);

    handler.setDirection(Tanks::model::Direction::DOWN);
    handler.shoot();

    serverModel.nextTick();

    auto &floor1 = serverModel.getByCoords(TILE_SIZE * 10, TILE_SIZE * 16);
    auto &floor2 = serverModel.getByCoords(TILE_SIZE * 11, TILE_SIZE * 16);
    CHECK(floor1.getType() == EntityType::FLOOR);
    CHECK(floor2.getType() == EntityType::FLOOR);
}

TEST_CASE("Bullet destroy 1 of blocks on creation, then shoot again") {
    INIT_GAME();
    handler.setPosition(TILE_SIZE * 10 + TILE_SIZE - TANK_SIZE, TILE_SIZE * 15);
    handler.setDirection(Tanks::model::Direction::DOWN);

    auto &brick1 = serverModel.getByCoords(TILE_SIZE * 10, TILE_SIZE * 16);
    auto &brick2 = serverModel.getByCoords(TILE_SIZE * 11, TILE_SIZE * 16);
    CHECK(brick1.getType() == EntityType::BRICK);
    CHECK(brick2.getType() == EntityType::BRICK);

    handler.shoot();

    auto &floor1 = serverModel.getByCoords(TILE_SIZE * 10, TILE_SIZE * 16);
    auto &floor2 = serverModel.getByCoords(TILE_SIZE * 11, TILE_SIZE * 16);
    CHECK(floor1.getType() == EntityType::FLOOR);
    CHECK(&floor2 == &brick2);

    serverModel.nextTick();
    handler.shoot();
    serverModel.nextTick();
}
