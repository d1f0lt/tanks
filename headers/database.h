#ifndef DATABASE_H
#define DATABASE_H

#include <iostream>
#include <string>
#include <vector>
#include "constants.h"
#include "sqlite3.h"

namespace Tanks {

struct DatabaseError : std::runtime_error {
    DatabaseError(const std::string &err);
};

struct Database {
public:
    Database() = default;

    Database(const Database &) = delete;
    Database(Database &&) = default;
    Database &operator=(const Database &) = delete;
    Database &operator=(Database &&) = default;
    virtual ~Database();

    void exec(const std::string &request);

    void createTable(const std::string &patternFilename);
    [[maybe_unused]] void dropTable(const std::string &tableName);

    [[nodiscard]] int getNumberOfRows(const std::string &tableName) const;

    void disconnectFromDatabase();
    [[nodiscard]] bool isConnected() const;

protected:
    sqlite3 *db = nullptr;         // NOLINT
    sqlite3_stmt *stmt = nullptr;  // NOLINT

    void connectToDatabase(const std::string &filename);
};

namespace Menu {

struct PlayerGeneral {
    std::string name;
    int64_t money{};
};

struct PlayerSettings {
    int musicVolume = 50;
    int soundsVolume = 50;
};

struct PlayerSkills {
    int tankSpeed = DEFAULT_TANK_SPEED;
    int bulletSpeed = DEFAULT_BULLET_SPEED;
    int reloadTicks = DEFAULT_RELOAD_TICKS;
};

struct PlayerInfo {
    PlayerGeneral general;
    PlayerSkills skills{};
    PlayerSettings settings{};
};

struct PlayersDatabase : Database {
public:
    PlayersDatabase(const std::string &path);

    void connect();

    PlayerInfo getInfoByName(const std::string &username);
    std::vector<std::string> getAllUsernames();

    void insert(PlayerInfo info);
    void insert(const std::string &name);

    bool checkOnline(const std::string &username);
    void makeOnline(const std::string &username);
    void makeOffline(const std::string &username);

private:
    const std::string filename;

    PlayerGeneral getGeneralInfoByName(const std::string &username);
    PlayerSettings getSettingsInfoByName(const std::string &username);
    PlayerSkills getSkillsInfoByName(const std::string &username);
};

}  // namespace Menu

}  // namespace Tanks

#endif