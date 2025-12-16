#include "UI_MainWindow.h"
#include <QApplication>
#include <QMessageBox>
#include <QFontDatabase>
#include <QPushButton>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QTextEdit>
#include <QLineEdit>
#include <QProgressBar>
#include <sstream>
#include <iomanip>

UI_MainWindow::UI_MainWindow(QWidget *parent)
    : QMainWindow(parent) {
    setupUI();
    applyDarkTheme();
    showMenu();
}

void UI_MainWindow::setupUI() {
    centralWidget = new QWidget(this);
    setCentralWidget(centralWidget);
    mainLayout = new QVBoxLayout(centralWidget);
    mainLayout->setSpacing(15);
    mainLayout->setContentsMargins(20, 20, 20, 20);
    
    // Title
    titleLabel = new QLabel("BREAKING BONDS", this);
    titleLabel->setAlignment(Qt::AlignCenter);
    QFont titleFont("Consolas", 32, QFont::Bold);
    titleLabel->setFont(titleFont);
    titleLabel->setStyleSheet("color: #39FF14;"); // Bright green (Breaking Bad color)
    
    subtitleLabel = new QLabel("A Chemistry Chronicle", this);
    subtitleLabel->setAlignment(Qt::AlignCenter);
    QFont subtitleFont("Consolas", 14);
    subtitleLabel->setFont(subtitleFont);
    subtitleLabel->setStyleSheet("color: #AAAAAA;");
    
    mainLayout->addWidget(titleLabel);
    mainLayout->addWidget(subtitleLabel);
    mainLayout->addSpacing(10);
    
    // Character label
    characterLabel = new QLabel("", this);
    characterLabel->setAlignment(Qt::AlignCenter);
    QFont charFont("Consolas", 12, QFont::Bold);
    characterLabel->setFont(charFont);
    characterLabel->setStyleSheet("color: #FFA500; padding: 5px;");
    mainLayout->addWidget(characterLabel);
    
    // Dialog text area
    dialogTextArea = new QTextEdit(this);
    dialogTextArea->setReadOnly(true);
    dialogTextArea->setMinimumHeight(150);
    QFont dialogFont("Consolas", 10);
    dialogTextArea->setFont(dialogFont);
    dialogTextArea->setStyleSheet(
        "background-color: #1a1a1a; "
        "color: #CCCCCC; "
        "border: 2px solid #39FF14; "
        "border-radius: 5px; "
        "padding: 10px;"
    );
    mainLayout->addWidget(dialogTextArea);
    
    // Task description
    taskDescriptionArea = new QTextEdit(this);
    taskDescriptionArea->setReadOnly(true);
    taskDescriptionArea->setMinimumHeight(80);
    taskDescriptionArea->setFont(dialogFont);
    taskDescriptionArea->setStyleSheet(
        "background-color: #1a1a1a; "
        "color: #FFFFFF; "
        "border: 1px solid #666666; "
        "border-radius: 5px; "
        "padding: 8px;"
    );
    mainLayout->addWidget(taskDescriptionArea);
    
    // Task question label
    taskQuestionLabel = new QLabel("", this);
    taskQuestionLabel->setWordWrap(true);
    taskQuestionLabel->setFont(dialogFont);
    taskQuestionLabel->setStyleSheet("color: #FFFFFF; padding: 5px;");
    mainLayout->addWidget(taskQuestionLabel);
    
    // Answer input
    answerInput = new QLineEdit(this);
    answerInput->setPlaceholderText("Введите ответ...");
    answerInput->setFont(dialogFont);
    answerInput->setStyleSheet(
        "background-color: #2a2a2a; "
        "color: #FFFFFF; "
        "border: 2px solid #39FF14; "
        "border-radius: 3px; "
        "padding: 8px; "
        "font-size: 12pt;"
    );
    mainLayout->addWidget(answerInput);
    
    // Feedback label
    feedbackLabel = new QLabel("", this);
    feedbackLabel->setAlignment(Qt::AlignCenter);
    feedbackLabel->setWordWrap(true);
    feedbackLabel->setFont(dialogFont);
    feedbackLabel->setStyleSheet("color: #39FF14; padding: 10px; min-height: 30px;");
    mainLayout->addWidget(feedbackLabel);
    
    // Progress bar
    progressBar = new QProgressBar(this);
    progressBar->setMinimum(0);
    progressBar->setMaximum(100);
    progressBar->setValue(0);
    progressBar->setStyleSheet(
        "QProgressBar {"
        "  border: 2px solid #39FF14; "
        "  border-radius: 5px; "
        "  text-align: center; "
        "  color: #FFFFFF; "
        "  background-color: #1a1a1a; "
        "}"
        "QProgressBar::chunk {"
        "  background-color: #39FF14; "
        "}"
    );
    mainLayout->addWidget(progressBar);
    
    // Level label
    levelLabel = new QLabel("Уровень: 0/0", this);
    levelLabel->setAlignment(Qt::AlignCenter);
    levelLabel->setFont(dialogFont);
    levelLabel->setStyleSheet("color: #AAAAAA;");
    mainLayout->addWidget(levelLabel);
    
    // Buttons layout
    QHBoxLayout *buttonLayout = new QHBoxLayout();
    
    startButton = new QPushButton("НАЧАТЬ ИГРУ", this);
    startButton->setFont(QFont("Consolas", 11, QFont::Bold));
    startButton->setStyleSheet(
        "QPushButton {"
        "  background-color: #39FF14; "
        "  color: #000000; "
        "  border: none; "
        "  border-radius: 5px; "
        "  padding: 10px 20px; "
        "  font-weight: bold; "
        "}"
        "QPushButton:hover {"
        "  background-color: #2ECC71; "
        "}"
        "QPushButton:pressed {"
        "  background-color: #27AE60; "
        "}"
    );
    
    continueButton = new QPushButton("ПРОДОЛЖИТЬ", this);
    continueButton->setFont(QFont("Consolas", 11));
    continueButton->setStyleSheet(
        "QPushButton {"
        "  background-color: #555555; "
        "  color: #FFFFFF; "
        "  border: 2px solid #39FF14; "
        "  border-radius: 5px; "
        "  padding: 10px 20px; "
        "}"
        "QPushButton:hover {"
        "  background-color: #666666; "
        "}"
    );
    
    submitButton = new QPushButton("ОТВЕТИТЬ", this);
    submitButton->setFont(QFont("Consolas", 11, QFont::Bold));
    submitButton->setStyleSheet(
        "QPushButton {"
        "  background-color: #39FF14; "
        "  color: #000000; "
        "  border: none; "
        "  border-radius: 5px; "
        "  padding: 10px 30px; "
        "  font-weight: bold; "
        "}"
        "QPushButton:hover {"
        "  background-color: #2ECC71; "
        "}"
    );
    
    nextButton = new QPushButton("ДАЛЬШЕ", this);
    nextButton->setFont(QFont("Consolas", 11));
    nextButton->setStyleSheet(continueButton->styleSheet());
    
    restartButton = new QPushButton("ЗАНОВО", this);
    restartButton->setFont(QFont("Consolas", 11));
    restartButton->setStyleSheet(continueButton->styleSheet());
    
    exitButton = new QPushButton("ВЫХОД", this);
    exitButton->setFont(QFont("Consolas", 11));
    exitButton->setStyleSheet(
        "QPushButton {"
        "  background-color: #8B0000; "
        "  color: #FFFFFF; "
        "  border: none; "
        "  border-radius: 5px; "
        "  padding: 10px 20px; "
        "}"
        "QPushButton:hover {"
        "  background-color: #A00000; "
        "}"
    );
    
    buttonLayout->addWidget(startButton);
    buttonLayout->addWidget(continueButton);
    buttonLayout->addWidget(submitButton);
    buttonLayout->addWidget(nextButton);
    buttonLayout->addWidget(restartButton);
    buttonLayout->addWidget(exitButton);
    
    // Initially hide buttons except start and exit
    continueButton->hide();
    submitButton->hide();
    nextButton->hide();
    restartButton->hide();
    
    mainLayout->addLayout(buttonLayout);
    
    // Connect signals
    connect(startButton, &QPushButton::clicked, this, &UI_MainWindow::onStartClicked);
    connect(continueButton, &QPushButton::clicked, this, &UI_MainWindow::onContinueClicked);
    connect(submitButton, &QPushButton::clicked, this, &UI_MainWindow::onSubmitAnswerClicked);
    connect(nextButton, &QPushButton::clicked, this, &UI_MainWindow::onNextClicked);
    connect(restartButton, &QPushButton::clicked, this, &UI_MainWindow::onRestartClicked);
    connect(exitButton, &QPushButton::clicked, this, &UI_MainWindow::onExitClicked);
    connect(answerInput, &QLineEdit::returnPressed, this, &UI_MainWindow::onSubmitAnswerClicked);
    
    setWindowTitle("Breaking Bonds: A Chemistry Chronicle");
    resize(900, 700);
}

