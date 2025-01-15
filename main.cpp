#include <QApplication>
#include "ui/main_window.h"

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);
    app.setQuitOnLastWindowClosed(false);

    MainWindow mainWindow;

    return app.exec();
}