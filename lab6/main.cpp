#include "mainwindow.h"

#include <QApplication>
#include "../common/qt/test_runtime.h"

// 入口：建窗口、显示、进入事件循环。摄像头等设备在界面里点了才开始工作。
int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.show();
    // 无头自动测试钩子：设了 VISUAL_LAB_AUTO_EXIT_MS 时到点自动退出，正常运行不生效。
    scheduleAutomatedExit();
    return a.exec();
}
