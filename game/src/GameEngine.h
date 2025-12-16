#ifndef GAMEENGINE_H
#define GAMEENGINE_H

#include "DialogSystem.h"
#include "ChemistryEngine.h"
#include <string>

/**
 * @brief GameEngine - Main game state management
 */
class GameEngine {
public:
    enum class GameState {
        MENU,           // Main menu
        DIALOG,         // Showing dialog
        TASK,           // Showing task/question
        RESULT,         // Showing result (correct/incorrect)
        GAME_OVER,      // Game completed
        EXIT            // Exit game
    };

    GameEngine();
    ~GameEngine() = default;

    // Game flow control
    void startGame();
    void nextLevel();
    void checkAnswer(const std::string& answer);
    
    // State management
    GameState getCurrentState() const { return currentState; }
    void setState(GameState state) { currentState = state; }
    
    // Current task/dialog info
    DialogSystem::Task getCurrentTask() const;
    DialogSystem::Dialog getCurrentDialog() const;
    int getCurrentLevel() const { return currentLevel; }
    int getMaxLevel() const { return dialogSystem.getTaskCount(); }
    
    // Result info
    bool getLastAnswerCorrect() const { return lastAnswerCorrect; }
    std::string getLastFeedback() const { return lastFeedback; }
    
    // Reset game
    void reset();

private:
    DialogSystem dialogSystem;
    GameState currentState;
    int currentLevel;
    bool lastAnswerCorrect;
    std::string lastFeedback;
    
    void processCorrectAnswer();
    void processIncorrectAnswer();
};

#endif // GAMEENGINE_H


