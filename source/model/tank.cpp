#include "model/tank.h"
#include <boost/asio/basic_stream_socket.hpp>
#include <boost/asio/ip/tcp.hpp>
#include <queue>
#include "constants.h"
#include "model/playable-tank.h"

namespace Tanks::model {
Tank::Tank(int left,
           int top,
           int entityId,
           std::unique_ptr<TankHandler> handler,
           Direction direction,
           int speed)
    : MovableEntity(left, top, entityId, std::move(handler), direction),
      speed_(speed) {
}

int Tank::getWidth() const {
    return TANK_SIZE;
}

int Tank::getHeight() const {
    return TANK_SIZE;
}

int Tank::getStrength() const {
    return 1;
}

EntityType Tank::getType() const {
    return EntityType::LEFT_DOWN_CORNER;
}

int Tank::getSpeed() const {
    return speed_;
}

bool Tank::canPass(const Entity &other) const {
    return other.isTankPassable();
}

void Tank::shoot() {
    dynamic_cast<TankHandler &>(getHandler()).shoot();
}

PlayableTank::PlayableTank(int left,
                           int top,
                           int entityId,
                           Direction direction,
                           boost::asio::ip::tcp::socket &is,
                           GameModel &model)
    : Tank(left,
           top,
           entityId,
           std::make_unique<PlayableTankHandler>(model, *this, is),
           direction,
           DEFAULT_TANK_SPEED) {
}

EntityType PlayableTank::getType() const {
    return EntityType::PLAYABLE_TANK;
}

PlayableTank::PlayableTank(int left,
                           int top,
                           int entityId,
                           std::unique_ptr<PlayableTankHandler> handler,
                           Direction direction)
    : Tank(left,
           top,
           entityId,
           std::move(handler),
           direction,
           DEFAULT_TANK_SPEED) {
}
void PlayableTank::move(Direction direction) {
    move(direction, getSpeed());
}

void PlayableTank::move(Direction direction, int speed) {
    dynamic_cast<PlayableTankHandler &>(getHandler()).move(direction, speed);
}

}  // namespace Tanks::model
