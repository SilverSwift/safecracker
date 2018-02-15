#include <QApplication>

#include "mainwidget.h"

#include <QState>
#include <QFinalState>
#include <QStateMachine>
#include <QDebug>

//void link(QAbstractState* state)
//{
//    QObject::connect(state, &QState::entered, qApp, [=](){
//        qDebug()<<"entered"<<state->objectName();
//    });

//    QObject::connect(state, &QState::exited, qApp, [=](){
//        qDebug()<<"exited"<<state->objectName();
//    });
//}

int main(int argc, char** argv)
{
    QApplication app(argc, argv);

//    QStateMachine machine;

//    QState s1(&machine);
//    QFinalState s2(&machine);

//    s1.setChildMode(QState::ParallelStates);
//    s1.setObjectName("s1");
//    s1.addTransition(&s2);

//    QState s1u(&s1);
//    QState s1d(&s1);
//    s1u.setObjectName("s1u");
//    s1d.setObjectName("s1d");
//    link(&s1u);
//    link(&s1d);

//    QState s1u1(&s1u);
//    QState s1u2(&s1u);
//    QFinalState s1u3(&s1u);
//    s1u1.setObjectName("s1u1");
//    s1u2.setObjectName("s1u2");
//    s1u3.setObjectName("s1u3");

//    s1u.setInitialState(&s1u1);
//    s1u1.addTransition(&s1u2);
//    s1u2.addTransition(&s1u3);
//    link(&s1u1);
//    link(&s1u2);
//    link(&s1u3);

//    QState s1d1(&s1d);
//    QState s1d2(&s1d);
//    QFinalState s1d3(&s1d);
//    s1d1.setObjectName("s1d1");
//    s1d2.setObjectName("s1d2");
//    s1d3.setObjectName("s1d3");

//    s1d.setInitialState(&s1d1);
//    s1d1.addTransition(&s1d2);
//    s1d2.addTransition(&s1d3);
//    link(&s1d1);
//    link(&s1d2);
//    link(&s1d3);


//    s2.setObjectName("s2");
//    machine.setInitialState(&s1);

//    link(&s1);
//    link(&s2);

//    machine.start();
//    QObject::connect(&machine, SIGNAL(finished()), QApplication::instance(), SLOT(quit()));

    QApplication::setApplicationName("safecracker");
    QApplication::setOrganizationDomain("alberts.com");
    QApplication::setOrganizationName("Alberts");

    presentation::MainWidget wgt;
    wgt.show();

    app.exec();
}
