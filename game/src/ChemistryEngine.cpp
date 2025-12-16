#include "ChemistryEngine.h"
#include <regex>
#include <sstream>
#include <algorithm>
#include <cctype>

// Atomic masses database (common elements)
const std::map<std::string, double> ChemistryEngine::ATOMIC_MASSES = {
    {"H", 1.008},   {"He", 4.003},  {"Li", 6.941},  {"Be", 9.012},
    {"B", 10.81},   {"C", 12.01},   {"N", 14.01},   {"O", 16.00},
    {"F", 19.00},   {"Ne", 20.18},  {"Na", 22.99},  {"Mg", 24.31},
    {"Al", 27.98},  {"Si", 28.09},  {"P", 30.97},   {"S", 32.07},
    {"Cl", 35.45},  {"Ar", 39.95},  {"K", 39.10},   {"Ca", 40.08},
    {"Fe", 55.85},  {"Cu", 63.55},  {"Zn", 65.38},  {"Br", 79.90},
    {"I", 126.90},  {"Pb", 207.2}
};

// ChemicalFormula implementation
ChemistryEngine::ChemicalFormula::ChemicalFormula(const std::string& formula)
    : originalFormula(formula) {
    elements = ChemistryEngine::parseFormula(formula).elements;
}

double ChemistryEngine::ChemicalFormula::calculateMolarMass() const {
    return ChemistryEngine::calculateMolarMass(*this);
}

std::string ChemistryEngine::ChemicalFormula::toString() const {
    return originalFormula;
}

// Parse chemical formula (handles basic formulas like H2O, CH4, C10H15N)
ChemistryEngine::ChemicalFormula ChemistryEngine::parseFormula(const std::string& formula) {
    ChemicalFormula result;
    result.originalFormula = formula;
    
    // Remove spaces and convert to standard format
    std::string cleanFormula = formula;
    cleanFormula.erase(std::remove_if(cleanFormula.begin(), cleanFormula.end(), ::isspace), cleanFormula.end());
    
    size_t pos = 0;
    parseFormulaRecursive(cleanFormula, pos, result.elements, 1);
    
    return result;
}

void ChemistryEngine::parseFormulaRecursive(const std::string& formula, 
                                           size_t& pos, 
                                           std::map<std::string, int>& elements,
                                           int multiplier) {
    while (pos < formula.length()) {
        if (formula[pos] == '(') {
            // Handle parentheses (e.g., (NH4)2SO4)
            pos++; // skip '('
            std::map<std::string, int> subElements;
            parseFormulaRecursive(formula, pos, subElements, 1);
            
            if (pos < formula.length() && formula[pos] == ')') {
                pos++; // skip ')'
                int subMultiplier = readNumber(formula, pos);
                if (subMultiplier == 0) subMultiplier = 1;
                
                // Multiply sub-elements by multiplier
                for (auto& elem : subElements) {
                    elements[elem.first] += elem.second * subMultiplier * multiplier;
                }
            }
        } else if (std::isupper(formula[pos])) {
            // Read element symbol
            std::string symbol = readElementSymbol(formula, pos);
            int count = readNumber(formula, pos);
            if (count == 0) count = 1;
            
            elements[symbol] += count * multiplier;
        } else {
            break;
        }
    }
}

std::string ChemistryEngine::readElementSymbol(const std::string& formula, size_t& pos) {
    std::string symbol;
    if (pos < formula.length()) {
        symbol += formula[pos++];
        // Check if next character is lowercase (two-letter elements)
        if (pos < formula.length() && std::islower(formula[pos])) {
            symbol += formula[pos++];
        }
    }
    return symbol;
}

int ChemistryEngine::readNumber(const std::string& formula, size_t& pos) {
    int number = 0;
    while (pos < formula.length() && std::isdigit(formula[pos])) {
        number = number * 10 + (formula[pos] - '0');
        pos++;
    }
    return number;
}

// Calculate molar mass
double ChemistryEngine::calculateMolarMass(const std::string& formula) {
    ChemicalFormula parsed = parseFormula(formula);
    return calculateMolarMass(parsed);
}

double ChemistryEngine::calculateMolarMass(const ChemicalFormula& formula) {
    double totalMass = 0.0;
    
    for (const auto& elem : formula.elements) {
        auto it = ATOMIC_MASSES.find(elem.first);
        if (it != ATOMIC_MASSES.end()) {
            totalMass += it->second * elem.second;
        }
    }
    
    return totalMass;
}

// Moles <-> grams conversion
double ChemistryEngine::molesToGrams(double moles, const std::string& formula) {
    double molarMass = calculateMolarMass(formula);
    return moles * molarMass;
}

double ChemistryEngine::gramsToMoles(double grams, const std::string& formula) {
    double molarMass = calculateMolarMass(formula);
    if (molarMass == 0.0) return 0.0;
    return grams / molarMass;
}

// Simplified equation balancing (works for basic equations)
bool ChemistryEngine::balanceEquation(ChemicalEquation& equation) {
    // Collect all unique elements
    std::map<std::string, int> elementCounts;
    
    // Count elements in products (with positive sign)
    for (const auto& comp : equation.products) {
        for (const auto& elem : comp.formula.elements) {
            elementCounts[elem.first] += elem.second * comp.coefficient;
        }
    }
    
    // Count elements in reactants (with negative sign)
    for (const auto& comp : equation.reactants) {
        for (const auto& elem : comp.formula.elements) {
            elementCounts[elem.first] -= elem.second * comp.coefficient;
        }
    }
    
    // Check if all counts are zero (balanced)
    for (const auto& elem : elementCounts) {
        if (elem.second != 0) {
            return false;
        }
    }
    
    return true;
}

// Stoichiometry: calculate product yield
double ChemistryEngine::calculateProductYield(const std::string& reactantFormula,
                                              double reactantMoles,
                                              const std::string& productFormula,
                                              int reactantCoeff,
                                              int productCoeff) {
    // Using stoichiometric ratio: (reactantMoles / reactantCoeff) * (productCoeff / 1) = productMoles
    double productMoles = (reactantMoles / reactantCoeff) * productCoeff;
    return molesToGrams(productMoles, productFormula);
}

// ChemicalEquation toString
std::string ChemistryEngine::ChemicalEquation::toString() const {
    std::stringstream ss;
    
    // Reactants
    for (size_t i = 0; i < reactants.size(); ++i) {
        if (reactants[i].coefficient > 1) {
            ss << reactants[i].coefficient;
        }
        ss << reactants[i].formula.toString();
        if (i < reactants.size() - 1) {
            ss << " + ";
        }
    }
    
    ss << " -> ";
    
    // Products
    for (size_t i = 0; i < products.size(); ++i) {
        if (products[i].coefficient > 1) {
            ss << products[i].coefficient;
        }
        ss << products[i].formula.toString();
        if (i < products.size() - 1) {
            ss << " + ";
        }
    }
    
    return ss.str();
}

bool ChemistryEngine::ChemicalEquation::isBalanced() const {
    ChemicalEquation eq = *this;
    return ChemistryEngine::balanceEquation(eq);
}


