#include <QApplication>
#include <QDir>
#include "MainController.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    
    
    
    
    MainController controller;
    controller.initialize();
    
    return app.exec();
}