void UI_MainWindow::applyDarkTheme() {
    QPalette darkPalette;
    darkPalette.setColor(QPalette::Window, QColor(30, 30, 30));
    darkPalette.setColor(QPalette::WindowText, QColor(200, 200, 200));
    darkPalette.setColor(QPalette::Base, QColor(26, 26, 26));
    darkPalette.setColor(QPalette::AlternateBase, QColor(40, 40, 40));
    darkPalette.setColor(QPalette::ToolTipBase, QColor(57, 255, 20));
    darkPalette.setColor(QPalette::ToolTipText, QColor(0, 0, 0));
    darkPalette.setColor(QPalette::Text, QColor(200, 200, 200));
    darkPalette.setColor(QPalette::Button, QColor(50, 50, 50));
    darkPalette.setColor(QPalette::ButtonText, QColor(200, 200, 200));
    darkPalette.setColor(QPalette::BrightText, QColor(57, 255, 20));
    darkPalette.setColor(QPalette::Link, QColor(57, 255, 20));
    darkPalette.setColor(QPalette::Highlight, QColor(57, 255, 20));
    darkPalette.setColor(QPalette::HighlightedText, QColor(0, 0, 0));
    
    qApp->setPalette(darkPalette);
    setStyleSheet("QMainWindow { background-color: #1e1e1e; }");
}

