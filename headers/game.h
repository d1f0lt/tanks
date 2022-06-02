#ifndef GAME_H_
#define GAME_H_

#include <SFML/Graphics.hpp>
#include <boost/asio/ip/tcp.hpp>
#include <optional>
#include "menu/menu.h"

namespace Tanks {

std::optional<Menu::ButtonType> startGame(
    sf::RenderWindow &window,
    int level = 1,
    std::optional<boost::asio::ip::tcp::endpoint> endpoint = std::nullopt);

}

#endif  // GAME_H_
