CREATE TABLE IF NOT EXISTS rating (
    name VARCHAR(15) REFERENCES players(name) NOT NULL,
    single_player_kills INTEGER DEFAULT 0 NOT NULL CHECK(single_player_kills >= 0),
    single_player_deaths INTEGER DEFAULT 0 NOT NULL CHECK(single_player_kills >= 0),
    multiplayer_kills INTEGER DEFAULT 0 NOT NULL CHECK(multiplayer_kills >= 0),
    multiplayer_deaths INTEGER DEFAULT 0 NOT NULL CHECK(multiplayer_deaths >= 0)
);