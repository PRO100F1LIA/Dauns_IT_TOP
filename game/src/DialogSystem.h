#ifndef DIALOGSYSTEM_H
#define DIALOGSYSTEM_H

#include <string>
#include <vector>
#include <map>
#include <memory>

/**
 * @brief DialogSystem - Manages dialogues and character interactions
 */
class DialogSystem {
public:
    // Character types
    enum class Character {
        WALTER,     // Walter White / Heisenberg
        JESSE,      // Jesse Pinkman
        MIKE,       // Mike Ehrmantraut
        GUS,        // Gustavo Fring
        GALE,       // Gale Boetticher
        SAUL        // Saul Goodman (bonus)
    };

    // Dialog entry
    struct Dialog {
        Character character;
        std::string text;
        std::string correctResponse;
        std::string incorrectResponse;
        
        Dialog() : character(Character::WALTER) {}
        Dialog(Character c, const std::string& t, 
               const std::string& correct = "", 
               const std::string& incorrect = "")
            : character(c), text(t), correctResponse(correct), incorrectResponse(incorrect) {}
    };

    // Task definition
    enum class TaskType {
        MOLAR_MASS,         // Calculate molar mass
        MOLES_CONVERSION,   // Convert moles to grams or vice versa
        EQUATION_BALANCE,   // Balance chemical equation
        STOICHIOMETRY,      // Calculate product yield
        FORMULA_PARSE       // Parse and understand formula
    };

    struct Task {
        int level;
        TaskType type;
        std::string description;
        std::string question;
        std::string answer; // Expected answer (can be numeric or formula string)
        double tolerance;   // For numeric answers
        Dialog dialog;
        
        // Task-specific data
        std::string formula1;      // For molar mass, conversion tasks
        std::string formula2;      // For stoichiometry
        double inputValue;         // Input moles/grams
        int reactantCoeff;
        int productCoeff;
    };

    DialogSystem();
    ~DialogSystem() = default;

    // Load dialogs and tasks from JSON file
    bool loadFromJSON(const std::string& filename);
    
    // Get dialog by level
    Dialog getDialog(int level) const;
    
    // Get task by level
    Task getTask(int level) const;
    
    // Check answer
    bool checkAnswer(const Task& task, const std::string& userAnswer) const;
    bool checkAnswer(const Task& task, double userAnswer) const;
    
    // Get character name
    static std::string getCharacterName(Character c);
    
    // Get character quote style
    static std::string getCharacterGreeting(Character c);
    
    // Get all tasks count
    int getTaskCount() const {
        return static_cast<int>(tasks.size());
    }


private:
    std::vector<Task> tasks;
    std::map<int, Dialog> dialogs;
    
    void initializeDefaultTasks(); // Initialize with default Breaking Bad themed tasks
    void initializeDefaultDialogs();
};

#endif // DIALOGSYSTEM_H


