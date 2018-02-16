#include <QApplication>
#include <QTranslator>

#include "mainwindow.h"

int main(int argc, char** argv)
{
    QApplication app(argc, argv);

    QApplication::setApplicationName("safecracker");
    QApplication::setOrganizationDomain("alberts.com");
    QApplication::setOrganizationName("Alberts");

    QTranslator translator;
    translator.load(":/translate_rus.qm");
    app.installTranslator(&translator);

    presentation::MainWindow wgt;
    wgt.show();

    app.exec();
}
