#include "mainwindow.h"

#include <QApplication>
#include "../common/qt/test_runtime.h"

// 程序入口：创建 QApplication 与主窗口，交给事件循环；
// 界面逻辑全在 MainWindow，这里不做任何业务处理。
int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.show();
    // 可选的自动退出钩子，由环境变量 VISUAL_LAB_AUTO_EXIT_MS 控制，
    // 仅用于自动化与无头运行。
    scheduleAutomatedExit();
    return a.exec();
}
