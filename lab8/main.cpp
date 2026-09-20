#include "mainwindow.h"
#include <QApplication>
#include "../common/qt/test_runtime.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    MainWindow window;
    window.show();
    scheduleAutomatedExit();
    return app.exec();
}
