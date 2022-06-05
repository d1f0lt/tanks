#include <cassert>
#include <chrono>
#include <iostream>
#include <thread>
#include "game_controller.h"
#include "game_environment.h"
#include "menu/menu_controller.h"
#include "model/client_game_model.h"
#include "model/network_utils.h"
#include "model/player_action_handler.h"
#include "model/server_game_model.h"
#include "pause.h"
#include "player_skills.h"
#include "server.h"
#include "view/bullets_view.h"
#include "view/game_view.h"
#include "view/tank_view.h"

namespace Tanks {
using boost::asio::ip::tcp;
using Menu::PlayerSkills;

// constexpr int PLAYERS = 1;
constexpr int BOTS = 10;
constexpr int BONUSES = 10;

namespace {
void makeAction(model::PlayerActionsHandler &player) {
    if (player.tank()) {
        GameController::makeShot(player);
        GameController::makeMove(player);
    }
}

void sendSkillsTo(tcp::socket &socket, const PlayerSkills &skills) {
    model::sendMultipleInts(socket, skills.tankSpeed, skills.bulletSpeed,
                            skills.reloadTicks);
}

struct ServerHolder {
public:
    explicit ServerHolder(std::thread &&thread,
                          std::unique_ptr<Server> server,
                          std::condition_variable *serverStart)
        : thread_(std::move(thread)),
          server_(std::move(server)),
          serverStart_(serverStart) {
        assert(server_ != nullptr);
        assert(serverStart_ != nullptr);
    }

    ServerHolder(ServerHolder &&other) noexcept
        : thread_(std::move(other.thread_)),
          server_(std::move(other.server_)),
          serverStart_(other.serverStart_) {
    }

    [[nodiscard]] Server &getServer() const {
        return *server_;
    }

    void startServer() const {
        server_->start();
        serverStart_->notify_all();
    }

    ~ServerHolder() {
        if (!server_->getIsStarted()) {
            server_->stop();
            server_->start();
            serverStart_->notify_all();
        }
        server_->stop();
        //        client_.finishGame();
        thread_.join();
    }

private:
    std::thread thread_;
    std::unique_ptr<Server> server_ = nullptr;
    std::condition_variable *serverStart_ = nullptr;
};

void serverImp(const std::string &filename,
               int players,
               int bots,
               int bonuses,
               int level,
               std::condition_variable *&condvarOnClient,
               std::atomic<bool> &isServerCreated,
               std::unique_ptr<Server> &serverOnClient,
               std::condition_variable &serverOnClientInitiallized) {
    std::condition_variable cv;
    condvarOnClient = &cv;
    auto serverTmp = std::make_unique<Server>(filename, bots, bonuses, level);
    serverOnClient = std::move(serverTmp);
    Server *server = serverOnClient.get();
    isServerCreated = true;
    serverOnClientInitiallized.notify_all();
    for (int i = 0; i < players; i++) {
        server->listenForNewPlayer();
    }
    std::mutex mutex;
    std::unique_lock lock(mutex);
    cv.wait(lock, [&]() { return server->getIsStarted(); });
    while (!server->getIsStopped()) {
        server->nextTick();
        std::this_thread::sleep_for(std::chrono::milliseconds(15));
    }
    // init servermodel
}

std::unique_ptr<ServerHolder> createServer(const std::string levelFilename,
                                           int players,
                                           int level) {
    std::condition_variable *startServer = nullptr;
    std::unique_ptr<Server> serverPtr = nullptr;
    std::condition_variable serverCreatedCv;

    std::atomic<bool> isServerCreated = false;

    auto serverThread = std::thread([&]() {
        serverImp(levelFilename, players, BOTS, BONUSES, level, startServer,
                  isServerCreated, serverPtr, serverCreatedCv);
    });
    std::mutex mutex;
    std::unique_lock lock(mutex);
    serverCreatedCv.wait(lock, [&]() -> bool { return isServerCreated; });

    assert(startServer != nullptr);
    return std::make_unique<ServerHolder>(std::move(serverThread),
                                          std::move(serverPtr), startServer);
}

void drawTank(sf::RenderWindow &window, View::TankSpriteHolder &tankView, const model::Entity* entity) {
    const auto &tank = dynamic_cast<const model::Tank &>(*entity);
    tankView.draw(window, tank);
}

}  // namespace

std::optional<Menu::ButtonType>
startGame(  // NOLINT(readability-function-cognitive-complexity)
    sf::RenderWindow &window,
    int level,
    PlayerSkills skills,
    std::optional<std::pair<std::string, std::string>> addressPort,
    int players) {
    const bool isHost = (addressPort == std::nullopt);
    //    addressPort = {"127.0.0.1", "12345"};

    static const std::string imagesPath = "../images/";

    //    static_assert(std::is_move_constructible_v<Server>);
    boost::asio::io_context ioContext;
    boost::asio::ip::tcp::resolver resolver(ioContext);
    tcp::socket clientSocket(ioContext);
    clientSocket.open(tcp::v4());
    clientSocket.set_option(tcp::no_delay(true));
    tcp::endpoint endpoint;

    std::unique_ptr<ServerHolder> serverHolder = nullptr;
    if (isHost) {
        const std::string levelFilename("../levels/level" +
                                        std::to_string(level) + ".csv");
        assert(level >= 1);
        serverHolder = createServer(levelFilename, players, level);
        endpoint = serverHolder->getServer().getEndpoint();
        clientSocket.connect(endpoint);
    } else {
        boost::asio::connect(clientSocket, tcp::resolver(ioContext).resolve(
                                               addressPort.value().first,
                                               addressPort.value().second));
    }
    sendSkillsTo(clientSocket, skills);
    model::sendInt(clientSocket, skills.lifeAmount);

    int playerId = model::receiveInt(clientSocket);
    assert(playerId < 0);

    level = model::receiveInt(clientSocket);

    const std::string levelFilename("../levels/level" + std::to_string(level) +
                                    ".csv");

    model::ClientModel model(playerId, static_cast<int>(skills.lifeAmount),
                             std::move(clientSocket));
    model.loadLevel(levelFilename);

    View::TankSpriteHolder greenTankView(imagesPath + "tanks/green_tank.png");
    View::TankSpriteHolder redTankView(imagesPath + "tanks/red_tank.png");
    View::TankSpriteHolder blueTankView(imagesPath + "tanks/blue_tank.png");

    View::BulletsSpriteHolder bulletsView(imagesPath + "bullet.png");

    View::Map mapView(imagesPath + "map.png", level);

    Environment environment(imagesPath + "environment/", skills.lifeAmount);

    Pause pause;

    const std::unordered_set<int> playerIds = [&]() -> std::unordered_set<int> {
        std::unordered_set<int> res;
        for (int i = 0; i < players; i++) {
            res.insert(-BONUSES - BOTS - 1 - i);
        }
        return res;
    }();

    //    auto serverThread = server->start();
    if (isHost) {
        std::cout << endpoint << '\n';
        serverHolder->startServer();
    }

    model.nextTick();

    std::cout << "Connected" << std::endl;

    //    auto &model = serverPtr.model();
    while (window.isOpen()) {
        // catch event
        sf::Event event{};
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                window.close();
            }
        }
        pause.checkPause(event);

