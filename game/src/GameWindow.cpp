#include "GameWindow.h"
#include <sstream>
#include <algorithm>
#include <cmath>

// Color constants
const sf::Color GameWindow::BG_COLOR(30, 30, 30);           // Dark gray #1e1e1e
const sf::Color GameWindow::ACCENT_COLOR(57, 255, 20);      // Bright green #39FF14
const sf::Color GameWindow::TEXT_COLOR(204, 204, 204);      // Light gray #CCCCCC
const sf::Color GameWindow::BUTTON_COLOR(85, 85, 85);       // Medium gray
const sf::Color GameWindow::BUTTON_HOVER_COLOR(102, 102, 102); // Light gray

GameWindow::GameWindow()
    : window(sf::VideoMode(WINDOW_WIDTH, WINDOW_HEIGHT), 
             "Breaking Bonds: A Chemistry Chronicle",
             sf::Style::Titlebar | sf::Style::Close),
      inputText(""),
      inputActive(false),
      selectedButton(-1) {
    
    // Try to load font from Windows fonts directory
    // Try Consolas (monospace, similar to Breaking Bad style)
    if (!font.loadFromFile("C:/Windows/Fonts/consola.ttf")) {
        // Fallback to Arial
        if (!font.loadFromFile("C:/Windows/Fonts/arial.ttf")) {
            // Try Calibri
            if (!font.loadFromFile("C:/Windows/Fonts/calibri.ttf")) {
                std::cerr << "Warning: Could not load font. Text may not display correctly." << std::endl;
                // SFML will use a default font internally, but it may not support Cyrillic
            }
        }
    }
    
    // Setup buttons
    setupButtons();
    updateButtonVisibility();
}

void GameWindow::setupButtons() {
    buttons.clear();
    
    // Button positions and sizes
    float buttonWidth = 150.0f;
    float buttonHeight = 40.0f;
    float buttonY = WINDOW_HEIGHT - 80.0f;
    float buttonSpacing = 20.0f;
    float totalWidth = buttonWidth * 6 + buttonSpacing * 5;
    float startX = (WINDOW_WIDTH - totalWidth) / 2.0f;
    
    // Start button
    buttons.push_back({sf::FloatRect(startX, buttonY, buttonWidth, buttonHeight), 
                       "НАЧАТЬ ИГРУ", true, true});
    
    // Continue button
    buttons.push_back({sf::FloatRect(startX + (buttonWidth + buttonSpacing), buttonY, 
                                     buttonWidth, buttonHeight), 
                       "ПРОДОЛЖИТЬ", false, true});
    
    // Submit button
    buttons.push_back({sf::FloatRect(startX + 2 * (buttonWidth + buttonSpacing), buttonY, 
                                     buttonWidth, buttonHeight), 
                       "ОТВЕТИТЬ", false, true});
    
    // Next button
    buttons.push_back({sf::FloatRect(startX + 3 * (buttonWidth + buttonSpacing), buttonY, 
                                     buttonWidth, buttonHeight), 
                       "ДАЛЬШЕ", false, true});
    
    // Restart button
    buttons.push_back({sf::FloatRect(startX + 4 * (buttonWidth + buttonSpacing), buttonY, 
                                     buttonWidth, buttonHeight), 
                       "ЗАНОВО", false, true});
    
    // Exit button
    buttons.push_back({sf::FloatRect(startX + 5 * (buttonWidth + buttonSpacing), buttonY, 
                                     buttonWidth, buttonHeight), 
                       "ВЫХОД", true, true});
}

void GameWindow::updateButtonVisibility() {
    GameEngine::GameState state = gameEngine.getCurrentState();
    
    // Reset all visibility
    for (auto& btn : buttons) {
        btn.visible = false;
    }
    
    switch (state) {
        case GameEngine::GameState::MENU:
            buttons[0].visible = true; // Start
            buttons[5].visible = true; // Exit
            break;
        case GameEngine::GameState::DIALOG:
            buttons[1].visible = true; // Continue
            buttons[4].visible = true; // Restart
            buttons[5].visible = true; // Exit
            break;
        case GameEngine::GameState::TASK:
            buttons[2].visible = true; // Submit
            buttons[4].visible = true; // Restart
            buttons[5].visible = true; // Exit
            inputActive = true;
            break;
        case GameEngine::GameState::RESULT:
            buttons[4].visible = true; // Restart
            buttons[5].visible = true; // Exit
            if (gameEngine.getLastAnswerCorrect()) {
                if (gameEngine.getCurrentLevel() < gameEngine.getMaxLevel()) {
                    buttons[3].visible = true; // Next
                }
            } else {
                buttons[2].visible = true; // Submit (retry)
            }
            break;
        case GameEngine::GameState::GAME_OVER:
            buttons[4].visible = true; // Restart
            buttons[5].visible = true; // Exit
            break;
        default:
            break;
    }
}