void UI_MainWindow::showMenu() {
    titleLabel->show();
    subtitleLabel->show();
    characterLabel->hide();
    dialogTextArea->setPlainText(
        "Добро пожаловать в BREAKING BONDS!\n\n"
        "Вы — новый стажер, нанятый командой Хайзенберга.\n"
        "Чтобы доказать свою ценность, вам нужно решать химические задачи,\n"
        "начиная с основ и доходя до сложного синтеза.\n\n"
        "Помните: точность — залог успеха. Никаких половинок.\n\n"
        "Нажмите 'НАЧАТЬ ИГРУ' чтобы начать обучение."
    );
    taskDescriptionArea->hide();
    taskQuestionLabel->hide();
    answerInput->hide();
    feedbackLabel->hide();
    progressBar->hide();
    levelLabel->hide();
    
    startButton->show();
    continueButton->hide();
    submitButton->hide();
    nextButton->hide();
    restartButton->hide();
    exitButton->show();
}

void UI_MainWindow::showDialog() {
    DialogSystem::Dialog dialog = gameEngine.getCurrentDialog();
    characterLabel->setText(DialogSystem::getCharacterName(dialog.character));
    characterLabel->show();
    dialogTextArea->setPlainText(formatDialogText(dialog));
    dialogTextArea->show();
    
    taskDescriptionArea->hide();
    taskQuestionLabel->hide();
    answerInput->hide();
    feedbackLabel->hide();
    
    startButton->hide();
    continueButton->show();
    submitButton->hide();
    nextButton->hide();
    restartButton->show();
    exitButton->show();
    
    updateProgress();
}

