CREATE TABLE IF NOT EXISTS skills (
    name VARCHAR(15) PRIMARY KEY NOT NULL CHECK(length(name) <= 15 and length(name) >= 2),
    tank_speed INTEGER DEFAULT 2 NOT NULL CHECK(tank_speed >= 2 and tank_speed <= 7),
    bullet_speed INTEGER DEFAULT 8 NOT NULL CHECK(bullet_speed >= 8 and bullet_speed <= 13),
    reload_ticks INTEGER DEFAULT 100 NOT NULL CHECK(reload_ticks <= 100 and reload_ticks >= 50)
)