#include <QApplication>
#include <QtGui>
#include "mainframe.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    MainFrame mf;
    mf.show();

    return app.exec();
}
