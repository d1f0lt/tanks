#ifndef CONSTANCE_H_
#define CONSTANCE_H_

namespace Tanks {
// general
inline constexpr int WINDOW_WIDTH = 1920;
inline constexpr int WINDOW_HEIGHT = 1080;

// game
enum class Direction { LEFT, RIGHT, DOWN, UP };
inline constexpr int TILE_SIZE = 56;  // ONE_TILE_WIDTH = ONE_TILE_HEIGHT = 56
inline constexpr int MARGIN_TOP = 8;  // = MARGIN_DOWN
inline constexpr int MARGIN_LEFT = 288;  // = MARGIN_RIGHT     for centralize
inline constexpr int MAP_HEIGHT = 19;    // number of vertical tiles
inline constexpr int MAP_WIDTH = 24;     // number of horizontal tiles
inline constexpr int TANK_SIZE = 48;     // TANK_WIDTH = TANK_HEIGHT = 48
inline constexpr int DEFAULT_TANK_SPEED = 5;
inline constexpr int BULLET_SPEED = 10;
inline constexpr int BULLET_SIZE = 10;
inline constexpr int RELOAD_TICKS = 10;
inline constexpr int BONUS_SIZE = 40;

// menu
inline constexpr int LEVELS_COUNT = 1;
inline constexpr int TEXT_MARGIN = 10;

}  // namespace Tanks

#endif  // CONSTANCE_H_