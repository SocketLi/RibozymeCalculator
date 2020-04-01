#include "mainwindow.h"
#include <QApplication>
#include"login.h"
int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    Login LoginWindow;
    LoginWindow.show();
    LoginWindow.exec();

    MainWindow w;
    w.show();
    return a.exec();
}
