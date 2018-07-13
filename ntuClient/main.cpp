#include "widget.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    Widget w;
    w.setFixedSize(290,400);//设置窗体大小
    w.show();

    return a.exec();
}
