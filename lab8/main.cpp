#include "mainwindow.h"
#include <QApplication>
#include "../common/qt/test_runtime.h"

// 程序入口，跑在主（GUI）线程：建 QApplication → 建主窗口并显示 → 注册自动退出钩子 → 进入事件循环。
int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    MainWindow window;
    window.show();
    // 读环境变量 VISUAL_LAB_AUTO_EXIT_MS，设定时后自动退出，便于无头自动化验收；未设置时不生效。
    scheduleAutomatedExit();
    return app.exec();
}
