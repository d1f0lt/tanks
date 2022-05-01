#ifndef CONSTANCE_H_
#define CONSTANCE_H_

namespace Tanks {
enum class Direction { LEFT, RIGHT, DOWN, UP };

const int WINDOW_WIDTH = 1920;
const int WINDOW_HEIGHT = 1080;
const int TILE_SIZE = 56;     // ONE_TILE_WIDTH = ONE_TILE_HEIGHT = 56
const int MARGIN_TOP = 8;     // = MARGIN_DOWN
const int MARGIN_LEFT = 288;  // = MARGIN_RIGHT     for centralize
const int MAP_HEIGHT = 19;    // number of vertical tiles
const int MAP_WIDTH = 24;     // number of horizontal tiles
const int TANK_SIZE = 48;     // TANK_WIDTH = TANK_HEIGHT = 48
constexpr int TANK_SPEED = 5;
constexpr int BULLET_SPEED = 10;
constexpr int BULLET_SIZE = 2;

}  // namespace Tanks

#endif  // CONSTANCE_H_