void UI_MainWindow::showTask() {
    DialogSystem::Task task = gameEngine.getCurrentTask();
    DialogSystem::Dialog dialog = gameEngine.getCurrentDialog();
    
    characterLabel->setText(DialogSystem::getCharacterName(dialog.character));
    characterLabel->show();
    
    dialogTextArea->setPlainText(formatDialogText(dialog));
    dialogTextArea->show();
    
    taskDescriptionArea->setPlainText(formatTaskText(task));
    taskDescriptionArea->show();
    
    taskQuestionLabel->setText(QString::fromStdString(task.question));
    taskQuestionLabel->show();
    
    answerInput->clear();
    answerInput->show();
    answerInput->setFocus();
    
    feedbackLabel->clear();
    feedbackLabel->hide();
    
    startButton->hide();
    continueButton->hide();
    submitButton->show();
    nextButton->hide();
    restartButton->show();
    exitButton->show();
    
    updateProgress();
}

void UI_MainWindow::showResult() {
    bool correct = gameEngine.getLastAnswerCorrect();
    std::string feedback = gameEngine.getLastFeedback();
    
    feedbackLabel->setText(QString::fromStdString(feedback));
    
    if (correct) {
        feedbackLabel->setStyleSheet("color: #39FF14; padding: 10px; min-height: 30px; font-weight: bold;");
    } else {
        feedbackLabel->setStyleSheet("color: #FF6B6B; padding: 10px; min-height: 30px; font-weight: bold;");
    }
    feedbackLabel->show();
    
    startButton->hide();
    continueButton->hide();
    submitButton->hide();
    
    if (correct) {
        if (gameEngine.getCurrentLevel() < gameEngine.getMaxLevel()) {
            nextButton->show();
        } else {
            showGameOver();
            return;
        }
    } else {
        submitButton->show(); // Allow retry
    }
    
    restartButton->show();
    exitButton->show();
}

void UI_MainWindow::showGameOver() {
    dialogTextArea->setPlainText(
        "ПОЗДРАВЛЯЕМ!\n\n"
        "Вы успешно прошли все уровни обучения!\n"
        "Хайзенберг доволен вашими навыками.\n"
        "Вы готовы к реальной работе.\n\n"
        "Наука, вот в чем суть!"
    );
    characterLabel->setText("ХАЙЗЕНБЕРГ");
    characterLabel->show();
    
    taskDescriptionArea->hide();
    taskQuestionLabel->hide();
    answerInput->hide();
    feedbackLabel->hide();
    
    startButton->hide();
    continueButton->hide();
    submitButton->hide();
    nextButton->hide();
    restartButton->show();
    exitButton->show();
    
    updateProgress();
}

void UI_MainWindow::updateProgress() {
    int current = gameEngine.getCurrentLevel();
    int max = gameEngine.getMaxLevel();
    
    if (max > 0) {
        int progress = (current * 100) / max;
        progressBar->setValue(progress);
        levelLabel->setText(QString("Уровень: %1/%2").arg(current).arg(max));
        progressBar->show();
        levelLabel->show();
    }
}

QString UI_MainWindow::formatDialogText(const DialogSystem::Dialog& dialog) {
    std::stringstream ss;
    ss << DialogSystem::getCharacterGreeting(dialog.character) << "\n\n";
    ss << dialog.text;
    return QString::fromStdString(ss.str());
}

QString UI_MainWindow::formatTaskText(const DialogSystem::Task& task) {
    std::stringstream ss;
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
    return QString::fromStdString(ss.str());
}

void UI_MainWindow::onStartClicked() {
    gameEngine.startGame();
    showDialog();
}

void UI_MainWindow::onContinueClicked() {
    gameEngine.setState(GameEngine::GameState::TASK);
    showTask();
}

void UI_MainWindow::onSubmitAnswerClicked() {
    QString answer = answerInput->text().trimmed();
    if (answer.isEmpty()) {
        return;
    }
    
    gameEngine.checkAnswer(answer.toStdString());
    showResult();
}

void UI_MainWindow::onNextClicked() {
    gameEngine.nextLevel();
    
    if (gameEngine.getCurrentState() == GameEngine::GameState::GAME_OVER) {
        showGameOver();
    } else {
        showDialog();
    }
}

void UI_MainWindow::onRestartClicked() {
    gameEngine.reset();
    showMenu();
}

void UI_MainWindow::onExitClicked() {
    QApplication::quit();
}

