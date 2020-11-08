#include "mainscene.h"

#include <QApplication>
#include<QDebug>
//int bbb=666;
int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainScene w;
    w.show();
    return a.exec();
}
