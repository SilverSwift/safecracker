#include <QApplication>

#include "mainwidget.h"

int main(int argc, char** argv)
{
    QApplication app(argc, argv);

    QApplication::setApplicationName("safecracker");
    QApplication::setOrganizationDomain("alberts.com");
    QApplication::setOrganizationName("Alberts");

    presentation::MainWidget wgt;
    wgt.resize(300, 300);
    wgt.show();

    app.exec();
}
