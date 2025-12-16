#include <QApplication>
#include "UI_MainWindow.h"
#include <iostream>

/**
 * @brief Main entry point for Breaking Bonds: A Chemistry Chronicle
 * 
 * Breaking Bonds - Educational chemistry game inspired by Breaking Bad
 * Learn chemistry through interactive problems and Breaking Bad themed dialogues
 */
int main(int argc, char *argv[]) {
    // Set high DPI scaling for better display on modern monitors
    QApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
    QApplication::setAttribute(Qt::AA_UseHighDpiPixmaps);
    
    QApplication app(argc, argv);
    app.setApplicationName("Breaking Bonds");
    app.setApplicationVersion("1.0.0");
    app.setOrganizationName("Breaking Bonds Studio");
    
    // Create and show main window
    UI_MainWindow window;
    window.show();
    
    std::cout << "Breaking Bonds: A Chemistry Chronicle" << std::endl;
    std::cout << "Version 1.0.0" << std::endl;
    std::cout << "Welcome to the lab!" << std::endl;
    
    return app.exec();
}

