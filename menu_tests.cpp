#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN

#include <memory>
#include "constants.h"
#include "doctest.h"
#include "menu/menu.h"

using namespace Tanks::Menu;

TEST_CASE("Picture size") {
    sf::Image img;
    img.loadFromFile("../images/menu/return.png");
    SUBCASE("construct with filename") {
        MenuPicture picture("../images/menu/return.png", {0, 0});
        REQUIRE(picture.getPosition() == sf::Vector2<float>{0, 0});
        CHECK(static_cast<sf::Vector2u>(picture.getSize()) == img.getSize());
    }
    SUBCASE("construct with image") {
        MenuPicture picture(img, {0, 0});
        REQUIRE(picture.getPosition() == sf::Vector2<float>{0, 0});
        CHECK(static_cast<sf::Vector2u>(picture.getSize()) == img.getSize());
    }
}

TEST_CASE("standard position") {
    auto picture = std::make_unique<MenuPicture>("../images/menu/return.png",
                                                 sf::Vector2<float>{0, 0});

    InscriptionInfo info{"123", 50, sf::Color(0, 0, 0)};
    auto inscription =
        std::make_unique<MenuInscription>(info, sf::Vector2<float>(0, 0));

    SUBCASE("picture") {
        REQUIRE(picture->getPosition().x == 0);
        REQUIRE(picture->getPosition().y == 0);
        CHECK(picture->getStandardPosition().x == 0);
        CHECK(picture->getStandardPosition().y == 0);

        SUBCASE("button with picture") {
            auto size = picture->getSize();
            MenuButton btn(std::move(picture), {0, 0},
                           {ButtonType::EXIT, size, sf::Color(), sf::Color()});
            CHECK(btn.getPosition() == sf::Vector2<float>{0, 0});
            CHECK(btn.getPosition() == btn.getStandardPosition());
        }
    }

    SUBCASE("inscription") {
        REQUIRE(inscription->getPosition().x == 0);
        REQUIRE(inscription->getPosition().y == 0);
        CHECK(inscription->getStandardPosition().x == 0);
        CHECK(inscription->getStandardPosition().y == 0);

        SUBCASE("button with inscription") {
            auto size = inscription->getSize();
            MenuButton btn(std::move(inscription), {0, 0},
                           {ButtonType::EXIT, size, sf::Color(), sf::Color()});
            REQUIRE(btn.getPosition() == sf::Vector2<float>{0, 0});
            CHECK(btn.getPosition() == btn.getStandardPosition());
        }
    }

    SUBCASE("picture with description") {
        auto size = picture->getSize();
        MenuPictureWithDescription item(std::move(picture),
                                        std::move(inscription), {0, 0},
                                        {size, sf::Color(), sf::Color()});
        REQUIRE(item.getPosition() == sf::Vector2<float>{0, 0});
        CHECK(item.getPosition() == item.getStandardPosition());
    }
}

TEST_CASE("Add menu item") {
    Menu menu;
    const static int steps = 10;
    for (int i = 0; i < steps; ++i) {
        menu.addMenuItem(std::make_unique<MenuPicture>(
            "../images/menu/return.png", sf::Vector2<float>(0, 0)));
        CHECK(menu.getItems().size() == 3 * i + 1);
        menu.addIconToLeftUpCorner("../images/menu/return.png",
                                   ButtonType::RETURN);
        CHECK(menu.getItems().size() == 3 * i + 2);
        menu.addIconToLeftLowerCorner("../images/menu/return.png",
                                      ButtonType::RETURN);
        CHECK(menu.getItems().size() == 3 * i + 3);
    }
}

