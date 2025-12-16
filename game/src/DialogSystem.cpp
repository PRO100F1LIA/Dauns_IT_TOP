#include "DialogSystem.h"
#include "ChemistryEngine.h"
#include <sstream>
#include <iomanip>
#include <cmath>
#include <algorithm>

DialogSystem::DialogSystem() {
    initializeDefaultDialogs();
    initializeDefaultTasks();
}

void DialogSystem::initializeDefaultDialogs() {
    // Level 1 - Jesse (Basic concepts)
    dialogs[1] = Dialog(
        Character::JESSE,
        "Йоу, чувак! Добро пожаловать в лабу! Я Джесси, и нам нужно проверить, что ты не полный идиот.\n\n"
        "Вот что нужно сделать: посчитай молярную массу воды. Формула H2O.\n"
        "Это базовый уровень, так что не облажайся!",
        "Отлично! Ты справился! Джесси одобряет.",
        "Блин, даже я это знаю! Ладно, давай еще разок..."
    );

    // Level 2 - Walter (Molar calculations)
    dialogs[2] = Dialog(
        Character::WALTER,
        "Хорошо. Я Уолтер Уайт. Наука — вот в чем суть.\n\n"
        "Нам нужно ровно 2 моля иодида метамфетамина. Формула: C10H15N•HI.\n"
        "Рассчитай массу в граммах, которую нам нужно взвесить.\n"
        "И не ошибись — от этого зависит чистота продукта.",
        "Верно. Адекватно. Ты можешь быть полезен.",
        "Это элементарно. Пересчитай."
    );

    // Level 3 - Gale (Equation balancing)
    dialogs[3] = Dialog(
        Character::GALE,
        "Добро пожаловать! Я Гейл Беттикер. Люблю точность в химии.\n\n"
        "Уравнение горения пропана: C3H8 + O2 -> CO2 + H2O\n"
        "Балансируй его! Введи коэффициенты через пробел (например: 1 5 3 4)",
        "Превосходно! Ты понимаешь основы стехиометрии.",
        "Хм, нужно еще потренироваться. Попробуй снова."
    );

    // Level 4 - Heisenberg (Stoichiometry)
    dialogs[4] = Dialog(
        Character::WALTER,
        "Теперь серьезно. Я Хайзенберг.\n\n"
        "У нас есть 5 моль бензола (C6H6). Сколько граммов нитробензола (C6H5NO2) "
        "мы получим при нитровании? Уравнение: C6H6 + HNO3 -> C6H5NO2 + H2O\n"
        "Коэффициенты: 1:1. Ответ в граммах.",
        "Отлично. Ты готов к реальной работе.",
        "Нет. Это не то, что нужно. Пересчитай."
    );

    // Level 5 - Gus (Purity control)
    dialogs[5] = Dialog(
        Character::GUS,
        "Густаво Фринг. Качество — превыше всего.\n\n"
        "У нас образец метамфетамина массой 100 г. Примеси составляют 5%.\n"
        "Рассчитай массу чистого продукта в граммах.",
        "Приемлемо. Бизнес требует точности.",
        "Недостаточно точно. Пересчитай."
    );
}