void GameWindow::run() {
    while (window.isOpen()) {
        handleEvents();
        render();
    }
}

void GameWindow::handleEvents() {
    sf::Event event;
    while (window.pollEvent(event)) {
        if (event.type == sf::Event::Closed) {
            window.close();
        }
        else if (event.type == sf::Event::MouseButtonPressed) {
            if (event.mouseButton.button == sf::Mouse::Left) {
                handleMouseClick(event.mouseButton.x, event.mouseButton.y);
            }
        }
        else if (event.type == sf::Event::TextEntered && inputActive) {
            handleTextInput(event.text.unicode);
        }
        else if (event.type == sf::Event::KeyPressed) {
            handleKeyPress(event.key.code);
        }
    }
    
    // Update mouse hover
    sf::Vector2i mousePos = sf::Mouse::getPosition(window);
    selectedButton = getButtonAt(mousePos.x, mousePos.y);
}

void GameWindow::handleMouseClick(int x, int y) {
    int buttonIndex = getButtonAt(x, y);
    if (buttonIndex >= 0 && buttons[buttonIndex].visible && buttons[buttonIndex].enabled) {
        switch (buttonIndex) {
            case 0: // Start
                gameEngine.startGame();
                gameEngine.setState(GameEngine::GameState::DIALOG);
                updateButtonVisibility();
                break;
            case 1: // Continue
                gameEngine.setState(GameEngine::GameState::TASK);
                updateButtonVisibility();
                break;
            case 2: // Submit
                if (!inputText.empty()) {
                    gameEngine.checkAnswer(inputText);
                    inputText.clear();
                    inputActive = false;
                    gameEngine.setState(GameEngine::GameState::RESULT);
                    updateButtonVisibility();
                }
                break;
            case 3: // Next
                gameEngine.nextLevel();
                if (gameEngine.getCurrentState() == GameEngine::GameState::GAME_OVER) {
                    updateButtonVisibility();
                } else {
                    gameEngine.setState(GameEngine::GameState::DIALOG);
                    updateButtonVisibility();
                }
                break;
            case 4: // Restart
                gameEngine.reset();
                inputText.clear();
                inputActive = false;
                updateButtonVisibility();
                break;
            case 5: // Exit
                window.close();
                break;
        }
    }
}

void GameWindow::handleTextInput(sf::Uint32 unicode) {
    if (unicode == '\b' && !inputText.empty()) {
        // Backspace
        inputText.pop_back();
    }
    else if (unicode >= 32 && unicode < 128) {
        // Printable ASCII characters
        inputText += static_cast<char>(unicode);
    }
}

void GameWindow::handleKeyPress(sf::Keyboard::Key key) {
    if (inputActive && key == sf::Keyboard::Enter) {
        if (!inputText.empty()) {
            gameEngine.checkAnswer(inputText);
            inputText.clear();
            inputActive = false;
            gameEngine.setState(GameEngine::GameState::RESULT);
            updateButtonVisibility();
        }
    }
}

int GameWindow::getButtonAt(int x, int y) {
    for (size_t i = 0; i < buttons.size(); ++i) {
        if (buttons[i].rect.contains(static_cast<float>(x), static_cast<float>(y))) {
            return static_cast<int>(i);
        }
    }
    return -1;
}

void GameWindow::render() {
    window.clear(BG_COLOR);
    
    GameEngine::GameState state = gameEngine.getCurrentState();
    
    switch (state) {
        case GameEngine::GameState::MENU:
            renderMenu();
            break;
        case GameEngine::GameState::DIALOG:
            renderDialog();
            break;
        case GameEngine::GameState::TASK:
            renderTask();
            break;
        case GameEngine::GameState::RESULT:
            renderResult();
            break;
        case GameEngine::GameState::GAME_OVER:
            renderGameOver();
            break;
        default:
            break;
    }
    
    // Render buttons
    for (auto& button : buttons) {
        if (button.visible) {
            drawButton(button);
        }
    }
    
    window.display();
}

