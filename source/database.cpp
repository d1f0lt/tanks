#include "database.h"
#include <cassert>
#include <fstream>
#include <memory>

namespace Tanks {

DatabaseError::DatabaseError(const std::string &err) : std::runtime_error(err) {
}

Database::~Database() {
    disconnectFromDatabase();
}

void Database::exec(const std::string &request) {
    char *err = nullptr;  // NOLINT
    if (sqlite3_exec(db, request.c_str(), nullptr, nullptr, &err) !=
        SQLITE_OK) {
        std::string errMsg(err);
        sqlite3_free(err);
        throw DatabaseError(errMsg);
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
static int callback(void *count,
                    [[maybe_unused]] int argc,
                    char **argv,
                    [[maybe_unused]] char **azColName) {
    int *c = static_cast<int *>(count);
    *c = atoi(*argv);
    return 0;
}
// NOLINTEND

int Database::getNumberOfRows(const std::string &tableName) const {
    int count = 0;
    char *err = nullptr;  // NOLINT
    if (sqlite3_exec(db, ("select count(*) from " + tableName).c_str(),
                     callback, &count, &err) != SQLITE_OK) {
        std::string errMsg(err);
        sqlite3_free(err);
        throw DatabaseError(errMsg);
    }
    return count;
}

void Database::connectToDatabase(const std::string &filename) {
    assert(filename.find_last_of(".dblite"));
    if (sqlite3_open(filename.c_str(), &db) != SQLITE_OK) {
        throw DatabaseError("Cannot open file: '" + filename + "'");
    }
}

void Database::disconnectFromDatabase() {
    if (db != nullptr) {
        assert(sqlite3_close(db) == SQLITE_OK);
        db = nullptr;
    }
}

bool Database::isConnected() const {
    return db != nullptr;
}

void Database::dropTable(const std::string &tableName) {
    std::string request = "DROP TABLE IF EXISTS " + tableName;
    exec(request);
}

namespace Menu {

PlayersDatabase::PlayersDatabase(const std::string &path)
    : filename(path + "players.dblite") {
    connect();
#ifndef MENU_TEST
    createTable(path + "pattern_for_players.sql");
    createTable(path + "pattern_for_settings.sql");
    createTable(path + "pattern_for_skills.sql");
    createTable(path + "pattern_for_rating.sql");
#endif
    disconnectFromDatabase();
}

PlayerInfo PlayersDatabase::getInfoByName(const std::string &username) {
    bool flag = isConnected();
    connect();
    PlayerInfo res{
        getGeneralInfoByName(username), getSkillsInfoByName(username),
        getSettingsInfoByName(username), getRatingInfoByName(username)};
    if (!flag) {
        disconnectFromDatabase();
    }
    return res;
}

namespace {
std::string requestEnd(const std::string &username) {
    return " WHERE name = '" + username + "';";
}
}  // namespace

PlayerGeneral PlayersDatabase::getGeneralInfoByName(
    const std::string &username) {
    bool flag = isConnected();
    connect();
    const std::string request =
        "SELECT name, money FROM players" + requestEnd(username);
    PlayerGeneral res;
    res.name = username;
    sqlite3_prepare_v2(db, request.c_str(), -1, &stmt, nullptr);
    sqlite3_step(stmt);
    res.money = sqlite3_column_int64(stmt, 1);
    sqlite3_finalize(stmt);
    if (!flag) {
        disconnectFromDatabase();
    }
    return res;
}

PlayerSkills PlayersDatabase::getSkillsInfoByName(const std::string &username) {
    const std::string request =
        "SELECT tank_speed, bullet_speed, reload_ticks, number_of_life FROM skills" +
        requestEnd(username);
    bool flag = isConnected();
    connect();
    PlayerSkills res;
    sqlite3_prepare_v2(db, request.c_str(), -1, &stmt, nullptr);
    sqlite3_step(stmt);
    res.tankSpeed = sqlite3_column_int(stmt, 0);
    res.bulletSpeed = sqlite3_column_int(stmt, 1);
    res.reloadTicks = sqlite3_column_int(stmt, 2);
    res.lifeAmount = sqlite3_column_int(stmt, 3);
    sqlite3_finalize(stmt);
    if (!flag) {
        disconnectFromDatabase();
    }
    return res;
}

PlayerSettings PlayersDatabase::getSettingsInfoByName(
    const std::string &username) {
    const std::string request =
        "SELECT music_volume, sounds_volume FROM settings" +
        requestEnd(username);
    bool flag = isConnected();
    connect();
    PlayerSettings res;
    sqlite3_prepare_v2(db, request.c_str(), -1, &stmt, nullptr);
    sqlite3_step(stmt);
    res.musicVolume = sqlite3_column_int(stmt, 0);
    res.soundsVolume = sqlite3_column_int(stmt, 1);
    sqlite3_finalize(stmt);
    if (!flag) {
        disconnectFromDatabase();
    }
    return res;
}

namespace {

std::string getSelectRequestForRatingByName(
    const std::string &username,  // NOLINT
    const std::string &mode) {
    std::string res = "SELECT " + mode + "_kills, " + mode + "_deaths, " +
                      mode + "_wins, " + mode + "_defeats ";
    res += "FROM rating" + requestEnd(username);
    return res;
}

}  // namespace

PlayerRating PlayersDatabase::getRatingInfoByName(const std::string &username,
                                                  const std::string &mode) {
    const std::string request = getSelectRequestForRatingByName(username, mode);
    bool flag = isConnected();
    connect();
    PlayerRating res;
    sqlite3_prepare_v2(db, request.c_str(), -1, &stmt, nullptr);
    sqlite3_step(stmt);
    res.kills = sqlite3_column_int(stmt, 0);
    res.deaths = sqlite3_column_int(stmt, 1);
    res.wins = sqlite3_column_int(stmt, 2);
    res.defeats = sqlite3_column_int(stmt, 3);
    sqlite3_finalize(stmt);
    if (!flag) {
        disconnectFromDatabase();
    }
    return res;
}

PlayerAllRating PlayersDatabase::getRatingInfoByName(
    const std::string &username) {
    const std::string request = "SELECT * FROM rating" + requestEnd(username);
    bool flag = isConnected();
    connect();
    PlayerAllRating res;
    res.singlePlayer = getRatingInfoByName(username, "single_player");
    res.multiplayer = getRatingInfoByName(username, "multiplayer");
    if (!flag) {
        disconnectFromDatabase();
    }
    return res;
}

std::vector<std::string> PlayersDatabase::getAllUsernames() {
    const static std::string request =
        "SELECT name FROM players ORDER BY registration_date;";
    bool flag = isConnected();
    connect();
    size_t cnt = getNumberOfRows("players");
    std::vector<std::string> res;
    res.reserve(cnt);
    sqlite3_prepare_v2(db, request.c_str(), -1, &stmt, nullptr);
    for (size_t i = 0; i < cnt; ++i) {
        sqlite3_step(stmt);
        res.emplace_back(std::string(reinterpret_cast<const char *>(  // NOLINT
            sqlite3_column_text(stmt, 0))));
    }
    sqlite3_finalize(stmt);
    if (!flag) {
        disconnectFromDatabase();
    }
    return res;
}

namespace {

template <typename T>
std::string convert(T val) {
    return std::to_string(val) + ',';
}

std::string playersAddRequest(PlayerGeneral &info) {
    std::string req = "INSERT INTO players (name,money) VALUES(";
    req += "'" + info.name + "'," + std::to_string(info.money) + ");";
    return req;
}

std::string skillsAddRequest(PlayerInfo &info) {
    auto &skills = info.skills;
    std::string req =
        "INSERT INTO skills (name,tank_speed,bullet_speed,reload_ticks,number_of_life) "
        "VALUES(";
    req += "'" + info.general.name + "',";
    req += convert(skills.tankSpeed) + convert(skills.bulletSpeed) +
           convert(skills.reloadTicks) + std::to_string(skills.lifeAmount) + ");";
    return req;
}

std::string settingsAddRequest(PlayerInfo &info) {
    auto &set = info.settings;
    std::string req =
        "INSERT INTO settings (name,music_volume,sounds_volume) VALUES(";
    assert(set.soundsVolume <= 100 && set.musicVolume <= 100);
    req += "'" + info.general.name + "',";
    req += convert(set.musicVolume) + std::to_string(set.soundsVolume) + ");";
    return req;
}

std::string ratingAddRequest(PlayerInfo &info) {
    auto &rat = info.rating;
    std::string req = "INSERT INTO rating VALUES(";
    req += "'" + info.general.name + "',";
    req += convert(rat.singlePlayer.kills) + convert(rat.singlePlayer.deaths) +
           convert(rat.singlePlayer.wins) + convert(rat.singlePlayer.defeats) +
           convert(rat.multiplayer.kills) + convert(rat.multiplayer.deaths) +
           convert(rat.multiplayer.wins) +
           std::to_string(rat.multiplayer.defeats) + ");";
    return req;
}

}  // namespace

void PlayersDatabase::insert(PlayerInfo info) {
    bool flag = isConnected();
    connect();
    exec(playersAddRequest(info.general));
    exec(skillsAddRequest(info));
    exec(settingsAddRequest(info));
    exec(ratingAddRequest(info));
    if (!flag) {
        disconnectFromDatabase();
    }
}

void PlayersDatabase::insert(const std::string &name) {
    PlayerInfo info{PlayerGeneral{name}};
    insert(info);
}

namespace {

std::string playersUpdateRequest(PlayerGeneral &info) {
    std::string request =
        "UPDATE players SET money = " + std::to_string(info.money) +
        requestEnd(info.name);
    return request;
}

std::string skillsUpdateRequest(PlayerInfo &playerInfo) {
    auto &info = playerInfo.skills;
    std::string request =
        "UPDATE skills SET tank_speed = " + convert(info.tankSpeed) +
        "bullet_speed = " + convert(info.bulletSpeed) +
        "reload_ticks = " + convert(info.reloadTicks) +
        "number_of_life = " + std::to_string(info.lifeAmount) +
        requestEnd(playerInfo.general.name);
    return request;
}

std::string settingsUpdateRequest(PlayerInfo &playerInfo) {
    auto &info = playerInfo.settings;
    std::string request =
        "UPDATE settings SET music_volume = " + convert(info.musicVolume) +
        "sounds_volume = " + std::to_string(info.soundsVolume) +
        requestEnd(playerInfo.general.name);
    return request;
}

std::string ratingUpdateRequest(PlayerInfo &playerInfo) {
    auto &rat = playerInfo.rating;
    std::string request =
        "UPDATE rating SET single_player_kills = " +
        convert(rat.singlePlayer.kills) +
        "single_player_deaths = " + convert(rat.singlePlayer.deaths) +
        "single_player_wins = " + convert(rat.singlePlayer.wins) +
        "single_player_defeats = " + convert(rat.singlePlayer.defeats) +
        "multiplayer_kills = " + convert(rat.multiplayer.wins) +
        "multiplayer_deaths = " + convert(rat.multiplayer.deaths) +
        "multiplayer_wins = " + convert(rat.multiplayer.wins) +
        "multiplayer_defeats = " + std::to_string(rat.multiplayer.defeats) +
        requestEnd(playerInfo.general.name);
    return request;
}

}  // namespace

void PlayersDatabase::updateInfo(PlayerInfo &info) {
    bool flag = isConnected();
    connect();
    exec(playersUpdateRequest(info.general));
    exec(skillsUpdateRequest(info));
    exec(settingsUpdateRequest(info));
    exec(ratingUpdateRequest(info));
    if (!flag) {
        disconnectFromDatabase();
    }
}

void PlayersDatabase::deleteByName(const std::string &username) {
    const static std::string requestStart = "DELETE FROM ";
    bool flag = isConnected();
    connect();
    exec(requestStart + "skills" + requestEnd(username));
    exec(requestStart + "settings" + requestEnd(username));
    exec(requestStart + "rating" + requestEnd(username));
    exec(requestStart + "players" + requestEnd(username));
    if (!flag) {
        disconnectFromDatabase();
    }
}

bool PlayersDatabase::isOnline(const std::string &username) {
    const std::string request =
        "SELECT online FROM players" + requestEnd(username);
    bool flag = isConnected();
    connect();
    sqlite3_prepare_v2(db, request.c_str(), -1, &stmt, nullptr);
    sqlite3_step(stmt);
    auto ans = static_cast<bool>(sqlite3_column_int(stmt, 0));
    sqlite3_finalize(stmt);
    if (!flag) {
        disconnectFromDatabase();
    }
    return ans;
}

void PlayersDatabase::makeOnline(const std::string &username) {
    const std::string request =
        "UPDATE players SET online = 1" + requestEnd(username);
    bool flag = isConnected();
    connect();
    exec(request);
    if (!flag) {
        disconnectFromDatabase();
    }
}

void PlayersDatabase::makeOffline(const std::string &username) {
    const std::string request =
        "UPDATE players SET online = 0" + requestEnd(username);
    bool flag = isConnected();
    connect();
    exec(request);
    if (!flag) {
        disconnectFromDatabase();
    }
}

void PlayersDatabase::connect() {
    if (db == nullptr) {
        connectToDatabase(filename);
    }
}

}  // namespace Menu
}  // namespace Tanks