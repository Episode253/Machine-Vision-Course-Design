#include "mainwindow.h"

#include <QApplication>
#include "../common/qt/test_runtime.h"

// 程序入口：主线程创建 QApplication 与主窗口并进入事件循环。
// lab5 不使用摄像头，也没有工作线程——状态机、刺激计时与绘制全部在 GUI 线程完成。
int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.show();
    // 仅当环境变量 VISUAL_LAB_AUTO_EXIT_MS 有合法值时才安排自动退出（无头验收用），常规运行是空操作。
    scheduleAutomatedExit();
    return a.exec();
}
