#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QMainWindow>
#include <QWidget>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QTextEdit>
#include <QLineEdit>
#include <QPushButton>
#include <QProgressBar>
#include <QFont>
#include <QPalette>
#include <QColor>
#include <QString>
#include "GameEngine.h"

/**
 * @brief UI_MainWindow - Main Qt GUI window for Breaking Bonds game
 * Features dark theme inspired by Breaking Bad aesthetic
 */
class UI_MainWindow : public QMainWindow {
    Q_OBJECT

public:
    explicit UI_MainWindow(QWidget *parent = nullptr);
    ~UI_MainWindow() = default;

private slots:
    void onStartClicked();
    void onContinueClicked();
    void onSubmitAnswerClicked();
    void onNextClicked();
    void onRestartClicked();
    void onExitClicked();

private:
    // UI Components
    QWidget *centralWidget;
    QVBoxLayout *mainLayout;
    
    // Title and branding
    QLabel *titleLabel;
    QLabel *subtitleLabel;
    
    // Dialog/Text area
    QTextEdit *dialogTextArea;
    QLabel *characterLabel;
    
    // Task area
    QLabel *taskQuestionLabel;
    QTextEdit *taskDescriptionArea;
    QLineEdit *answerInput;
    QPushButton *submitButton;
    
    // Feedback area
    QLabel *feedbackLabel;
    
    // Control buttons
    QPushButton *startButton;
    QPushButton *continueButton;
    QPushButton *nextButton;
    QPushButton *restartButton;
    QPushButton *exitButton;
    
    // Progress
    QProgressBar *progressBar;
    QLabel *levelLabel;
    
    // Game engine
    GameEngine gameEngine;
    
    // UI Setup methods
    void setupUI();
    void applyDarkTheme();
    void showMenu();
    void showDialog();
    void showTask();
    void showResult();
    void showGameOver();
    void updateProgress();
    
    // Helper methods
    QString formatDialogText(const DialogSystem::Dialog& dialog);
    QString formatTaskText(const DialogSystem::Task& task);
};

#endif // UI_MAINWINDOW_H

