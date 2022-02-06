//
// Created by ilma4 on 2/6/22.
//

#include "movable_object_view.h"

namespace Tanks {
sf::Sprite MovableView::getSprite() const {
    auto sprite = holder.getSprite(object.get_direction());
    sprite.setPosition(sf::Vector2<float>(object.get_coordinates()));
    return sprite;
}

MovableView::MovableView(const MovableObject &object_,
                         const std::string &filename)
    : object(object_), holder(filename) {  // TODO : create one holder for tank
                                           // and use link to them in all tanks
}

}  // namespace Tanks