void GameWindow::renderMenu() {
    // Title
    drawText("BREAKING BONDS", WINDOW_WIDTH / 2.0f, 50.0f, 48, ACCENT_COLOR, true);
    drawText("A Chemistry Chronicle", WINDOW_WIDTH / 2.0f, 110.0f, 20, TEXT_COLOR, true);
    
    // Welcome text
    std::string welcomeText = 
        "Добро пожаловать в BREAKING BONDS!\n\n"
        "Вы — новый стажер, нанятый командой Хайзенберга.\n"
        "Чтобы доказать свою ценность, вам нужно решать химические задачи,\n"
        "начиная с основ и доходя до сложного синтеза.\n\n"
        "Помните: точность — залог успеха. Никаких половинок.\n\n"
        "Нажмите 'НАЧАТЬ ИГРУ' чтобы начать обучение.";
    
    float textY = 200.0f;
    auto lines = wrapText(welcomeText, WINDOW_WIDTH - 100.0f, 16);
    for (const auto& line : lines) {
        drawText(line, WINDOW_WIDTH / 2.0f, textY, 16, TEXT_COLOR, true);
        textY += 25.0f;
    }
}

void GameWindow::renderDialog() {
    DialogSystem::Dialog dialog = gameEngine.getCurrentDialog();
    
    // Character name
    std::string charName = DialogSystem::getCharacterName(dialog.character);
    drawText(charName, WINDOW_WIDTH / 2.0f, 50.0f, 24, sf::Color(255, 165, 0), true); // Orange
    
    // Dialog text
    std::string dialogText = formatDialogText(dialog);
    float textY = 120.0f;
    auto lines = wrapText(dialogText, WINDOW_WIDTH - 100.0f, 16);
    for (const auto& line : lines) {
        drawText(line, 50.0f, textY, 16, TEXT_COLOR, false);
        textY += 25.0f;
    }
    
    // Progress
    int current = gameEngine.getCurrentLevel();
    int max = gameEngine.getMaxLevel();
    std::string levelText = "Уровень: " + std::to_string(current) + "/" + std::to_string(max);
    drawText(levelText, WINDOW_WIDTH / 2.0f, WINDOW_HEIGHT - 150.0f, 16, TEXT_COLOR, true);
    
    if (max > 0) {
        float progress = static_cast<float>(current) / static_cast<float>(max);
        drawProgressBar(100.0f, WINDOW_HEIGHT - 120.0f, WINDOW_WIDTH - 200.0f, 20.0f, progress);
    }
}

void GameWindow::renderTask() {
    DialogSystem::Task task = gameEngine.getCurrentTask();
    DialogSystem::Dialog dialog = gameEngine.getCurrentDialog();
    
    // Character name
    std::string charName = DialogSystem::getCharacterName(dialog.character);
    drawText(charName, WINDOW_WIDTH / 2.0f, 30.0f, 24, sf::Color(255, 165, 0), true);
    
    // Dialog text
    std::string dialogText = formatDialogText(dialog);
    float textY = 80.0f;
    auto lines = wrapText(dialogText, WINDOW_WIDTH - 100.0f, 14);
    for (const auto& line : lines) {
        drawText(line, 50.0f, textY, 14, TEXT_COLOR, false);
        textY += 22.0f;
        if (textY > 250.0f) break; // Limit dialog area
    }
    
    // Task description
    std::string taskDesc = formatTaskText(task);
    drawRectangle(50.0f, 260.0f, WINDOW_WIDTH - 100.0f, 80.0f, 
                 sf::Color(26, 26, 26), sf::Color(102, 102, 102));
    textY = 280.0f;
    auto descLines = wrapText(taskDesc, WINDOW_WIDTH - 120.0f, 14);
    for (const auto& line : descLines) {
        drawText(line, 60.0f, textY, 14, sf::Color::White, false);
        textY += 22.0f;
    }
    
    // Question
    textY = 360.0f;
    auto questionLines = wrapText(task.question, WINDOW_WIDTH - 100.0f, 16);
    for (const auto& line : questionLines) {
        drawText(line, 50.0f, textY, 16, TEXT_COLOR, false);
        textY += 24.0f;
    }
    
    // Input box
    float inputY = textY + 20.0f;
    drawRectangle(50.0f, inputY, WINDOW_WIDTH - 100.0f, 40.0f, 
                 sf::Color(42, 42, 42), ACCENT_COLOR);
    drawText(inputText + "_", 60.0f, inputY + 10.0f, 18, sf::Color::White, false);
    
    // Progress
    int current = gameEngine.getCurrentLevel();
    int max = gameEngine.getMaxLevel();
    std::string levelText = "Уровень: " + std::to_string(current) + "/" + std::to_string(max);
    drawText(levelText, WINDOW_WIDTH / 2.0f, inputY + 60.0f, 14, TEXT_COLOR, true);
    
    if (max > 0) {
        float progress = static_cast<float>(current) / static_cast<float>(max);
        drawProgressBar(100.0f, inputY + 85.0f, WINDOW_WIDTH - 200.0f, 15.0f, progress);
    }
}

