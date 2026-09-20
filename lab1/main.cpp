#include "mainwindow.h"

#include <QApplication>
#include "../common/qt/test_runtime.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.show();
    scheduleAutomatedExit();
    return a.exec();
}
