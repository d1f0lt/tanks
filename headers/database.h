#ifndef DATABASE_H
#define DATABASE_H

#include <iostream>
#include <string>
#include "sqlite3.h"

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

    int getNumberOfRows(const std::string &patternFilename);

protected:
    sqlite3 *db = nullptr; // NOLINT
    sqlite3_stmt *stmt = nullptr; // NOLINT

private:

    void connectToDatabase(const std::string &filename);
};

namespace Menu {

struct PlayersDatabaseFields {
    std::string name;
    int64_t money{};
    int tankSpeed{};
    int bulletSpeed{};
    int reloadTicks{};
};

struct PlayersDatabase : Database {
public:
    PlayersDatabase(const std::string &filename);

    PlayersDatabaseFields getInfoByName(const std::string &username);

    void insert(PlayersDatabaseFields info);

};

}

}

#endif