void GameWindow::renderResult() {
    renderTask(); // Show task info
    
    // Feedback
    bool correct = gameEngine.getLastAnswerCorrect();
    std::string feedback = gameEngine.getLastFeedback();
    sf::Color feedbackColor = correct ? ACCENT_COLOR : sf::Color(255, 107, 107); // Red for wrong
    
    float feedbackY = WINDOW_HEIGHT - 200.0f;
    drawRectangle(50.0f, feedbackY, WINDOW_WIDTH - 100.0f, 60.0f, 
                 sf::Color(26, 26, 26), feedbackColor);
    
    auto feedbackLines = wrapText(feedback, WINDOW_WIDTH - 120.0f, 16);
    float textY = feedbackY + 15.0f;
    for (const auto& line : feedbackLines) {
        drawText(line, WINDOW_WIDTH / 2.0f, textY, 16, feedbackColor, true);
        textY += 22.0f;
    }
}

void GameWindow::renderGameOver() {
    drawText("ПОЗДРАВЛЯЕМ!", WINDOW_WIDTH / 2.0f, 100.0f, 40, ACCENT_COLOR, true);
    drawText("ХАЙЗЕНБЕРГ", WINDOW_WIDTH / 2.0f, 160.0f, 28, sf::Color(255, 165, 0), true);
    
    std::string gameOverText = 
        "Вы успешно прошли все уровни обучения!\n"
        "Хайзенберг доволен вашими навыками.\n"
        "Вы готовы к реальной работе.\n\n"
        "Наука, вот в чем суть!";
    
    float textY = 250.0f;
    auto lines = wrapText(gameOverText, WINDOW_WIDTH - 100.0f, 18);
    for (const auto& line : lines) {
        drawText(line, WINDOW_WIDTH / 2.0f, textY, 18, TEXT_COLOR, true);
        textY += 30.0f;
    }
    
    // Progress
    int max = gameEngine.getMaxLevel();
    drawProgressBar(100.0f, WINDOW_HEIGHT - 120.0f, WINDOW_WIDTH - 200.0f, 20.0f, 1.0f);
    std::string levelText = "Уровень: " + std::to_string(max) + "/" + std::to_string(max);
    drawText(levelText, WINDOW_WIDTH / 2.0f, WINDOW_HEIGHT - 150.0f, 16, TEXT_COLOR, true);
}

void GameWindow::drawText(const std::string& text, float x, float y, int size, 
                          const sf::Color& color, bool centered) {
    sf::Text sfText;
    sfText.setFont(font);
    sfText.setString(text);
    sfText.setCharacterSize(size);
    sfText.setFillColor(color);
    
    if (centered) {
        sf::FloatRect bounds = sfText.getLocalBounds();
        sfText.setPosition(x - bounds.width / 2.0f, y - bounds.height / 2.0f);
    } else {
        sfText.setPosition(x, y);
    }
    
    window.draw(sfText);
}

