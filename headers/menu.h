#ifndef MENU_H
#define MENU_H

#include <SFML/Graphics.hpp>
#include <memory>
#include <vector>
#include "constants.h"

namespace Tanks {

struct MenuController;

namespace Menu {

enum class ButtonType {
    NEW_GAME,
    RESUME,
    SETTINGS,
    CREATE_MAP,
    RATING,
    RETURN,
    EXIT,
    PAUSE,
    UPGRADE,
    SINGLE_PLAYER,
    MULTIPLAYER,
    LEVEL
};

std::string convertButtonTypeToString(ButtonType type);

struct InscriptionInfo final {
    const std::string inscription;
    const size_t characterSize;
    const sf::Color textColor;
};

struct Button {
public:
    Button(const sf::Vector2<float> &size_, const sf::Color &standardColor_, const sf::Color &hoverColor_);

    [[nodiscard]] const sf::Color &getStandardColor() const;

    [[nodiscard]] const sf::Color &getHoverColor() const;

    [[nodiscard]] const sf::Vector2<float> &getSize() const;

    void setSize(const sf::Vector2<float> &newSize);

protected:
    const sf::Color standardColor;
    const sf::Color hoverColor;
    sf::Vector2<float> size;
};

struct ButtonWithType final : Button {
public:
    ButtonWithType(ButtonType type_, const sf::Vector2<float> &size_, const sf::Color &standardColor_, const sf::Color &hoverColor_);

    ButtonWithType(Button btn, ButtonType type_);

    [[nodiscard]] ButtonType getType() const;

private:
    ButtonType type;
};


struct MenuItem;
struct MenuButton;

struct Menu final {
public:
    explicit Menu() = default;

    explicit Menu(size_t menuWidth,
                  const InscriptionInfo &titleInfo,
                  const InscriptionInfo &inscriptionsInfo,
                  const std::vector<ButtonWithType> &buttonsInfo);  // for most

    explicit Menu(size_t menuWidth,
                  const InscriptionInfo &titleInfo,
                  const std::vector<InscriptionInfo> &inscriptions,
                  const std::string &path,
                  size_t quantityPerLine,
                  Button btnInfo);  // for levels

    void draw(sf::RenderWindow &window) const;

    void addIconToLeftUpCorner(const std::string &filename, ButtonType type);

    void addIconToLeftLowerCorner(const std::string &filename, ButtonType type);

    void addMenuItem(std::unique_ptr<MenuItem> &&item);

private:
    std::vector<std::unique_ptr<MenuItem>> items;

    [[nodiscard]] const std::vector<std::unique_ptr<MenuItem>> &getItems()
        const;

    friend struct Tanks::MenuController;
};

struct MenuItem {
public:
    virtual void draw(sf::RenderWindow &window) const = 0;

    [[nodiscard]] virtual sf::Vector2<float> getSize() const = 0;

    [[nodiscard]] virtual sf::Vector2<float> getPosition() const = 0;

    virtual void setPosition(sf::Vector2<float> newPosition) = 0;

    void centralizeByWidth(
        const std::pair<float, float> &rectangleCoordinatesX);

    void centralizeByHeight(
        const std::pair<float, float> &rectangleCoordinatesY);

    virtual ~MenuItem() = default;
};

struct MenuInscription final : MenuItem {
public:
    explicit MenuInscription(const InscriptionInfo &info,
                             const sf::Vector2<float> &coordinates);

    [[nodiscard]] sf::Vector2<float> getSize() const final;

    [[nodiscard]] sf::Vector2<float> getPosition() const final;

    void setPosition(sf::Vector2<float> newPosition) final;

    [[nodiscard]] std::string getContent() const;

    void draw(sf::RenderWindow &window) const final;

private:
    sf::Font font;
    sf::Text text;
};

struct MenuButton : MenuItem {
public:
    MenuButton(std::unique_ptr<MenuItem> &&content,
               const sf::Vector2<float> &coordinates,
               ButtonWithType info);

    ButtonType getType() const;

    void hover();

    [[nodiscard]] sf::Vector2<float> getSize() const override;

    [[nodiscard]] sf::Vector2<float> getPosition() const override;

    void setPosition(sf::Vector2<float> newPosition) override;

    void draw(sf::RenderWindow &window) const override;

protected:
    std::unique_ptr<MenuItem> content;
    mutable sf::RectangleShape rectangle;  // we want draw to be a const
    ButtonWithType info;
};

struct MenuPicture final : MenuItem {
public:
    explicit MenuPicture(const std::string &filename,
                         const sf::Vector2<float> &coordinates);

    explicit MenuPicture(const sf::Image &image_,
                         const sf::Vector2<float> &coordinates);

    [[nodiscard]] sf::Vector2<float> getSize() const final;

    [[nodiscard]] sf::Vector2<float> getPosition() const final;

    void setPosition(sf::Vector2<float> newPosition) final;

    void draw(sf::RenderWindow &window) const final;

private:
    sf::Image image;
    sf::Texture texture;
    sf::Sprite sprite;

    void initWithImage(const sf::Vector2<float> &coordinates);
};

struct MenuPictureWithDescription final : MenuButton {
public:
    explicit MenuPictureWithDescription(
        std::unique_ptr<MenuPicture> &&picture,
        std::unique_ptr<MenuInscription> &&description,
        const sf::Vector2<float> &coordinates,
        Button btnInfo);

    void draw(sf::RenderWindow &window) const final;

    [[nodiscard]] sf::Vector2<float> getSize() const final;

    [[nodiscard]] sf::Vector2<float> getPosition() const final;

    void setPosition(sf::Vector2<float> newPosition) final;

    [[nodiscard]] std::string getDescription() const;

private:
    std::unique_ptr<MenuInscription> description;
};

}  // namespace Menu

}  // namespace Tanks

#endif