TEST_CASE("Menu animation") {
    sf::RenderWindow window;
    sf::Sprite backgroundSprite;

    auto checks = [](Menu &menu, sf::RenderWindow &window,
                     sf::Sprite &backgroundSprite, size_t elements_count) {
        const auto &items = menu.getItems();
        SUBCASE("check flyAwayToRight without parameters") {
            menu.flyAwayToRight();
            REQUIRE(items.size() == elements_count);
            for (const auto &item : items) {
                CHECK(item->getPosition().x >= Tanks::WINDOW_WIDTH);
                CHECK(item->getPosition().y == item->getStandardPosition().y);
            }
        }

        SUBCASE("check flyAwayToRight with parameters") {
            menu.flyAwayToRight(window, backgroundSprite);
            REQUIRE(items.size() == elements_count);
            for (const auto &item : items) {
                CHECK(item->getPosition().x >= Tanks::WINDOW_WIDTH);
                CHECK(item->getPosition().y == item->getStandardPosition().y);
            }
        }

        SUBCASE("check flyOutFromRight") {
            menu.flyAwayToRight();
            menu.flyOutFromRight(window, backgroundSprite);
            REQUIRE(items.size() == elements_count);
            for (const auto &item : items) {
                CHECK(item->getPosition() == item->getStandardPosition());
            }
        }

        SUBCASE("check flyAwayToLeft") {
            menu.flyAwayToLeft(window, backgroundSprite);
            REQUIRE(items.size() == elements_count);
            for (const auto &item : items) {
                CHECK(item->getPosition().x < 0);
                CHECK(item->getPosition().y == item->getStandardPosition().y);
            }
        }

        SUBCASE("check flyOutFromLeft") {
            menu.flyAwayToLeft(window, backgroundSprite);
            menu.flyOutFromLeft(window, backgroundSprite);
            REQUIRE(items.size() == elements_count);
            for (const auto &item : items) {
                CHECK(item->getPosition() == item->getStandardPosition());
            }
        }
    };

    SUBCASE("construct from buttons") {
        auto menu = ([]() {
            const static sf::Color textColor;
            const static auto menuWidth =
                static_cast<size_t>(Tanks::WINDOW_WIDTH / 3.4);

            // title
            const static std::string titleText = "JUST TANKS";
            const static size_t titleCharacterSize = 80;
            InscriptionInfo title{titleText, titleCharacterSize, textColor};

            // inscriptions
            const static std::string inscriptionsText;
            const static size_t inscriptionsCharacterSize = 50;
            InscriptionInfo inscriptions{inscriptionsText,
                                         inscriptionsCharacterSize, textColor};

            // buttons
            const static std::vector<ButtonType> buttonTypes = {
                ButtonType::NEW_GAME, ButtonType::UPGRADE,
                ButtonType::CREATE_MAP, ButtonType::RATING};
            const static size_t buttonsHeight = 100;
            const static sf::Color btnStandardColor;
            const static sf::Color btnHoverColor;
            std::vector<ButtonWithType> buttons;
            buttons.reserve(buttonTypes.size());
            for (auto type : buttonTypes) {
                buttons.emplace_back(ButtonWithType(
                    type, sf::Vector2<float>(menuWidth, buttonsHeight),
                    btnStandardColor, btnHoverColor));
            }

            return Menu(menuWidth, title, inscriptions, buttons);
        })();
        checks(menu, window, backgroundSprite, 5);
    }

    SUBCASE("construct from picture with description") {
        auto menu = []() {
            const static sf::Color textColor{63, 87, 210};
            const static size_t menuWidth =
                static_cast<int>(Tanks::WINDOW_WIDTH / 1.5);
            const static size_t quantityPerLine = 4;
            const static size_t quantityElement = 7;
            const static sf::Color standardColor(0, 0, 0, 150);

            // title
            const static std::string titleText = "SELECT LEVEL";
            const static size_t titleCharacterSize = 76;
            const static InscriptionInfo title{titleText, titleCharacterSize,
                                               textColor};

            // inscriptions
            std::vector<InscriptionInfo> inscriptions;
            inscriptions.reserve(quantityElement);
            const static int characterSize = 32;
            for (size_t i = 1; i <= quantityElement; ++i) {
                inscriptions.emplace_back(InscriptionInfo{
                    std::to_string(i), characterSize, textColor});
            }

            const static sf::Color btnStandardColor(0, 0, 0, 150);
            const static sf::Color btnHoverColor(66, 66, 66, 230);
            Button btnInfo{sf::Vector2<float>(0, 0), btnStandardColor,
                           btnHoverColor};
            return Menu(menuWidth, title, inscriptions,
                        "../images/menu/levels/", quantityPerLine, btnInfo);
        }();
        checks(menu, window, backgroundSprite, 8);
    }
}