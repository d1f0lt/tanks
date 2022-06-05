CREATE TABLE IF NOT EXISTS settings (
    name VARCHAR(15) REFERENCES players (name) NOT NULL,
    music_volume INTEGER DEFAULT 50 NOT NULL CHECK(music_volume >= 0 and music_volume <= 100),
    sounds_volume INTEGER DEFAULT 50 NOT NULL CHECK(sounds_volume >= 0 and sounds_volume <= 100)
);