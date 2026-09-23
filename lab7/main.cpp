#include <QApplication>

#include "mainwindow.h"

// 程序入口，全程只有 Qt 主线程：窗口与 DetectorWorker 都在这里创建，检测工作线程要到点「开始检测」才起。
int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    MainWindow window;
    window.show();
    return app.exec();
}
