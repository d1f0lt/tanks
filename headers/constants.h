#ifndef CONSTANCE_H_
#define CONSTANCE_H_

namespace Tanks {

// general
constexpr int WINDOW_WIDTH = 1920;
constexpr int WINDOW_HEIGHT = 1080;

// game
enum class Direction { LEFT, RIGHT, DOWN, UP };
constexpr int TILE_SIZE = 56;     // ONE_TILE_WIDTH = ONE_TILE_HEIGHT = 56
constexpr int MARGIN_TOP = 8;     // = MARGIN_DOWN
constexpr int MARGIN_LEFT = 288;  // = MARGIN_RIGHT     for centralize
constexpr int MAP_HEIGHT = 19;    // number of vertical tiles
constexpr int MAP_WIDTH = 24;     // number of horizontal tiles
constexpr int TANK_SIZE = 48;     // TANK_WIDTH = TANK_HEIGHT = 48
constexpr int DEFAULT_TANK_SPEED = 5;
constexpr int BULLET_SPEED = 10;
constexpr int BULLET_SIZE = 2;

// menu
constexpr int LEVELS_COUNT = 1;
constexpr int TEXT_MARGIN = 10;

}  // namespace Tanks

#endif  // CONSTANCE_H_