        auto player = model.getHandler();

        if (!pause.isPause()) {
            if (auto signal = MenuController::control(environment.getMenu(),
                                                      window, event);
                signal != std::nullopt) {
                assert(signal.value()->getType() == Menu::ButtonType::PAUSE);
                pause.makePause();
            } else {
                model.nextTick();
                makeAction(player);
                while (model.getLives() < static_cast<int>(environment.getLivesAmount())) {
                    environment.destroyLastHeart();
                }
            }
        } else {
            if (auto signal =
                    MenuController::control(pause.getMenu(), window, event);
                signal != std::nullopt) {
                switch (signal.value()->getType()) {
                    case Menu::ButtonType::RESUME:
                        pause.unpause();
                        break;
                    case Menu::ButtonType::NEW_GAME:
                        return Menu::ButtonType::NEW_GAME;
                    case Menu::ButtonType::EXIT:
                        return signal.value()->getType();
                    default:
                        break;
                }
            }
        }

        // redraw
        window.clear();
        environment.draw(window, pause.isPause());

        mapView.draw(window, model);

        drawTank(window, greenTankView, &model.getById(playerId).value().get());

        const auto &tanks = model.getAll(model::EntityType::MEDIUM_TANK);
        for (const auto *entity : tanks) {
            if (playerIds.find(entity->getId()) == playerIds.end()) {
                drawTank(window, redTankView, entity);
            } else if (entity->getId() != playerId) {
                drawTank(window, blueTankView, entity);
            }
        }

        const auto &bullets = model.getAll(model::EntityType::BULLET);
        bulletsView.draw(window, bullets);

        if (pause.isPause()) {
            pause.drawPause(window);
        }

        window.display();

        // Hack for better performance
        std::this_thread::sleep_for(std::chrono::milliseconds(15));
    }
    return std::nullopt;
}

}  // namespace Tanks
