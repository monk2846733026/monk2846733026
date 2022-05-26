#include "widget.h"
#include"config.h"
#include <QApplication>
#include<QResource>//注册二进制res文件
int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    //注册二进制res文件
    QResource::registerResource(GAME_RES_PATH);
    Widget w;
    w.show();
    return a.exec();
}
