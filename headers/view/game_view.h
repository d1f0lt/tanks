#ifndef GAME_VIEW_H
#define GAME_VIEW_H

#include <string>
#include "SFML/Graphics.hpp"
#include "game_environment.h"
#include "model/entity.h"
#include "model/game_map.h"
#include "model/game_model.h"

namespace Tanks::View {

struct BlockSpriteHolder {
public:
    BlockSpriteHolder(model::EntityType type_,
                      sf::Texture &texture,
                      const sf::Vector2<int> &coordinates);

    void draw(sf::RenderWindow &window) const;

    model::EntityType getType() const;

    void destroy();

private:
    sf::Sprite sprite;
    model::EntityType type;

    void changeSprite(model::EntityType newType);
};

struct Map final {
public:
    explicit Map(const std::string &filename, int level);

    void draw(sf::RenderWindow &window, model::GameModel &model);

private:
    std::vector<std::vector<BlockSpriteHolder>> map;
    sf::Image image;
    sf::Texture texture;

    void loadLevel(int level);
};

}  // namespace Tanks::View

#endif