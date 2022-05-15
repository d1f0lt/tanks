#ifndef TANKS_GAME_MODEL_H
#define TANKS_GAME_MODEL_H

#include <boost/asio.hpp>
#include <optional>
#include <queue>
#include <unordered_map>
#include "entity_holder.h"
#include "model/event.h"
#include "model/game_map.h"
#include "model/grouped_entities.h"
#include "model/handler.h"
#include "model/playable-tank.h"

namespace Tanks::model {
class GameModel {
    friend BasicHandler;
    friend ForegroundHandler;
    friend MovableHandler;
    friend TankHandler;
    friend ProjectileHandler;

public:
    explicit GameModel() = default;

    [[nodiscard]] Entity &getByCoords(int col, int row);
    [[nodiscard]] std::optional<std::reference_wrapper<Entity>> getById(
        int entityId);

    virtual void nextTick();

    int addPlayer(boost::asio::ip::tcp::iostream &ios);

    [[nodiscard]] PlayableTank &spawnPlayableTank(int left,
                                                  int top,
                                                  std::ostream &os);

    void loadLevel(int level);

    [[nodiscard]] int getHeight() const;
    [[nodiscard]] int getWidth() const;

    [[nodiscard]] std::vector<const Entity *> getAll(EntityType type);
    [[nodiscard]] std::vector<std::vector<const Entity *>> getAll();

    [[nodiscard]] int getTick() const;

private:
    [[nodiscard]] PlayableTank &spawnPlayableTank(int left,
                                                  int top,
                                                  int id,
                                                  std::ostream &os);

    void addEntity(std::unique_ptr<Entity> entity);
    void removeEntity(Entity &entity);

    [[nodiscard]] int getCurrentId() {
        return currentId_;
    }

    void listen(boost::asio::ip::tcp::iostream &client);

    GameMap map_;
    GroupedEntities groupedEntities_;
    EntityHolder entityHolder_;
    std::unordered_map<int, Entity *> byId_;
    std::unordered_map<Entity *, BasicHandler *> handlers_;
    int currentTick_ = 0;
    std::queue<std::unique_ptr<Event>> events_;
    int currentId_ = 0;
};
}  // namespace Tanks::model

#endif  // TANKS_GAME_MODEL_H
