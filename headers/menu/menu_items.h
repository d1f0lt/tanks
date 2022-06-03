#ifndef MENU_ITEMS_H
#define MENU_ITEMS_H

#include <SFML/Graphics.hpp>
#include <memory>

namespace Tanks::Menu {

enum class ButtonType {
    NEW_GAME,
    RESUME,
    SETTINGS,
    RATING,
    RETURN,
    EXIT,
    PAUSE,
    UPGRADE,
    SINGLE_PLAYER,
    MULTIPLAYER,
    LEVEL,
    USER,
    NEW_USER,
    USERS,
    DELETE,
    IMPROVE
};

std::string convertButtonTypeToString(ButtonType type);

struct MenuItem {
public:
    explicit MenuItem(const sf::Vector2<float> &coordinates);

    virtual void draw(sf::RenderWindow &window) const = 0;

    [[nodiscard]] virtual sf::Vector2<float> getSize() const = 0;
    [[nodiscard]] virtual sf::Vector2<float> getPosition() const = 0;
    [[nodiscard]] const sf::Vector2<float> &getStandardPosition() const;

    void setStandardPosition(const sf::Vector2<float> &newPosition);
    virtual void setPosition(sf::Vector2<float> newPosition) = 0;
    void move(const sf::Vector2<float> &distance);

    void centralizeByWidth(
        const std::pair<float, float> &rectangleCoordinatesX);
    void centralizeByHeight(
        const std::pair<float, float> &rectangleCoordinatesY);

    MenuItem() = default;
    MenuItem(const MenuItem &) = default;
    MenuItem(MenuItem &&) = default;
    MenuItem &operator=(const MenuItem &) = default;
    MenuItem &operator=(MenuItem &&) = default;
    virtual ~MenuItem() = default;

private:
    sf::Vector2<float>
        standardCoordinates{};  // for animation, if the MenuItem is contained
                                // within another MenuItem, then the values may
                                // most likely be incorrect
};

struct InscriptionInfo final {
    std::string inscription{};
    size_t characterSize{};
    sf::Color textColor{};
};

struct MenuInscription final : MenuItem {
public:
    explicit MenuInscription(const InscriptionInfo &parameters,
                             const sf::Vector2<float> &coordinates);

    [[nodiscard]] sf::Vector2<float> getSize() const final;
    [[nodiscard]] sf::Vector2<float> getPosition() const final;
    [[nodiscard]] std::string getContent() const;

    void setPosition(sf::Vector2<float> newPosition) final;
    void setContent(const std::string &newString);
    void setTextColor(const sf::Color &color);

    void draw(sf::RenderWindow &window) const final;

private:
    sf::Font font;
    sf::Text text;

    void updateOrigin();
};

struct Button {
public:
    Button(const sf::Vector2<float> &size_,
           const sf::Color &standardColor_,
           const sf::Color &hoverColor_);

    [[nodiscard]] const sf::Vector2<float> &getSize() const;
    [[nodiscard]] const sf::Color &getStandardColor() const;
    [[nodiscard]] const sf::Color &getHoverColor() const;

    void setSize(const sf::Vector2<float> &newSize);

private:
    const sf::Color standardColor;
    const sf::Color hoverColor;
    sf::Vector2<float> size;
};

struct ButtonWithType final : Button {
public:
    ButtonWithType(ButtonType type_,
                   const sf::Vector2<float> &size_,
                   const sf::Color &standardColor_,
                   const sf::Color &hoverColor_);

    ButtonWithType(Button btn, ButtonType type_);

    [[nodiscard]] ButtonType getType() const;

private:
    ButtonType type;
};

struct MenuRectangle : MenuItem {
public:
    MenuRectangle(Button info, const sf::Vector2<float> &coordinates);

    sf::Vector2<float> getSize() const override;
    sf::Vector2<float> getPosition() const override;

    void setPosition(sf::Vector2<float> newPosition) override;
    [[maybe_unused]] void setBorderColor(const sf::Color &color);

    void draw(sf::RenderWindow &window) const override;

protected:
    mutable sf::RectangleShape rectangle;  // NOLINT
};

struct MenuButton : MenuRectangle {
public:
    MenuButton(std::unique_ptr<MenuItem> &&content,
               const sf::Vector2<float> &coordinates,
               ButtonWithType info);

    void hover();

    [[nodiscard]] ButtonType getType() const;
    [[nodiscard]] sf::Vector2<float> getSize() const override;
    [[nodiscard]] sf::Vector2<float> getPosition() const override;
    [[nodiscard]] std::string getInscription() const;

    void setPosition(sf::Vector2<float> newPosition) override;

    void draw(sf::RenderWindow &window) const override;

protected:
    [[nodiscard]] MenuItem *getContent() const;
    [[nodiscard]] sf::RectangleShape &getRectangle() const;
    [[nodiscard]] const ButtonWithType &getButtonInfo() const;

private:
    std::unique_ptr<MenuItem> content;
    ButtonWithType info;
};

struct MenuPicture final : MenuItem {
public:
    explicit MenuPicture(const std::string &filename,
                         const sf::Vector2<float> &coordinates);

    explicit MenuPicture(const sf::Image &image_,
                         const sf::Vector2<float> &coordinates);

    explicit MenuPicture(
        const std::string &filename,
        size_t sizeOfOne,
        size_t count,
        const sf::Vector2<float> &coordinates);  // for animation

    [[nodiscard]] sf::Vector2<float> getSize() const final;
    [[nodiscard]] sf::Vector2<float> getPosition() const final;

    void setPosition(sf::Vector2<float> newPosition) final;

    void draw(sf::RenderWindow &window) const final;

private:
    sf::Image image;
    sf::Texture texture;
    std::vector<sf::Sprite> sprites;
    mutable size_t curIndex = 0;
    mutable sf::Clock timer;

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
    [[nodiscard]] std::string getDescription() const;

    void setPosition(sf::Vector2<float> newPosition) final;

private:
    std::unique_ptr<MenuInscription> description;
};

struct MenuAdditionalButton final : MenuButton {
public:
    MenuAdditionalButton(const MenuItem *mainButton_,
                         int marginFromLeft,
                         std::unique_ptr<MenuItem> &&content,
                         ButtonWithType &info);

    const MenuItem *getMainButton() const;

private:
    const MenuItem *const mainButton;
};

struct OwningRectangle final : MenuRectangle {
public:
    OwningRectangle(std::unique_ptr<MenuInscription> &&title,
                    std::vector<std::unique_ptr<MenuInscription>> &&leftItems,
                    std::vector<std::unique_ptr<MenuInscription>> &&rightItems,
                    const sf::Color &backgroundColor,
                    const sf::Vector2<float> &coordinates);

    void setPosition(sf::Vector2<float> newPosition) final;

    void draw(sf::RenderWindow &window) const final;

private:
    std::unique_ptr<MenuInscription> title;
    std::vector<std::unique_ptr<MenuInscription>> leftItems;
    std::vector<std::unique_ptr<MenuInscription>> rightItems;

    const static size_t padding = 30;

    float calcMaxRightItemWidth() const;
    float calcMaxLeftItemWidth() const;

    static float calcMaxItemWidth(
        const std::vector<std::unique_ptr<MenuInscription>> &items);
    static void drawItems(
        sf::RenderWindow &window,
        const std::vector<std::unique_ptr<MenuInscription>> &items);
};
}  // namespace Tanks::Menu

#endif