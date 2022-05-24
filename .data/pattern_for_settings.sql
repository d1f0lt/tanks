CREATE TABLE IF NOT EXISTS settings (
    name VARCHAR(15) PRIMARY KEY NOT NULL CHECK(length(name) <= 15 and length(name) >= 2),
    music_volume INTEGER DEFAULT 50 NOT NULL CHECK(music_volume >= 0 and music_volume <= 100),
    sounds_volume INTEGER DEFAULT 50 NOT NULL CHECK(sounds_volume >= 0 and sounds_volume <= 100)
);