CREATE TABLE IF NOT EXISTS players (
    name VARCHAR(15) PRIMARY KEY NOT NULL CHECK(length(name) <= 18 and length(name) >= 2),
    money INTEGER DEFAULT 0 NOT NULL CHECK(money >= 0),
    registration_date datetime DEFAULT (datetime('now','localtime')) NOT NULL,
    online BOOLEAN DEFAULT 0 NOT NULL
);
