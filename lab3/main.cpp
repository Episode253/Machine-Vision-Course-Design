#include <QApplication>

#include "mainwindow.h"

// lab3 入口：创建 QApplication 与主窗口后进入事件循环。QWidget 只能在主线程创建，界面对象都在这里诞生。
int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    MainWindow window;
    window.show();
    // window 是栈对象，先于 app 析构，其 ViewModel 析构时会 stop() 并 join 工作线程。
    return app.exec();
}
