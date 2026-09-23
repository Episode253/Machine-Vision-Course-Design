#include <QApplication>

#include "mainwindow.h"

// lab4 入口：与 lab3 各自独立成可执行文件；算法与 ViewModel 的源文件由 lab4.pro 从 lab3 目录直接编入。
int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    MainWindow window;
    window.show();
    // window 是栈对象，先于 app 析构，其 ViewModel 析构时会 stop() 并 join 工作线程。
    return app.exec();
}
