#ifndef DATABASE_H
#define DATABASE_H

#include <iostream>
#include <string>
#include <vector>
#include "sqlite3.h"
#include "constants.h"

namespace Tanks {

struct Database {
public:
    Database(const std::string &filename);

    Database(const Database &) = delete;
    Database(Database &&) = default;
    Database &operator=(const Database &) = delete;
    Database &operator=(Database &&) = default;
    ~Database();

    void exec(const std::string &request);

    void createTable(const std::string &patternFilename);
    [[maybe_unused]] void dropTable(const std::string &patternFilename);

    [[nodiscard]] int getNumberOfRows(const std::string &patternFilename) const;

protected:
    sqlite3 *db = nullptr; // NOLINT
    sqlite3_stmt *stmt = nullptr; // NOLINT

private:

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

    PlayerInfo getInfoByName(const std::string &username);
    std::vector<std::string> getAllUsernames();

    void insert(PlayerInfo info);
private:
    PlayerGeneral getGeneralInfoByName(const std::string &username);
    PlayerSettings getSettingsInfoByName(const std::string &username);
    PlayerSkills getSkillsInfoByName(const std::string &username);
};

}

}

#endif