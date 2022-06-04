#include "model/event.h"
#include <unordered_map>
#include "model/network_utils.h"
#include "player_skills.h"

namespace Tanks::model {
std::unique_ptr<Event> TankMove::readFrom(
    boost::asio::ip::tcp::socket &socket) {
    auto [tankId, direction, speed] =
        receiveMultipleInts<int, Direction, int>(socket);

    return std::make_unique<TankMove>(tankId, direction, speed);
}

TankMove::TankMove(int tankId, Direction direction, int speed)
    : id_(tankId), direction_(direction), speed_(speed) {
}

bool TankMove::acceptExecutor(const EventVisitor &executor) {
    return executor.visit(*this);
}

int TankMove::getId() const {
    return id_;
}

Direction TankMove::getDirection() const {
    return direction_;
}

int TankMove::getSpeed() const {
    return speed_;
}

void TankMove::sendTo(boost::asio::ip::tcp::socket &socket) const {
    sendMultipleInts(socket, EventType::TANK_MOVE, id_, direction_, speed_);
}

EventType TankMove::getType() const {
    return EventType::TANK_MOVE;
}

std::unique_ptr<Event> readEvent(boost::asio::ip::tcp::socket &socket) {
    const std::unordered_map<EventType,
                             std::unique_ptr<Event> (*)(tcp::socket &)>
        readers = []() {
            std::unordered_map<EventType,
                               std::unique_ptr<Event> (*)(tcp::socket &)>
                tmp;
            tmp[EventType::TANK_MOVE] = TankMove::readFrom;
            tmp[EventType::SPAWN_TANK] = TankSpawn::readFrom;
            tmp[EventType::BONUS_SPAWN] = BonusSpawn::readFrom;
            tmp[EventType::SET_POSITION] = SetPosition::readFrom;
            tmp[EventType::TANK_SHOOT] = TankShoot::readFrom;
            tmp[EventType::GAME_END] = GameEnd::readFrom;
            return tmp;
        }();
    auto type = static_cast<EventType>(receiveInt(socket));
    assert(static_cast<unsigned>(type) != 0);
    return readers.at(type)(socket);
}

void TankSpawn::sendTo(boost::asio::ip::tcp::socket &socket) const {
    sendMultipleInts(socket, EventType::SPAWN_TANK, tankId_, left_, top_,
                     entityType_, tankSpeed_, bulletSpeed_, reloadTicks_);
}

bool TankSpawn::acceptExecutor(const EventVisitor &executor) {
    return executor.visit(*this);
}

std::unique_ptr<Event> TankSpawn::readFrom(tcp::socket &socket) {
    auto [tankId, left, top, type, tankSpeed, bulletSpeed, reloadTicks] =
        receiveMultipleInts<int, int, int, EntityType, int, int, int>(socket);
    return std::make_unique<TankSpawn>(
        tankId, left, top, type,
        Menu::PlayerSkills{tankSpeed, bulletSpeed, reloadTicks});
}

DecrId TankSpawn::getTankId() const {
    return tankId_;
}
int TankSpawn::getLeft() const {
    return left_;
}

int TankSpawn::getTop() const {
    return top_;
}

TankSpawn::TankSpawn(
    int tankId,  // NOLINT(bugprone-easily-swappable-parameters)
    int left,
    int top,
    EntityType entityType,
    Menu::PlayerSkills skills)
    : tankId_(tankId),
      left_(left),
      top_(top),
      entityType_(entityType),
      tankSpeed_(skills.tankSpeed),
      bulletSpeed_(skills.bulletSpeed),
      reloadTicks_(skills.reloadTicks) {
}

EntityType TankSpawn::getEntityType() const {
    return entityType_;
}

int TankSpawn::getTankSpeed() const {
    return tankSpeed_;
}

int TankSpawn::getBulletSpeed() const {
    return bulletSpeed_;
}

int TankSpawn::getReloadTicks() const {
    return reloadTicks_;
}

EventType TankSpawn::getType() const {
    return EventType::SPAWN_TANK;
}

// NOLINTNEXTLINE(bugprone-easily-swappable-parameters)
BonusSpawn::BonusSpawn(int bonusId, int left, int top, EntityType type)
    : id_(bonusId), left_(left), top_(top), type_(type) {
}

void BonusSpawn::sendTo(tcp::socket &socket) const {
    sendMultipleInts(socket, EventType::BONUS_SPAWN, id_, left_, top_, type_);
}

std::unique_ptr<Event> BonusSpawn::readFrom(tcp::socket &socket) {
    auto [id, left, top, type] =
        receiveMultipleInts<int, int, int, EntityType>(socket);
    return std::make_unique<BonusSpawn>(id, left, top, type);
}

bool BonusSpawn::acceptExecutor(const EventVisitor &executor) {
    return executor.visit(*this);
}

DecrId BonusSpawn::getId() const {
    return id_;
}

int BonusSpawn::getLeft() const {
    return left_;
}

int BonusSpawn::getTop() const {
    return top_;
}

EntityType BonusSpawn::getEntityType() const {
    return type_;
}

EventType BonusSpawn::getType() const {
    return EventType::BONUS_SPAWN;
}

bool TankShoot::acceptExecutor(const EventVisitor &executor) {
    return executor.visit(*this);
}

void TankShoot::sendTo(tcp::socket &socket) const {
    sendMultipleInts(socket, EventType::TANK_SHOOT, tankId_, direction_);
}

int TankShoot::getTankId() const {
    return tankId_;
}

Direction TankShoot::getDirection() const {
    return direction_;
}
TankShoot::TankShoot(int tankId, Direction direction)
    : tankId_(tankId), direction_(direction) {
}
std::unique_ptr<Event> TankShoot::readFrom(tcp::socket &socket) {
    auto [tankId, direction] = receiveMultipleInts<int, Direction>(socket);
    return std::make_unique<TankShoot>(tankId, direction);
}

EventType TankShoot::getType() const {
    return EventType::TANK_SHOOT;
}

// NOLINTNEXTLINE(bugprone-easily-swappable-parameters)
SetPosition::SetPosition(int entityId, int left, int top)
    : entityId_(entityId), left_(left), top_(top) {
}

int SetPosition::getId() const {
    return entityId_;
}
int SetPosition::getLeft() const {
    return left_;
}
int SetPosition::getTop() const {
    return top_;
}

bool SetPosition::acceptExecutor(const EventVisitor &executor) {
    return executor.visit(*this);
}

void SetPosition::sendTo(tcp::socket &socket) const {
    sendMultipleInts(socket, EventType::SET_POSITION, entityId_, left_, top_);
}

std::unique_ptr<Event> SetPosition::readFrom(tcp::socket &socket) {
    auto [id_, left_, top_] = receiveMultipleInts<int, int, int>(socket);
    return std::make_unique<SetPosition>(id_, left_, top_);
}

EventType SetPosition::getType() const {
    return EventType::SET_POSITION;
}

bool GameEnd::acceptExecutor(const EventVisitor &executor) {
    return executor.visit(*this);
}

void GameEnd::sendTo(tcp::socket &socket) const {
    sendMultipleInts(socket, EventType::GAME_END, getKills());
}

int GameEnd::getKills() const {
    return kills_;
}

std::unique_ptr<Event> GameEnd::readFrom(tcp::socket &socket) {
    auto kills = receiveInt(socket);
    return std::make_unique<GameEnd>(kills);
}

GameEnd::GameEnd(int kills) : kills_(kills) {
}

EventType GameEnd::getType() const {
    return EventType::GAME_END;
}
}  // namespace Tanks::model
