#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN

#include <array>
#include <boost/asio/connect.hpp>
#include <thread>
#include "doctest.h"
#include "model/client_game_model.h"
#include "model/network_utils.h"
#include "model/projectile.h"
#include "model/server_game_model.h"
#include "model/tank.h"

using namespace Tanks;
using namespace Tanks::model;
using boost::asio::io_context;
using boost::asio::ip::tcp;

const std::array<Tanks::model::Direction, 4> DIRECTIONS = {
    Direction::UP, Direction::DOWN, Direction::LEFT, Direction::RIGHT};

namespace {
class DebugServer : public ServerModel {
public:
    explicit DebugServer(int level = 1, int bots = 0, int bonuses = 0)
        : ServerModel(level, bots, bonuses) {
    }

    using GameModel::getHandler;
    using ServerModel::addEvent;
};

void setPosition(DebugServer &server,
                 ClientModel &clientModel,
                 int id,
                 int left,
                 int top) {
    server.addEvent(std::make_unique<SetPosition>(id, left, top));
}

tcp::socket newSocket(io_context &context) {
    return tcp::socket(context);
}

}  // namespace
TEST_CASE("Game creation") {
    Tanks::model::ServerModel model;
    model.loadLevel(1);
    auto &brick00 = model.getByCoords(TILE_SIZE, TILE_SIZE);

    CHECK(brick00.getType() == EntityType::FLOOR);
}

