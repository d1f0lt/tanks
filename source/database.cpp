#include "database.h"
#include <cassert>
#include <fstream>
#include <memory>

namespace Tanks {

Database::Database(const std::string &filename) {  // NOLINT
    assert(filename.find_last_of(".dblite"));
    connectToDatabase(filename);
}

Database::~Database() {
    sqlite3_close(db);
}

void Database::exec(const std::string &request) { // NOLINT
    char *err = nullptr;
    if (sqlite3_exec(db, request.c_str(), nullptr, nullptr, &err) !=
        SQLITE_OK) {
        std::cerr << err << std::endl;
        sqlite3_free(err);
        exit(1);
    }
}

void Database::createTable(const std::string &patternFilename) {
    std::ifstream databasePattern(patternFilename);
    assert(databasePattern.is_open());
    std::string request;
    std::getline(databasePattern, request, '\0');
    exec(request);
}

// NOLINTBEGIN
static int callback(void *count, [[maybe_unused]] int argc, char **argv, [[maybe_unused]] char **azColName) {
    int *c = static_cast<int *>(count);
    *c = atoi(argv[0]);
    return 0;
}
// NOLINTEND

int Database::getNumberOfRows(const std::string &tableName) {
    int count = 0;
    char *err = nullptr;
    if (sqlite3_exec(db, ("select count(*) from " + tableName).c_str(), callback, &count, &err) != SQLITE_OK) {
        sqlite3_free(err);
        exit(1);
    }
    return count;
}

void Database::connectToDatabase(const std::string &filename) {
    if (sqlite3_open((filename).c_str(), &db) != SQLITE_OK) {
        std::cerr << "Cannot open file: '" + filename + "'" << std::endl;
        exit(1);
    }
}

void Database::dropTable(const std::string &tableName) {
    std::string request = "DROP TABLE IF EXISTS " + tableName;
    exec(request);
}

namespace Menu {

namespace {

std::string convert(int64_t val) {
    return std::to_string(val) + ',';
}

void fillRequestForPlayers(std::string &req, PlayersDatabaseFields &&info) {
    req += "'" + info.name + "'" + ',';
    req += convert(info.money) + convert(info.tankSpeed) + convert(info.bulletSpeed) + std::to_string(info.reloadTicks) + ");";
}

}

PlayersDatabase::PlayersDatabase(const std::string &filename) : Database(filename) {
}

PlayersDatabaseFields PlayersDatabase::getInfoByName(
    const std::string &username) {
    const std::string request =
        "SELECT * FROM players WHERE name = '" + username + "';";
    sqlite3_prepare_v2(db, request.c_str(), -1, &stmt, nullptr);
    sqlite3_step(stmt);
    PlayersDatabaseFields res;
    res.name = std::string(reinterpret_cast<const char *>(sqlite3_column_text(stmt, 0))); // NOLINT
    res.money = sqlite3_column_int64(stmt, 1);
    res.tankSpeed = sqlite3_column_int(stmt, 2);
    res.bulletSpeed = sqlite3_column_int(stmt, 3);
    res.reloadTicks = sqlite3_column_int(stmt, 4);
    sqlite3_free(stmt);
    return res;
}

void PlayersDatabase::insert(PlayersDatabaseFields info) { // NOLINT
    std::string request = "INSERT INTO players (name,money,tank_speed,bullet_speed,reload_ticks) VALUES(";
    fillRequestForPlayers(request, std::move(info));
    exec(request);
}

} // namespace Menu
} // namespace Tanks