void GameWindow::drawButton(Button& button) {
    sf::Color fillColor = BUTTON_COLOR;
    sf::Color outlineColor = ACCENT_COLOR;
    
    // Check if button is hovered
    sf::Vector2i mousePos = sf::Mouse::getPosition(window);
    if (button.rect.contains(static_cast<float>(mousePos.x), static_cast<float>(mousePos.y))) {
        fillColor = BUTTON_HOVER_COLOR;
    }
    
    // Special styling for accent buttons
    if (button.text == "НАЧАТЬ ИГРУ" || button.text == "ОТВЕТИТЬ") {
        fillColor = ACCENT_COLOR;
        outlineColor = sf::Color::Transparent;
    } else if (button.text == "ВЫХОД") {
        fillColor = sf::Color(139, 0, 0); // Dark red
        outlineColor = sf::Color::Transparent;
    }
    
    drawRectangle(button.rect.left, button.rect.top, 
                 button.rect.width, button.rect.height, 
                 fillColor, outlineColor);
    
    // Button text
    float textX = button.rect.left + button.rect.width / 2.0f;
    float textY = button.rect.top + button.rect.height / 2.0f;
    sf::Color textColor = (button.text == "НАЧАТЬ ИГРУ" || button.text == "ОТВЕТИТЬ") 
                         ? sf::Color::Black : TEXT_COLOR;
    drawText(button.text, textX, textY, 14, textColor, true);
}

void GameWindow::drawRectangle(float x, float y, float width, float height, 
                               const sf::Color& fillColor, const sf::Color& outlineColor) {
    sf::RectangleShape rect(sf::Vector2f(width, height));
    rect.setPosition(x, y);
    rect.setFillColor(fillColor);
    if (outlineColor != sf::Color::Transparent) {
        rect.setOutlineColor(outlineColor);
        rect.setOutlineThickness(2.0f);
    }
    window.draw(rect);
}

void GameWindow::drawProgressBar(float x, float y, float width, float height, float progress) {
    // Background
    drawRectangle(x, y, width, height, sf::Color(26, 26, 26), ACCENT_COLOR);
    
    // Progress fill
    float fillWidth = width * std::max(0.0f, std::min(1.0f, progress));
    if (fillWidth > 0) {
        drawRectangle(x, y, fillWidth, height, ACCENT_COLOR, sf::Color::Transparent);
    }
}

std::vector<std::string> GameWindow::wrapText(const std::string& text, float maxWidth, int fontSize) {
    std::vector<std::string> lines;
    
    // Approximate character width (monospace font)
    float charWidth = fontSize * 0.6f;
    int charsPerLine = static_cast<int>(maxWidth / charWidth);
    
    // Split by newlines first
    std::istringstream textStream(text);
    std::string paragraph;
    
    while (std::getline(textStream, paragraph)) {
        // Handle each paragraph separately
        if (paragraph.empty()) {
            lines.push_back(""); // Empty line
            continue;
        }
        
        // Word wrap within paragraph
        std::istringstream paraStream(paragraph);
        std::string word;
        std::string currentLine;
        
        while (paraStream >> word) {
            std::string testLine = currentLine.empty() ? word : currentLine + " " + word;
            if (static_cast<int>(testLine.length()) <= charsPerLine || currentLine.empty()) {
                currentLine = testLine;
            } else {
                if (!currentLine.empty()) {
                    lines.push_back(currentLine);
                }
                currentLine = word;
            }
        }
        
        if (!currentLine.empty()) {
            lines.push_back(currentLine);
        }
    }
    
    return lines;
}

std::string GameWindow::formatDialogText(const DialogSystem::Dialog& dialog) {
    std::string greeting = DialogSystem::getCharacterGreeting(dialog.character);
    return greeting + "\n\n" + dialog.text;
}

std::string GameWindow::formatTaskText(const DialogSystem::Task& task) {
    std::ostringstream ss;
    ss << "Задача: " << task.description << "\n";
    ss << "Тип: ";
    switch (task.type) {
        case DialogSystem::TaskType::MOLAR_MASS:
            ss << "Расчет молярной массы";
            break;
        case DialogSystem::TaskType::MOLES_CONVERSION:
            ss << "Конвертация моли ↔ граммы";
            break;
        case DialogSystem::TaskType::EQUATION_BALANCE:
            ss << "Балансировка уравнения";
            break;
        case DialogSystem::TaskType::STOICHIOMETRY:
            ss << "Стехиометрия";
            break;
        case DialogSystem::TaskType::FORMULA_PARSE:
            ss << "Анализ формулы";
            break;
    }
    return ss.str();
}