#define ADD_PLAYER(model, i)                                   \
    io_context context_client##i;                              \
    tcp::socket server##i(serverContext);                      \
    auto connection##i =                                       \
        std::thread([&]() { server##i = acceptor.accept(); }); \
                                                               \
    auto client##i = newSocket(context_client##i);             \
    client##i.connect(acceptor.local_endpoint());              \
    connection##i.join();                                      \
    auto id##i = serverModel.addPlayer(server##i);             \
    ClientModel clientModel##i(id##i, std::move(client##i));

#define GET_PLAYER_CONTORS(i)                                                \
    auto &tank##i = dynamic_cast<Tank &>(serverModel.getById(id##i)->get()); \
    auto &handler##i =                                                       \
        dynamic_cast<TankHandler &>(serverModel.getHandler(tank##i));

#define INIT_GAME()                                                     \
    DebugServer serverModel;                                            \
    io_context serverContext;                                           \
    tcp::acceptor acceptor(serverContext, tcp::endpoint(tcp::v4(), 0)); \
    ADD_PLAYER(serverModel, );                                          \
    serverModel.nextTick();                                             \
    GET_PLAYER_CONTORS()                                                \
    clientModel.loadLevel(1);                                           \
    clientModel.nextTick();

#define INIT_GAME_FULL(level, bots, bonuses)       \
    DebugServer serverModel(level, bots, bonuses); \
    io_context serverContext;                      \
    tcp::acceptor acceptor(serverContext, tcp::endpoint(tcp::v4(), 0));

bool operator==(const Entity &a, const Entity &b) {
    if (a.getId() != b.getId()) {
        CHECK(false);
        return false;
    }
    if (a.getType() != b.getType()) {
        CHECK(false);
        return false;
    }
    if (a.getTop() != b.getTop() || a.getLeft() != b.getLeft()) {
        CHECK(false);
        return false;
    }
    if (a.getHeight() != b.getHeight() || a.getWidth() != b.getWidth()) {
        CHECK(false);
        return false;
    }
    if (a.getStrength() != b.getStrength()) {
        CHECK(false);
        return false;
    }
    if (a.isTankPassable() != b.isTankPassable() ||
        a.isBulletPassable() != b.isBulletPassable()) {
        CHECK(false);
        return false;
    }
    return true;
}

[[nodiscard]] bool operator!=(const Entity &a, const Entity &b) {
    return !(a == b);
}

[[nodiscard]] inline static int differences(GameModel &serverModel,
                                            GameModel &clientModel) {
    if (serverModel.getTick() != clientModel.getTick()) {
        CHECK(serverModel.getTick() == clientModel.getTick());
        SIGTRAP;
        return false;
    }
    auto all1 = serverModel.getAll();
    auto all2 = clientModel.getAll();
    int res = 0;
    for (const auto &vec : all1) {
        for (const auto *ptr : vec) {
            if (ptr == nullptr) {
                CHECK(ptr != nullptr);
                res++;
                continue;
            }
            const auto &entity1 = *ptr;
            auto e2 = clientModel.getById(entity1.getId());
            if (!e2) {
                CHECK(e2);
                res++;
                continue;
            }
            auto &entity2 = clientModel.getById(entity1.getId())->get();
            if (entity1 != entity2) {
                CHECK(false);
                res++;
            }
        }
    }
    return res;
}

TEST_CASE("Network functions") {
    boost::asio::io_context server_context, client_context;
    tcp::acceptor acceptor(server_context, tcp::endpoint(tcp::v4(), 0));
    tcp::socket server(server_context), client(client_context);
    std::thread connect([&]() { server = acceptor.accept(); });
    client.connect(acceptor.local_endpoint());
    connect.join();
    constexpr int A_1 = 10;
    SUBCASE("send first") {
        sendInt(server, A_1);
        sendMultipleInts(client, 1, 2, 3, 4, 125);

        CHECK(receiveMultipleInts<int, int, int, int, int>(server) ==
              std::tuple{1, 2, 3, 4, 125});

        CHECK(receiveInt(client) == A_1);
    }

    SUBCASE("receive first") {
        std::thread send([&]() {
            //            std::this_thread::sleep_for(std::chrono::milliseconds(10));
            sendInt(server, A_1);
            sendMultipleInts(client, 1, 2, 3, 4, 125);
        });
        std::thread receive([&]() {
            CHECK(receiveMultipleInts<int, int, int, int, int>(server) ==
                  std::tuple{1, 2, 3, 4, 125});

            CHECK(receiveInt(client) == A_1);
        });
        receive.join();
        send.join();
    }
}

TEST_CASE("Single move. Online") {
    INIT_GAME();

    CHECK(differences(serverModel, clientModel) == 0);
    setPosition(serverModel, clientModel, id, TILE_SIZE, TILE_SIZE);
    serverModel.nextTick();
    clientModel.nextTick();
    CHECK(differences(serverModel, clientModel) == 0);
    //    handler.setPosition(TILE_SIZE * 2, TILE_SIZE);
    auto &brick00 = serverModel.getByCoords(TILE_SIZE, TILE_SIZE);
    //    handler.setPosition(TILE_SIZE, TILE_SIZE);

    CHECK(tank.getLeft() == TILE_SIZE);
    CHECK(tank.getTop() == TILE_SIZE);

    auto controller = clientModel.getHandler();

    controller.move(Direction::DOWN, tank.getSpeed());

    Tanks::model::Entity &ptr2 = serverModel.getByCoords(TILE_SIZE, TILE_SIZE);
    std::this_thread::sleep_for(std::chrono::milliseconds(5));
    serverModel.nextTick();
    clientModel.nextTick();

    CHECK(differences(serverModel, clientModel) == 0);
    CHECK(tank.getTop() == TILE_SIZE + tank.getSpeed());
}

TEST_CASE("multiple moves. Online") {
    INIT_GAME();
    CHECK(differences(serverModel, clientModel) == 0);
    setPosition(serverModel, clientModel, id, TILE_SIZE, TILE_SIZE);

    int speed = tank.getSpeed();

    // [0,48) : Tank
    // [0,56) : Block
    auto controller = clientModel.getHandler();
    const int MOVES = 9;
    for (int i = 0; i < MOVES; i++) {
        controller.move(Direction::DOWN, speed);
        serverModel.nextTick();
        clientModel.nextTick();
        CHECK(differences(serverModel, clientModel) == 0);
    }

    serverModel.nextTick();
    clientModel.nextTick();

    CHECK(differences(serverModel, clientModel) == 0);
}

TEST_CASE("Can't do 2 moves on 1 tick") {
    INIT_GAME();
    handler.setPosition(TILE_SIZE, TILE_SIZE);

    int left = tank.getLeft();
    int top = tank.getTop();
    handler.move(Direction::RIGHT);
    int left1 = tank.getLeft();
    int top1 = tank.getTop();
    handler.move(Direction::RIGHT);
    CHECK(left1 == tank.getLeft());
    CHECK(top1 == tank.getTop());
    serverModel.nextTick();
    CHECK(left1 == tank.getLeft());
    CHECK(top1 == tank.getTop());
    serverModel.finishGame();
}
TEST_CASE("Can move and shoot on 1 tick") {
    INIT_GAME();

    int left = tank.getLeft(), top = tank.getTop();
    handler.move(Direction::RIGHT);
    int left1 = tank.getLeft(), top1 = tank.getTop();
    handler.move(Direction::RIGHT);
    CHECK(left1 == left + tank.getSpeed());
    CHECK(top1 == top);
    handler.shoot();
    CHECK(left1 == left + tank.getSpeed());
    CHECK(top1 == top);
    serverModel.finishGame();
}

TEST_CASE("Can't shoot then move on 1 tick") {
    INIT_GAME();
    handler.setPosition(TILE_SIZE, TILE_SIZE);
    int left = tank.getLeft(), top = tank.getTop();
    handler.move(Direction::RIGHT, 0);
    CHECK(serverModel.getTick() == 1);
    handler.shoot();
    CHECK(serverModel.getTick() == 1);
    handler.move(Direction::RIGHT);
    CHECK(left == tank.getLeft());
    CHECK(top == tank.getTop());
    serverModel.nextTick();
    CHECK(serverModel.getTick() == 2);
    serverModel.finishGame();
    clientModel.nextTick();
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
    serverModel.finishGame();
}

TEST_CASE("Look for grass") {
    INIT_GAME();
    handler.setPosition(80, 80);

    for (auto dir : DIRECTIONS) {
        for (const auto *col : tank.look(dir)) {
            CHECK(col != nullptr);
            CHECK(col->getType() == Tanks::model::EntityType::FLOOR);
        }
    }
    serverModel.finishGame();
}

TEST_CASE("Tank simple shoot") {
    INIT_GAME();
    handler.setPosition(TILE_SIZE, TILE_SIZE);
    handler.move(Direction::RIGHT, 0);
    handler.shoot();
    auto &bullet = dynamic_cast<Projectile &>(
        const_cast<Entity &>(*serverModel.getAll(EntityType::BULLET)[0]));

    CHECK(bullet.getType() == Tanks::model::EntityType::BULLET);
    CHECK(bullet.getTop() == Tanks::TILE_SIZE + Tanks::TANK_SIZE / 2 - 12);
    CHECK(bullet.getLeft() == Tanks::TILE_SIZE + Tanks::TANK_SIZE + 4);
    CHECK(bullet.getDirection() == tank.getDirection());

    serverModel.nextTick();
    CHECK(bullet.getTop() == Tanks::TILE_SIZE + Tanks::TANK_SIZE / 2 - 12);
    CHECK(bullet.getLeft() == TILE_SIZE + TANK_SIZE + bullet.getSpeed() + 4);
    serverModel.finishGame();
}

TEST_CASE("Shoot, bullet die") {
    INIT_GAME();
    handler.setPosition(TILE_SIZE, TILE_SIZE);
    handler.setDirection(Direction::RIGHT);
    handler.shoot();
    auto &bullet = dynamic_cast<Projectile &>(
        serverModel.getByCoords(Tanks::TILE_SIZE + Tanks::TANK_SIZE + 4,
                                Tanks::TILE_SIZE + Tanks::TANK_SIZE / 2 - 12));

    CHECK(bullet.getType() == Tanks::model::EntityType::BULLET);
    CHECK(bullet.getTop() == Tanks::TILE_SIZE + Tanks::TANK_SIZE / 2 - 12);
    CHECK(bullet.getLeft() == Tanks::TILE_SIZE + Tanks::TANK_SIZE + 4);
    CHECK(bullet.getDirection() == tank.getDirection());

    serverModel.nextTick();
    CHECK(bullet.getTop() == Tanks::TILE_SIZE + Tanks::TANK_SIZE / 2 - 12);
    CHECK(bullet.getLeft() == TILE_SIZE + TANK_SIZE + bullet.getSpeed() + 4);
    for (int i = 0; i < 50; i++) {
        serverModel.nextTick();
    }
    serverModel.finishGame();
}

TEST_CASE("3 Bullets destroy 3 bricks") {
    INIT_GAME();
    handler.setPosition(TILE_SIZE, TILE_SIZE * 2);

    handler.setDirection(Direction::RIGHT);
    constexpr std::array<std::pair<int, int>, 3> COORDS = {
        std::pair{TILE_SIZE * 3, TILE_SIZE * 2},
        {TILE_SIZE * 7, TILE_SIZE * 2},
        {TILE_SIZE * 13, TILE_SIZE * 2}};
    for (auto [x, y] : COORDS) {
        handler.shoot();
        auto &bullet = dynamic_cast<Projectile &>(
            serverModel.getByCoords(tank.getLeft() + tank.getWidth() + 4,
                                    tank.getTop() + tank.getHeight() / 2 - 12));

        auto &brick = serverModel.getByCoords(x, y);
        int t = bullet.dist(brick) / DEFAULT_BULLET_SPEED +
                (bullet.dist(brick) % DEFAULT_BULLET_SPEED != 0);
        for (int i = 0; i < t; i++) {
            auto &brickNow = serverModel.getByCoords(x, y);
            CHECK(&brick == &brickNow);
            serverModel.nextTick();
        }
        auto &floor = serverModel.getByCoords(x, y);
        CHECK(floor.getType() == Tanks::model::EntityType::FLOOR);
        for (int i = 0; i < DEFAULT_RELOAD_TICKS; i++) {
            serverModel.nextTick();
        }
    }
    serverModel.finishGame();
}

TEST_CASE("Bullet fly above water") {
    INIT_GAME();
    handler.setPosition(TILE_SIZE * 4, TILE_SIZE * 15);

    CHECK(serverModel.getWidth() == MAP_WIDTH * TILE_SIZE);
    CHECK(serverModel.getHeight() == MAP_HEIGHT * TILE_SIZE);

    handler.setDirection(Direction::RIGHT);
    auto &brick = serverModel.getByCoords(TILE_SIZE * 19, TILE_SIZE * 15);
    CHECK(brick.getType() == EntityType::BRICK);
    auto &water = serverModel.getByCoords(TILE_SIZE * 15, TILE_SIZE * 15);
    CHECK(water.getType() == Tanks::model::EntityType::WATER);
    handler.shoot();
    auto &bullet = dynamic_cast<Projectile &>(
        serverModel.getByCoords(tank.getLeft() + tank.getWidth() + 4,
                                tank.getTop() + tank.getHeight() / 2 - 12));

    int tWater = bullet.dist(water) / DEFAULT_BULLET_SPEED +
                 (bullet.dist(water) % DEFAULT_BULLET_SPEED != 0);
    for (int i = 0; i < tWater; i++) {
        auto &waterNow = serverModel.getByCoords(
            TILE_SIZE * 15, TILE_SIZE * 15 + tank.getHeight() / 2);
        CHECK(&water == &waterNow);
        serverModel.nextTick();
    }

    int tBrick = bullet.dist(brick) / DEFAULT_BULLET_SPEED +
                 (bullet.dist(brick) % DEFAULT_BULLET_SPEED != 0);
    int brickId = brick.getId();
    for (int i = 0; i < tBrick; i++) {
        auto &brickNow =
            serverModel.getByCoords(TILE_SIZE * 19, TILE_SIZE * 15);
        CHECK(serverModel.getById(brickId));
        serverModel.nextTick();
    }

    auto &waterNow = serverModel.getByCoords(
        TILE_SIZE * 15 + 1, TILE_SIZE * 15 + tank.getHeight() / 2);
    CHECK(&waterNow == &water);

    auto &floor = serverModel.getByCoords(TILE_SIZE * 19, TILE_SIZE * 15);
    CHECK(floor.getType() == Tanks::model::EntityType::FLOOR);
    serverModel.finishGame();
}

TEST_CASE("Bullet destroy on creation") {
    INIT_GAME();
    constexpr int TANK_LEFT = TILE_SIZE * 3;
    constexpr int TANK_TOP = TILE_SIZE * 1 + TILE_SIZE - TANK_SIZE;
    handler.setPosition(TANK_LEFT, TANK_TOP);
    handler.setDirection(Direction::DOWN);

    auto &brick = serverModel.getByCoords(TILE_SIZE * 3, TILE_SIZE * 2);
    CHECK(brick.getType() == EntityType::BRICK);

    handler.shoot();

    auto &floor = serverModel.getByCoords(TILE_SIZE * 3, TILE_SIZE * 2);
    CHECK(floor.getType() == Tanks::model::EntityType::FLOOR);
    serverModel.finishGame();
}

#define INIT_GAME_TWO_PLAYERS() \
    INIT_GAME_FULL(1, 0, 0);    \
                                \
    ADD_PLAYER(serverModel, 2); \
    ADD_PLAYER(serverModel, );  \
    clientModel.loadLevel(1);   \
    serverModel.nextTick();     \
    clientModel.nextTick();     \
    GET_PLAYER_CONTORS();       \
    GET_PLAYER_CONTORS(2);

TEST_CASE("Tank move on ") {
    INIT_GAME_TWO_PLAYERS();
    handler.setPosition(TILE_SIZE, TILE_SIZE);
    handler2.setPosition(TILE_SIZE + TANK_SIZE, TILE_SIZE);
    CHECK(serverModel.getByCoords(TILE_SIZE, TILE_SIZE).getType() ==
          EntityType::MEDIUM_TANK);
    handler.move(Tanks::model::Direction::RIGHT);
    auto &nowTank = serverModel.getByCoords(TILE_SIZE, TILE_SIZE);
    CHECK(dynamic_cast<Tank *>(&nowTank));
    auto &ttank = dynamic_cast<Tank &>(nowTank);
    CHECK(ttank.getLeft() == TILE_SIZE);
    CHECK(ttank.getTop() == TILE_SIZE);
    serverModel.finishGame();
}

TEST_CASE("Shoot static tank") {
    INIT_GAME_TWO_PLAYERS();

    int ok = differences(serverModel, clientModel);
    CHECK(ok == 0);
    handler.setPosition(TILE_SIZE * 2, TILE_SIZE);
    auto &tank1 = tank;
    handler.setPosition(TILE_SIZE * 2, TILE_SIZE);
    handler2.setPosition(TILE_SIZE * 3, TILE_SIZE);

    handler.setDirection(Direction::RIGHT);
    handler.shoot();
    serverModel.nextTick();
    auto &floor = serverModel.getByCoords(TILE_SIZE * 3, TILE_SIZE);
    CHECK(floor.getType() == EntityType::FLOOR);
    serverModel.finishGame();
}

TEST_CASE("TankMoveOnBullet") {
    INIT_GAME_TWO_PLAYERS();

    handler.setPosition(TILE_SIZE * 2 + BULLET_SIZE, TILE_SIZE);
    handler2.setPosition(TILE_SIZE, TILE_SIZE);

    handler.setDirection(Direction::LEFT);
    handler.shoot();
    handler2.move(Direction::RIGHT);
    auto &floor = serverModel.getByCoords(TILE_SIZE, TILE_SIZE);
    CHECK(floor.getType() == EntityType::FLOOR);
    serverModel.finishGame();
}

TEST_CASE("BulletDestroy2BlocksOnCreation") {
    INIT_GAME();
    handler.setPosition(TILE_SIZE * 10 + TILE_SIZE / 2, TILE_SIZE * 15);

    handler.setDirection(Direction::DOWN);
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
    serverModel.finishGame();
}

TEST_CASE("Bullet destroy 2 blocks on nextTick") {
    INIT_GAME();

    handler.setPosition(TILE_SIZE * 10 + TILE_SIZE / 2,
                        TILE_SIZE * 15 - BULLET_SIZE - 1);
    auto &brick1 = serverModel.getByCoords(TILE_SIZE * 10, TILE_SIZE * 16);
    auto &brick2 = serverModel.getByCoords(TILE_SIZE * 11, TILE_SIZE * 16);
    CHECK(brick1.getType() == EntityType::BRICK);
    CHECK(brick2.getType() == EntityType::BRICK);

    handler.setDirection(Direction::DOWN);
    handler.shoot();

    serverModel.nextTick();

    auto &floor1 = serverModel.getByCoords(TILE_SIZE * 10, TILE_SIZE * 16);
    auto &floor2 = serverModel.getByCoords(TILE_SIZE * 11, TILE_SIZE * 16);
    CHECK(floor1.getType() == EntityType::FLOOR);
    CHECK(floor2.getType() == EntityType::FLOOR);
    serverModel.finishGame();
}

TEST_CASE("Bullet destroy 1 of blocks on creation, then shoot again") {
    INIT_GAME();
    handler.setPosition(TILE_SIZE * 10 + TILE_SIZE - TANK_SIZE, TILE_SIZE * 15);
    handler.setDirection(Direction::DOWN);

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
    serverModel.finishGame();
}

TEST_CASE("Respawn") {
    INIT_GAME_TWO_PLAYERS();

    handler.setPosition(TILE_SIZE * 2, TILE_SIZE);
    auto &tank1 = tank;
    handler.setPosition(TILE_SIZE * 2, TILE_SIZE);
    handler2.setPosition(TILE_SIZE * 3, TILE_SIZE);

    handler.setDirection(Direction::RIGHT);
    handler.shoot();
    serverModel.nextTick();
    auto &floor = serverModel.getByCoords(TILE_SIZE * 3, TILE_SIZE);
    CHECK(floor.getType() == EntityType::FLOOR);
    for (int i = 0; i < DEFAULT_RESPAWN_TIME; i++) {
        auto tank2 = serverModel.getById(id2);
        CHECK(tank2 == std::nullopt);
        serverModel.nextTick();
    }
    auto tank2New = serverModel.getById(id2);
    CHECK(tank2New != std::nullopt);
    serverModel.finishGame();
}

TEST_CASE("Online shoot") {
    INIT_GAME();
    setPosition(serverModel, clientModel, tank.getId(), TILE_SIZE * 2,
                TILE_SIZE * 2);

    serverModel.nextTick();
    clientModel.nextTick();

    auto user = clientModel.getHandler();
    user.shoot(Direction::RIGHT);
    std::this_thread::sleep_for(std::chrono::milliseconds(1));
    serverModel.nextTick();
    clientModel.nextTick();

    CHECK(serverModel.getByCoords(TILE_SIZE * 3, TILE_SIZE * 2).getType() ==
          EntityType::FLOOR);
    CHECK(clientModel.getByCoords(TILE_SIZE * 3, TILE_SIZE * 2).getType() ==
          EntityType::FLOOR);
    serverModel.finishGame();
}

TEST_CASE("Online. Bullet destroy on creation") {
    INIT_GAME();
    CHECK(differences(serverModel, clientModel) == 0);
    constexpr int TANK_LEFT = TILE_SIZE * 3;
    constexpr int TANK_TOP = TILE_SIZE * 1 + TILE_SIZE - TANK_SIZE;
    setPosition(serverModel, clientModel, id, TANK_LEFT, TANK_TOP);

    serverModel.nextTick();
    clientModel.nextTick();

    auto &brick = serverModel.getByCoords(TILE_SIZE * 3, TILE_SIZE * 2);
    CHECK(brick.getType() == EntityType::BRICK);
    CHECK(differences(serverModel, clientModel) == 0);

    clientModel.getHandler().shoot(Direction::DOWN);
    std::this_thread::sleep_for(std::chrono::milliseconds(1));

    serverModel.nextTick();
    clientModel.nextTick();

    CHECK(clientModel.wasShootThisTurn());
    CHECK(serverModel.wasShootThisTurn());

    auto &floor = serverModel.getByCoords(TILE_SIZE * 3, TILE_SIZE * 2);
    CHECK(floor.getType() == Tanks::model::EntityType::FLOOR);
    CHECK(differences(serverModel, clientModel) == 0);
    CHECK(serverModel.wasDestroyedBlockThisTurn());
    CHECK(clientModel.wasDestroyedBlockThisTurn());
    serverModel.finishGame();
}

TEST_CASE("Bonus") {
    INIT_GAME_FULL(1, 0, 1);
    ADD_PLAYER(serverModel, );
    serverModel.nextTick();
    GET_PLAYER_CONTORS();
    auto user = clientModel.getHandler();
    constexpr int LEFT = 10 * TILE_SIZE - TANK_SIZE;
    constexpr int TOP = 8 * TILE_SIZE;
    auto &entity = serverModel.getByCoords(LEFT, TOP);
    handler.setPosition(LEFT, TOP);
    int bonusId =
        serverModel.getAll(EntityType::WALK_ON_WATER_BONUS)[0]->getId();
    dynamic_cast<ForegroundHandler &>(
        serverModel.getHandler(serverModel.getById(bonusId)->get()))
        .setPosition(LEFT + TANK_SIZE, TOP);
    //    setPosition(serverModel, clientModel, id, LEFT, TOP);
    //    setPosition(serverModel, clientModel, bonusId, LEFT + TANK_SIZE, TOP);
    serverModel.nextTick();
    //    clientModel.nextTick();
    decltype(auto) safeHandler = [&]() -> TankHandler & {
        auto e = serverModel.getById(id);
        auto &h = serverModel.getHandler(e->get());
        return dynamic_cast<TankHandler &>(h);
    };

    for (int i = 0; i <= 20; i++) {
        safeHandler().move(Direction::RIGHT);
        //        user.move(Direction::RIGHT);
        //        std::this_thread::sleep_for(std::chrono::milliseconds(10));
        serverModel.nextTick();
        //        clientModel.nextTick();
        CHECK(tank.getLeft() == LEFT + (i + 1) * tank.getSpeed());
    }

    serverModel.finishGame();
}

TEST_CASE("Bots, bonuses stress") {
    INIT_GAME_FULL(1, 40, 10);
    ADD_PLAYER(serverModel, );
    clientModel.loadLevel(1);
    serverModel.nextTick();
    clientModel.nextTick();
    CHECK(serverModel.getTick() == clientModel.getTick());
    CHECK(differences(serverModel, clientModel) == 0);

    int bonuseHere = 0;
#ifdef MODEL_BIG_TESTS
    constexpr int errorTick = 65;
    constexpr int TICKS = 2e3;
#else
    constexpr int errorTick = 0;
    constexpr int TICKS = 2e2;

    for (int i = 0; i < errorTick; i++) {
        serverModel.nextTick();
        clientModel.nextTick();
        CHECK(differences(serverModel, clientModel) == 0);
    }
    for (int i = errorTick; i < TICKS; i++) {
        serverModel.nextTick();
        clientModel.nextTick();
        CHECK(differences(serverModel, clientModel) == 0);
        //        std::cout << serverModel.getTick() << std::endl;
        if (!serverModel.getAll(EntityType::WALK_ON_WATER_BONUS).empty()) {
            bonuseHere++;
        }
    }
    CHECK(bonuseHere != 0);
    serverModel.finishGame();
}

TEST_CASE("BIG Bots, bonuses stress") {
    INIT_GAME_FULL(1, 100, 100);
    ADD_PLAYER(serverModel, );
    serverModel.nextTick();

    int bonuseHere = 0;
#ifdef MODEL_BIG_TESTS
    constexpr int TICKS = 1e4;
#else
    constexpr int TICKS = 5e2;
#endif

    // 1266
    for (int i = 0; i < TICKS; i++) {
        serverModel.nextTick();
    }
    serverModel.nextTick();
    serverModel.finishGame();
}

void userReceiver(tcp::acceptor &acceptor, ServerModel &serverModel) {
    std::vector<std::unique_ptr<tcp::socket> > serverSockets;
    serverSockets.reserve(100);
    while (true) {
        try {
            tcp::socket server = acceptor.accept();
            serverSockets.emplace_back(
                std::make_unique<tcp::socket>(std::move(server)));
            int id = serverModel.addPlayer(*serverSockets.back(), {});
            sendInt(*serverSockets.back(), id);
        } catch (boost::system::system_error &e) {
            return;
        }
    }
};

void userMover(tcp::acceptor &acceptor,
               std::mutex &vec_mutex,
               std::vector<std::reference_wrapper<ClientModel> > &vec,
               std::mt19937 &rnd) {
    boost::asio::io_context context;
    tcp::socket client(context);
    client.connect(acceptor.local_endpoint());
    int id = receiveInt(client);
    ClientModel clientModel(id, std::move(client));
    clientModel.loadLevel(1);
    clientModel.nextTick();
    std::unique_lock lock(vec_mutex);
    vec.push_back(std::ref(clientModel));
    lock.unlock();
    try {
        while (true) {
            auto tank = clientModel.tank();
            if (!tank) {
                clientModel.nextTick();
                continue;
            }
            int todo = (std::abs(static_cast<int>(rnd()))) % 12;
            if (todo >= 12) {
                clientModel.getHandler().shoot(tank->get().getDirection());
            } else {
                auto directon = static_cast<Direction>(todo % 4);
                clientModel.getHandler().move(
                    directon, clientModel.tank()->get().getSpeed());
            }
            clientModel.nextTick();
        }
    } catch (boost::system::system_error &e) {
        CHECK(false);
        std::cerr << e.what() << std::endl;
    }
}

TEST_CASE("10 users 30 bots 10 bonuses, check correction") {
    INIT_GAME_FULL(1, 30, 10);
    constexpr int CLIENTS = 10;
#ifdef MODEL_BIG_TESTS
    constexpr int TICKS = 1e3;
#else
    constexpr int TICKS = 1e2;
#endif

    //    std::mt19937 rnd(
    //        std::chrono::steady_clock::now().time_since_epoch().count());
    std::mt19937 rnd{0};
    std::thread([&]() { userReceiver(acceptor, serverModel); }).detach();
    std::mutex vec_mutex;
    std::vector<std::reference_wrapper<ClientModel> > vec;
    vec.reserve(CLIENTS);
    for (int i = 0; i < CLIENTS; i++) {
        std::thread([&]() {
            userMover(acceptor, vec_mutex, vec, rnd);
        }).detach();
    }
    std::this_thread::sleep_for(std::chrono::seconds(5));
    int sleeps = 0;
    for (int i = 0; i < TICKS; i++) {
        try {
            serverModel.nextTick();
        } catch (boost::system::system_error &e) {
            std::cerr << e.what() << std::endl;
        }
        for (auto &mod : vec) {
            while (mod.get().getTick() != serverModel.getTick()) {
                std::this_thread::sleep_for(std::chrono::milliseconds(15));
                sleeps++;
                //                CHECK(cnt < 10);
            }
            CHECK(differences(serverModel, mod) == 0);
        }
    }
    serverModel.finishGame();
    std::cout << "sleeps: " << sleeps;
}

TEST_CASE("Real") {
    INIT_GAME_FULL(1, 4, 2);
    constexpr int CLIENTS = 4;
#ifdef MODEL_BIG_TESTS
    constexpr int TICKS = 1e3;
#else
    constexpr int TICKS = 1e2;
#endif

    //    std::mt19937 rnd(
    //        std::chrono::steady_clock::now().time_since_epoch().count());
    std::mt19937 rnd{0};
    std::thread([&]() { userReceiver(acceptor, serverModel); }).detach();
    std::mutex vec_mutex;
    std::vector<std::reference_wrapper<ClientModel> > vec;
    vec.reserve(CLIENTS);
    for (int i = 0; i < CLIENTS; i++) {
        std::thread([&]() {
            userMover(acceptor, vec_mutex, vec, rnd);
        }).detach();
    }
    std::this_thread::sleep_for(std::chrono::seconds(5));
    int sleeps = 0;
    for (int i = 0; i < TICKS; i++) {
        try {
            serverModel.nextTick();
        } catch (boost::system::system_error &e) {
            std::cerr << e.what() << std::endl;
        }

        for (auto &mod : vec) {
            while (mod.get().getTick() != serverModel.getTick()) {
                std::this_thread::sleep_for(std::chrono::milliseconds(1));
                sleeps++;
                //                CHECK(cnt < 10);
            }
            CHECK(differences(serverModel, mod) == 0);
        }
    }
    serverModel.finishGame();
    std::cout << "sleeps: " << sleeps;
}

TEST_CASE("4 users 4 bots 2 bonuses, don't check correction") {
    INIT_GAME_FULL(1, 4, 2);
    constexpr int CLIENTS = 4;
    constexpr int TICKS = 1e2;

    //    std::mt19937 rnd(
    //        std::chrono::steady_clock::now().time_since_epoch().count());
    std::mt19937 rnd{0};
    std::thread([&]() { userReceiver(acceptor, serverModel); }).detach();
    std::mutex vec_mutex;
    std::vector<std::reference_wrapper<ClientModel> > vec;
    static auto timeNow = []() {
        return std::chrono::milliseconds(
            std::chrono::steady_clock::now().time_since_epoch().count());
    };

    vec.reserve(CLIENTS);
    for (int i = 0; i < CLIENTS; i++) {
        std::thread([&]() {
            userMover(acceptor, vec_mutex, vec, rnd);
        }).detach();
    }

    std::this_thread::sleep_for(std::chrono::seconds(5));
    int sleeps = 0;
    for (int i = 0; i < TICKS; i++) {
        try {
            serverModel.nextTick();
            std::this_thread::sleep_for(std::chrono::milliseconds(15));
        } catch (boost::system::system_error &e) {
            std::cerr << e.what() << std::endl;
        }
    }
    for (auto &mod : vec) {
        auto tick = mod.get().getTick();
        CHECK(tick >= TICKS / 2);
        std::cout << mod.get().getTick() << ' ';
        std::cout.flush();
    }
    std::cout << std::endl;
    serverModel.finishGame();
    std::cout << "sleeps: " << sleeps << std::endl;
}

TEST_CASE("4 users 4 bots 2 bonuses, don't check correction") {
    INIT_GAME_FULL(1, 4, 2);
    constexpr int CLIENTS = 5;
#ifdef MODEL_BIG_TESTS
    constexpr int TICKS = 1e3;
#else
    constexpr int TICKS = 1e2;
#endif

    //    std::mt19937 rnd(
    //        std::chrono::steady_clock::now().time_since_epoch().count());
    std::mt19937 rnd{0};
    std::thread([&]() { userReceiver(acceptor, serverModel); }).detach();
    std::mutex vec_mutex;
    std::vector<std::reference_wrapper<ClientModel> > vec;
    static auto timeNow = []() {
        return std::chrono::milliseconds(
            std::chrono::steady_clock::now().time_since_epoch().count());
    };

    vec.reserve(CLIENTS);
    for (int i = 0; i < CLIENTS; i++) {
        std::thread([&]() {
            userMover(acceptor, vec_mutex, vec, rnd);
        }).detach();
    }

    std::this_thread::sleep_for(std::chrono::seconds(5));
    int sleeps = 0;
    for (int i = 0; i < TICKS; i++) {
        try {
            serverModel.nextTick();
            std::this_thread::sleep_for(std::chrono::milliseconds(15));
        } catch (boost::system::system_error &e) {
            std::cerr << e.what() << std::endl;
        }
    }
    for (auto &mod : vec) {
        auto tick = mod.get().getTick();
        CHECK(tick >= TICKS / 2);
        std::cout << mod.get().getTick() << ' ';
        std::cout.flush();
    }
    std::cout << std::endl;
    serverModel.finishGame();
    std::cout << "sleeps: " << sleeps << std::endl;
}

#endif  // MODER_RUN_BIG_TESTS