#include "GameWindow.h"
#include <iostream>

/**
 * @brief Main entry point for Breaking Bonds: A Chemistry Chronicle
 * 
 * Breaking Bonds - Educational chemistry game inspired by Breaking Bad
 * Learn chemistry through interactive problems and Breaking Bad themed dialogues
 */
int main() {
    std::cout << "Breaking Bonds: A Chemistry Chronicle" << std::endl;
    std::cout << "Version 1.0.0" << std::endl;
    std::cout << "Welcome to the lab!" << std::endl;
    
    try {
        GameWindow window;
        window.run();
    }
    catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return 1;
    }
    
    return 0;
}
