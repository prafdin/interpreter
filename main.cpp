#include <QApplication>
#include <QWidget>

#include "interpreter_window.h"

int main(int argc, char* argv[]) {

    QApplication app(argc, argv);

    InterpreterWindow window;

    window.resize(900, 600); 
    window.setWindowTitle("Interpreter"); 
    window.show(); 

    return app.exec();
}