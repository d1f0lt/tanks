#include <cassert>
#include <chrono>
#include <thread>
#include "game_controller.h"
#include "game_environment.h"
#include "menu/menu_controller.h"
#include "model/client_game_model.h"
#include "model/network_utils.h"
#include "model/player_action_handler.h"
#include "model/server_game_model.h"
#include "pause.h"
#include "server.h"
#include "view/bullets_view.h"
#include "view/game_view.h"
#include "view/tank_view.h"

namespace Tanks {
using boost::asio::ip::tcp;

namespace {
void makeAction(model::PlayerActionsHandler &player) {
    if (player.tank()) {
        GameController::makeShot(player);
        GameController::makeMove(player);
    }
}
struct ServerThreadJoiner {
public:
    explicit ServerThreadJoiner(std::thread &&thread, Server &server)
        : thread_(std::move(thread)), server_(server) {
    }

    [[nodiscard]] Server &getServer() const {
        return server_;
    }

    ~ServerThreadJoiner() {
        server_.stop();
        //        client_.finishGame();
        thread_.detach();
    }

private:
    std::thread thread_;
    Server &server_;
};

void server(const std::string &filename,
            int players,
            int bots,
            int bonuses,
            std::condition_variable *&condvarOnClient,
            std::atomic<Server *> &serverOnClient,
            std::condition_variable &serverOnClientInitiallized) {
    std::condition_variable cv;
    condvarOnClient = &cv;
    Server server(filename, bots, bonuses);
    serverOnClient = &server;
    serverOnClientInitiallized.notify_all();
    for (int i = 0; i < players; i++) {
        server.listenForNewPlayer();
    }
    std::mutex mutex;
    std::unique_lock lock(mutex);
    cv.wait(lock);
    while (!server.getIsStopped()) {
        server.nextTick();
        std::this_thread::sleep_for(std::chrono::milliseconds(15));
    }
    // init servermodel
}
}  // namespace

std::optional<Menu::ButtonType>
startGame(  // NOLINT(readability-function-cognitive-complexity)
    sf::RenderWindow &window,
    int level) {
    static const std::string imagesPath = "../images/";
    const std::string levelFilename("../levels/level" + std::to_string(level) +
                                    ".csv");

    static_assert(std::is_move_constructible_v<Server>);
    constexpr int BOTS = 10;
    constexpr int BONUSES = 10;
    //    auto server = std::make_unique<Server>(levelFilename, BOTS, BONUSES);

    std::condition_variable *startServer = nullptr;
    std::atomic<Server *> serverPtr = nullptr;
    std::condition_variable serverCreated;

    auto serverThread = std::thread([&]() {
        server(levelFilename, 1, BOTS, BONUSES, startServer, serverPtr,
               serverCreated);
    });
    std::mutex mutex;
    std::unique_lock lock(mutex);
    serverCreated.wait(lock, [&]() { return serverPtr != nullptr; });

    assert(startServer != nullptr);
    boost::asio::io_context ioContext;
    tcp::socket clientSocket(ioContext);
    Server *server = serverPtr;
    auto endpoint = server->getEndpoint();
    clientSocket.connect(endpoint);
    int playerId = model::receiveInt(clientSocket);
    assert(playerId < 0);
    ServerThreadJoiner joiner(std::move(serverThread), *server);

    //    auto modelPtr =
    //        std::make_unique<model::ClientModel>(playerId,
    //        std::move(clientSocket));
    //    modelPtr->loadLevel(levelFilename);
    model::ClientModel model(playerId, std::move(clientSocket));
    model.loadLevel(levelFilename);

    View::TankSpriteHolder greenTankView(imagesPath + "tanks/green_tank.png");

    View::BulletsSpriteHolder bulletsView(imagesPath + "bullet.png");

    View::Map mapView(imagesPath + "map.png", level);

    Environment environment(imagesPath + "environment/");

    Pause pause;

    //    auto serverThread = server->start();
    startServer->notify_all();

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

        const auto &tanks = model.getAll(model::EntityType::MEDIUM_TANK);
        for (const auto *tank : tanks) {
            greenTankView.draw(window,
                               dynamic_cast<const model::Tank &>(*tank));
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
