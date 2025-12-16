#ifndef CHEMISTRYENGINE_H
#define CHEMISTRYENGINE_H

#include <string>
#include <vector>
#include <map>
#include <cmath>

// Forward declarations
struct Element;
struct ChemicalFormula;
struct ChemicalEquation;

/**
 * @brief ChemistryEngine - Core chemistry calculations engine
 * Handles molecular formulas, molar mass calculations, equation balancing, and stoichiometry
 */
class ChemistryEngine {
public:
    // Element data structure
    struct Element {
        std::string symbol;
        double atomicMass;
    };

    // Chemical formula representation
    struct ChemicalFormula {
        std::map<std::string, int> elements; // element symbol -> count
        std::string originalFormula;
        
        ChemicalFormula() = default;
        explicit ChemicalFormula(const std::string& formula);
        double calculateMolarMass() const;
        std::string toString() const;
    };

    // Chemical equation component
    struct EquationComponent {
        ChemicalFormula formula;
        int coefficient;
        
        EquationComponent(const ChemicalFormula& f, int coeff = 1) 
            : formula(f), coefficient(coeff) {}
    };

    // Chemical equation representation
    struct ChemicalEquation {
        std::vector<EquationComponent> reactants;
        std::vector<EquationComponent> products;
        
        bool isBalanced() const;
        std::string toString() const;
    };

    // Atomic masses database (simplified - common elements)
    static const std::map<std::string, double> ATOMIC_MASSES;

    // Formula parsing
    static ChemicalFormula parseFormula(const std::string& formula);
    
    // Molar mass calculation
    static double calculateMolarMass(const std::string& formula);
    static double calculateMolarMass(const ChemicalFormula& formula);
    
    // Moles <-> grams conversion
    static double molesToGrams(double moles, const std::string& formula);
    static double gramsToMoles(double grams, const std::string& formula);
    
    // Equation balancing (simplified algorithm for basic equations)
    static bool balanceEquation(ChemicalEquation& equation);
    
    // Stoichiometry calculations
    static double calculateProductYield(const std::string& reactantFormula, 
                                       double reactantMoles,
                                       const std::string& productFormula,
                                       int reactantCoeff = 1,
                                       int productCoeff = 1);

private:
    // Helper functions for formula parsing
    static void parseFormulaRecursive(const std::string& formula, 
                                     size_t& pos, 
                                     std::map<std::string, int>& elements,
                                     int multiplier = 1);
    static std::string readElementSymbol(const std::string& formula, size_t& pos);
    static int readNumber(const std::string& formula, size_t& pos);
};

#endif // CHEMISTRYENGINE_H


