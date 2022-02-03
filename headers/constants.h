#ifndef CONSTANCE_H_
#define CONSTANCE_H_

namespace Tanks {
enum class Direction { LEFT, RIGHT, DOWN, UP };

const int WINDOW_WIDTH = 1920;
const int WINDOW_HEIGHT = 1080;
const int TILE_SIZE = 56; // ONE_TILE_WIDTH = ONE_TILE_HEIGHT = 56
const int MARGIN_TOP = 4;  // = MARGIN_DOWN
const int MARGIN_LEFT = 288; // = MARGIN_RIGHT     for centralize
const int MAP_HEIGHT = 18;
const int MAP_WIDTH = 24;

}  // namespace Tanks

#endif  // CONSTANCE_H_