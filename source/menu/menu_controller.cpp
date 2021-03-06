#include "menu/menu_controller.h"
#include <cassert>
#include "menu/slider_bar.h"
#include "menu/textbox.h"

namespace Tanks {
namespace {
void updateVolume(Menu::PlayerSettings &settings,
                  const Menu::SliderBar *sliderBar) {
    switch (sliderBar->getType()) {
        case Menu::SliderBarType::MUSIC:
            settings.musicVolume =
                static_cast<int>(sliderBar->getCurrentVolume());
            break;
        case Menu::SliderBarType::SOUND:
            settings.soundsVolume =
                static_cast<int>(sliderBar->getCurrentVolume());
            break;
        default:
            assert(false);
    }
}
}  // namespace

std::optional<Menu::MenuButton *> MenuController::control(
    const std::vector<std::unique_ptr<Menu::MenuItem>> &items,
    sf::RenderWindow &window,
    const sf::Event &event) {
    for (const auto &menuItem : items) {
        auto *item = dynamic_cast<Menu::MenuButton *>(menuItem.get());
        if (item == nullptr) {
            continue;
        }
        auto coordinates = static_cast<sf::Vector2<int>>(item->getPosition());
        auto proportions = static_cast<sf::Vector2<int>>(item->getSize());
        if (sf::IntRect(coordinates.x, coordinates.y, proportions.x,
                        proportions.y)
                .contains(sf::Mouse::getPosition(window))) {
            item->hover();
            if (event.type == sf::Event::MouseButtonReleased &&
                event.mouseButton.button == sf::Mouse::Left) {  // NOLINT
                return item;
            }
        }
    }
    return std::nullopt;
}

std::optional<Menu::MenuButton *> MenuController::control(
    const Menu::Menu &menu,
    sf::RenderWindow &window,
    const sf::Event &event) {
    const auto &items = menu.getItems();
    return control(items, window, event);
}

const static int ENTERED_KEY = 13;

std::optional<std::string> MenuController::textEntered(const Menu::Menu &menu,
                                                       sf::Event &event) {
    for (const auto &menuItem : menu.getItems()) {
        auto *item = dynamic_cast<Menu::TextBox *>(menuItem.get());
        if (item == nullptr) {
            continue;
        }
        auto charTyped = event.text.unicode;  // NOLINT
        if (charTyped == ENTERED_KEY) {
            return item->getContent();
        }
        if (charTyped < 128) {
            item->input(static_cast<char>(charTyped));
        }
    }
    return std::nullopt;
}

void MenuController::sliderMove(const Menu::Menu &menu,
                                sf::RenderWindow &window,
                                Menu::PlayerSettings &settings) {
    for (const auto &menuItem : menu.getItems()) {
        auto *item = dynamic_cast<Menu::SliderBar *>(menuItem.get());
        if (item == nullptr) {
            continue;
        }
        item->logic(window);
        updateVolume(settings, item);
    }
}
}  // namespace Tanks