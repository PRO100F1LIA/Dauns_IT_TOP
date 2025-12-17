#include "GameEngine.h"
#include <algorithm>

GameEngine::GameEngine() 
    : currentState(GameState::MENU), 
      currentLevel(0), 
      lastAnswerCorrect(false),
      lastFeedback("") {
}

void GameEngine::startGame() {
    currentLevel = 1;
    currentState = GameState::DIALOG;
    lastAnswerCorrect = false;
    lastFeedback = "";
}

void GameEngine::nextLevel() {
    if (currentLevel < getMaxLevel()) {
        currentLevel++;
        currentState = GameState::DIALOG;
    } else {
        currentState = GameState::GAME_OVER;
    }
}

void GameEngine::checkAnswer(const std::string& answer) {
    if (currentState != GameState::TASK) {
        return;
    }
    
    DialogSystem::Task currentTask = getCurrentTask();
    lastAnswerCorrect = dialogSystem.checkAnswer(currentTask, answer);
    
    if (lastAnswerCorrect) {
        processCorrectAnswer();
    } else {
        processIncorrectAnswer();
    }
    
    currentState = GameState::RESULT;
}

DialogSystem::Task GameEngine::getCurrentTask() const {
    return dialogSystem.getTask(currentLevel);
}

DialogSystem::Dialog GameEngine::getCurrentDialog() const {
    return dialogSystem.getDialog(currentLevel);
}

void GameEngine::processCorrectAnswer() {
    DialogSystem::Dialog dialog = getCurrentDialog();
    lastFeedback = dialog.correctResponse;
}

void GameEngine::processIncorrectAnswer() {
    DialogSystem::Dialog dialog = getCurrentDialog();
    lastFeedback = dialog.incorrectResponse;
}

void GameEngine::reset() {
    currentLevel = 0;
    currentState = GameState::MENU;
    lastAnswerCorrect = false;
    lastFeedback = "";
}



