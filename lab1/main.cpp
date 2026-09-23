#include "mainwindow.h"

#include <QApplication>
#include "../common/qt/test_runtime.h"

// 程序入口，运行在 GUI 线程：创建应用与主窗口，随后进入 Qt 事件循环。
int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.show();
    // 无头测试钩子：只有设置了环境变量 VISUAL_LAB_AUTO_EXIT_MS 才会定时退出，正常运行时是空操作。
    scheduleAutomatedExit();
    return a.exec();
}