void DialogSystem::initializeDefaultTasks() {
    // Task 1: Molar mass of water (Jesse level)
    Task task1;
    task1.level = 1;
    task1.type = TaskType::MOLAR_MASS;
    task1.description = "Calculate molar mass of water";
    task1.question = "What is the molar mass of H2O? (in g/mol)";
    task1.formula1 = "H2O";
    task1.answer = std::to_string(ChemistryEngine::calculateMolarMass("H2O"));
    task1.tolerance = 0.1;
    task1.dialog = dialogs[1];
    tasks.push_back(task1);

    // Task 2: Moles to grams conversion (Walter level)
    Task task2;
    task2.level = 2;
    task2.type = TaskType::MOLES_CONVERSION;
    task2.description = "Convert moles to grams for methamphetamine HI salt";
    task2.question = "Calculate the mass in grams for 2 moles of C10H15N•HI";
    task2.formula1 = "C10H15N"; // Simplified - treating as single compound
    // For C10H15N•HI, we approximate: C10H15N (149.23) + HI (127.91) = 277.14 g/mol
    task2.inputValue = 2.0;
    task2.answer = "554.28"; // Approximate
    task2.tolerance = 5.0;
    task2.dialog = dialogs[2];
    tasks.push_back(task2);

    // Task 3: Equation balancing (Gale level)
    Task task3;
    task3.level = 3;
    task3.type = TaskType::EQUATION_BALANCE;
    task3.description = "Balance combustion of propane";
    task3.question = "Balance: C3H8 + O2 -> CO2 + H2O\nEnter coefficients separated by spaces (C3H8 O2 CO2 H2O):";
    task3.answer = "1 5 3 4"; // C3H8 + 5O2 -> 3CO2 + 4H2O
    task3.tolerance = 0.0;
    task3.dialog = dialogs[3];
    tasks.push_back(task3);

    // Task 4: Stoichiometry (Heisenberg level)
    Task task4;
    task4.level = 4;
    task4.type = TaskType::STOICHIOMETRY;
    task4.description = "Calculate product yield from benzene nitration";
    task4.question = "If we have 5 moles of C6H6, how many grams of C6H5NO2 will we get? (1:1 ratio)";
    task4.formula1 = "C6H6";
    task4.formula2 = "C6H5NO2";
    task4.inputValue = 5.0;
    task4.reactantCoeff = 1;
    task4.productCoeff = 1;
    double expectedGrams = ChemistryEngine::calculateProductYield(
        task4.formula1, task4.inputValue, task4.formula2, 
        task4.reactantCoeff, task4.productCoeff
    );
    task4.answer = std::to_string(expectedGrams);
    task4.tolerance = 1.0;
    task4.dialog = dialogs[4];
    tasks.push_back(task4);

    // Task 5: Purity calculation (Gus level)
    Task task5;
    task5.level = 5;
    task5.type = TaskType::MOLES_CONVERSION;
    task5.description = "Calculate pure product mass";
    task5.question = "Sample: 100g, impurities: 5%. Calculate pure mass in grams:";
    task5.answer = "95.0";
    task5.tolerance = 0.1;
    task5.dialog = dialogs[5];
    tasks.push_back(task5);
}

bool DialogSystem::loadFromJSON(const std::string& filename) {
    // TODO: Implement JSON loading if needed
    // For now, using default initialization
    return true;
}

Dialog DialogSystem::getDialog(int level) const {
    auto it = dialogs.find(level);
    if (it != dialogs.end()) {
        return it->second;
    }
    return Dialog(Character::WALTER, "Continue...", "Correct!", "Try again.");
}

Task DialogSystem::getTask(int level) const {
    if (level > 0 && level <= static_cast<int>(tasks.size())) {
        return tasks[level - 1];
    }
    return tasks[0]; // Return first task as default
}

bool DialogSystem::checkAnswer(const Task& task, const std::string& userAnswer) const {
    std::string cleanUser = userAnswer;
    std::string cleanAnswer = task.answer;
    
    // Remove spaces and convert to lowercase for comparison
    cleanUser.erase(std::remove_if(cleanUser.begin(), cleanUser.end(), ::isspace), cleanUser.end());
    cleanAnswer.erase(std::remove_if(cleanAnswer.begin(), cleanAnswer.end(), ::isspace), cleanAnswer.end());
    
    std::transform(cleanUser.begin(), cleanUser.end(), cleanUser.begin(), ::tolower);
    std::transform(cleanAnswer.begin(), cleanAnswer.end(), cleanAnswer.begin(), ::tolower);
    
    if (task.tolerance == 0.0) {
        // Exact match for string answers
        return cleanUser == cleanAnswer;
    } else {
        // Try numeric comparison
        try {
            double userNum = std::stod(userAnswer);
            double answerNum = std::stod(task.answer);
            return std::abs(userNum - answerNum) <= task.tolerance;
        } catch (...) {
            return cleanUser == cleanAnswer;
        }
    }
}

bool DialogSystem::checkAnswer(const Task& task, double userAnswer) const {
    try {
        double answerNum = std::stod(task.answer);
        return std::abs(userAnswer - answerNum) <= task.tolerance;
    } catch (...) {
        return false;
    }
}

std::string DialogSystem::getCharacterName(Character c) {
    switch (c) {
        case Character::WALTER: return "Уолтер Уайт (Хайзенберг)";
        case Character::JESSE: return "Джесси Пинкман";
        case Character::MIKE: return "Майк Эрмантраут";
        case Character::GUS: return "Густаво Фринг";
        case Character::GALE: return "Гейл Беттикер";
        case Character::SAUL: return "Сол Гудман";
        default: return "Неизвестный";
    }
}

std::string DialogSystem::getCharacterGreeting(Character c) {
    switch (c) {
        case Character::WALTER:
            return "\"Наука, вот в чем суть!\"";
        case Character::JESSE:
            return "\"Йоу! Ты чёртов прав!\"";
        case Character::MIKE:
            return "\"Нет половинок.\"";
        case Character::GUS:
            return "\"Качество. Превыше всего.\"";
        case Character::GALE:
            return "\"Точность — залог успеха.\"";
        case Character::SAUL:
            return "\"Лучше звоните Солу!\"";
        default:
            return "\"Привет!\"";
    }
}


