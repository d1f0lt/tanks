#include "menu/users_menu.h"
#include <cassert>
#include <memory>
#include "database.h"
#include "menu/input_menu.h"
#include "menu/main_menu.h"
#include "menu/menu.h"

namespace Tanks::Menu {

namespace {

PlayersDatabase initDatabase() {
    const static std::string path = "../.data/";
    PlayersDatabase db(path);  // NOLINT
    return db;
}

sf::Sprite initBackground(const std::string &imagesPath) {
    const static std::string backgroundFilename = imagesPath + "background.png";
    sf::Image backgroundImage;
    backgroundImage.loadFromFile(backgroundFilename);
    static sf::Texture backgroundTexture;
    backgroundTexture.loadFromImage(backgroundImage);
    sf::Sprite backgroundSprite;
    backgroundSprite.setTexture(backgroundTexture);
    backgroundSprite.setPosition(0, 0);
    return backgroundSprite;
}

Menu initMenu(PlayersDatabase &db, const std::string &imagesPath) {  // NOLINT
    const static sf::Color textColor{63, 87, 210};
    const static auto menuWidth = static_cast<size_t>(WINDOW_WIDTH / 3.4);

    // title
    const static std::string titleText = "SELECT USER";
    const static size_t titleCharacterSize = 80;
    InscriptionInfo title{titleText, titleCharacterSize, textColor};

    const static size_t inscriptionsCharacterSize = 50;

    // buttons
    auto usernames = db.getAllUsernames();

    const static size_t buttonsHeight = 100;
    const static sf::Color btnStandardColor(0, 0, 0, 150);
    const static sf::Color btnHoverColor(66, 66, 66, 230);

    std::vector<ButtonWithType> buttons;
    std::vector<std::unique_ptr<MenuItem>> items;
    buttons.reserve(usernames.size());
    items.reserve(usernames.size());
    for (const auto &name : usernames) {
        buttons.emplace_back(ButtonWithType(
            ButtonType::USER, sf::Vector2<float>(menuWidth, buttonsHeight),
            btnStandardColor, btnHoverColor));
        InscriptionInfo info{name, inscriptionsCharacterSize, textColor};
        auto inscription =
            std::make_unique<MenuInscription>(info, sf::Vector2<float>{0, 0});
        items.emplace_back(std::move(inscription));
    }
    if (items.size() < 6) {
        buttons.emplace_back(ButtonWithType(
            ButtonType::NEW_USER, sf::Vector2<float>(menuWidth, buttonsHeight),
            btnStandardColor, btnHoverColor));
        auto picture = std::make_unique<MenuPicture>(imagesPath + "plus.png",
                                                     sf::Vector2<float>(0, 0));
        items.emplace_back(std::move(picture));
    }

    Menu menu(menuWidth, title, items, buttons);

    std::vector<std::unique_ptr<MenuItem>> addingButtons;
    addingButtons.reserve(usernames.size());
    for (size_t i = 0; i < usernames.size(); ++i) {
        auto item = std::make_unique<MenuPicture>(imagesPath + "trash.png",
                                                  sf::Vector2<float>{0, 0});
        addingButtons.emplace_back(std::move(item));
    }
    sf::Color addingButtonsStandardColor{0, 0, 0, 0};
    sf::Color addingButtonsHoverColor{128, 128, 128, 128};

    ButtonWithType info{ButtonType::DELETE,
                        sf::Vector2<float>{buttonsHeight, buttonsHeight},
                        addingButtonsStandardColor, addingButtonsHoverColor};
    menu.addAddingButtons(1, std::move(addingButtons), info);

    menu.addIconToLeftLowerCorner(imagesPath + "exit.png", ButtonType::EXIT);

    return menu;
}

void insertNewUser(const std::string &username,
                   PlayersDatabase &db) {  // NOLINT
    if (username.size() >= 2) {
        try {
            db.insert(username);
        } catch (const DatabaseError &err) {
        }
    }
}

}  // namespace

void showUsersMenu(sf::RenderWindow &window) {
    PlayersDatabase db(initDatabase());  // NOLINT

    const static std::string imagesPath = "../images/menu/";
    sf::Sprite backgroundSprite(initBackground(imagesPath));

    Menu menu(initMenu(db, imagesPath));

    while (window.isOpen()) {
        const auto *const res = menu.showMenu(window, backgroundSprite);
        switch (res->getType()) {
            case ButtonType::USER: {
                db.connect();
                auto person = db.getInfoByName(res->getInscription());
                if (!db.isOnline(person.general.name)) {
                    db.makeOnline(person.general.name);
                    db.disconnectFromDatabase();
                    menu.flyAwayToLeft(window, backgroundSprite);
                    showMainMenu(window, backgroundSprite);
                    db.makeOffline(person.general.name);
                    menu.flyOutFromLeft(window, backgroundSprite);
                }
            } break;
            case ButtonType::NEW_USER: {
                const static std::string title = "INPUT NAME";

                menu.flyAwayToLeft(window, backgroundSprite);
                auto ans = showInputMenu(window, backgroundSprite, title);
                if (ans != std::nullopt) {
                    insertNewUser(ans.value(), db);
                    menu = initMenu(db, imagesPath);
                    menu.flyAwayToLeft();
                }

                menu.flyOutFromLeft(window, backgroundSprite);
            } break;
            case ButtonType::EXIT:
                window.close();
                break;
            case ButtonType::DELETE: {
                const auto *item =
                    dynamic_cast<const MenuAdditionalButton *>(res);
                assert(item != nullptr);
                std::string username =
                    dynamic_cast<const MenuButton *>(item->getMainButton())
                        ->getInscription();
                db.deleteByName(username);
                menu = initMenu(db, imagesPath);
            } break;
            default:
                assert(false);
        }
    }
}

}  // namespace Tanks::Menu
