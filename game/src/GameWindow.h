#ifndef GAMEWINDOW_H
#define GAMEWINDOW_H

#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <string>
#include <vector>
#include "GameEngine.h"

/**
 * @brief GameWindow - Main SFML window for Breaking Bonds game
 * Features dark theme inspired by Breaking Bad aesthetic
 */
class GameWindow {
public:
    GameWindow();
    ~GameWindow() = default;

    // Main game loop
    void run();

private:
    // SFML window and rendering
    sf::RenderWindow window;
    sf::Font font;
    
    // Game engine
    GameEngine gameEngine;
    
    // UI state
    std::string inputText;
    bool inputActive;
    int selectedButton;
    
    // UI constants
    static const int WINDOW_WIDTH = 1000;
    static const int WINDOW_HEIGHT = 700;
    static const sf::Color BG_COLOR;
    static const sf::Color ACCENT_COLOR;  // Green #39FF14
    static const sf::Color TEXT_COLOR;
    static const sf::Color BUTTON_COLOR;
    static const sf::Color BUTTON_HOVER_COLOR;
    
    // Button structure
    struct Button {
        sf::FloatRect rect;
        std::string text;
        bool visible;
        bool enabled;
    };
    
    std::vector<Button> buttons;
    
    // Event handling
    void handleEvents();
    void handleMouseClick(int x, int y);
    void handleTextInput(sf::Uint32 unicode);
    void handleKeyPress(sf::Keyboard::Key key);
    
    // Rendering
    void render();
    void renderMenu();
    void renderDialog();
    void renderTask();
    void renderResult();
    void renderGameOver();
    
    // Helper methods
    void drawText(const std::string& text, float x, float y, int size, 
                  const sf::Color& color = TEXT_COLOR, bool centered = false);
    void drawButton(Button& button);
    void drawRectangle(float x, float y, float width, float height, 
                      const sf::Color& fillColor, const sf::Color& outlineColor = sf::Color::Transparent);
    void drawProgressBar(float x, float y, float width, float height, float progress);
    
    // UI state management
    void setupButtons();
    void updateButtonVisibility();
    int getButtonAt(int x, int y);
    
    // Text formatting
    std::string formatDialogText(const DialogSystem::Dialog& dialog);
    std::string formatTaskText(const DialogSystem::Task& task);
    
    // Word wrapping helper
    std::vector<std::string> wrapText(const std::string& text, float maxWidth, int fontSize);
};

#endif // GAMEWINDOW_H

