#include "fridge.h"
#include "animatedswitch.h"
#include "common_traits.h"
#include "settingsholder.h"

#include <QDebug>
#include <QFinalState>
#include <QGridLayout>
#include <QPainter>
#include <QPaintEvent>
#include <QPixmap>
#include <QState>
#include <QStateMachine>
#include <QTimer>

#include <ctime>
#include <cstdlib>

using namespace fridge;

Fridge::Fridge(QWidget *parent) : QWidget(parent)
  , grid (new QGridLayout())
  , panel(new QWidget(this))
  , pMachine(nullptr)
{
    grid->setHorizontalSpacing(0);
    grid->setVerticalSpacing(0);
    panel->setLayout(grid);
    std::srand(unsigned(std::time(0)));

}

bool Fridge::canRedo() const
{
    return (!mCanceledActions.isEmpty() && !this->isLocked());
}

bool Fridge::canUndo() const
{
    return (!mLastActions.isEmpty() && !this->isLocked());
}

void Fridge::onPressed(QPoint position)
{
    if (this->isLocked())
        return;
    this->press(position);

    mLastActions<<position;
    mCanceledActions.clear();

    emit canRedoChanged(this->canRedo());
    emit canUndoChanged(this->canUndo());
}

void Fridge::onTriggered(QPoint position)
{

    bool triggered = this->triggerRow(position);
    triggered |= this->triggerColumn(position);

    this->lock(triggered);
}

void Fridge::redo()
{
    if (!this->canRedo())
        return;

    QPoint point = mCanceledActions.pop();
    mLastActions.push(point);
    this->press(point);

    emit canRedoChanged(this->canRedo());
    emit canUndoChanged(this->canUndo());
}

void Fridge::undo()
{
    if (!this->canUndo())
        return;

    QPoint point = mLastActions.pop();
    mCanceledActions.push(point);
    this->press(point);

    emit canRedoChanged(this->canRedo());
    emit canUndoChanged(this->canUndo());
}

void Fridge::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);

    painter.setRenderHints(QPainter::Antialiasing |
                            QPainter::TextAntialiasing |
                            QPainter::SmoothPixmapTransform);

    QPixmap pixmap(":/icons/fridge3.png");
    painter.drawPixmap(this->rect(), pixmap);

    QWidget::paintEvent(event);
}

void Fridge::resizeEvent(QResizeEvent *event)
{
    int w = this->width();
    int h = this->height();

    QRect rect(w*0.275, h*0.15,
               w*0.375, h*0.7);

    panel->setGeometry(rect);

    QWidget::resizeEvent(event);
}

void Fridge::checkState()
{
    if (!mNeedToCheck)
        return;

    mNeedToCheck = false;

    if (this->isLocked())
        return;

    foreach (QVector<AnimatedSwitch*> row, mItems) {
        foreach (AnimatedSwitch* item, row){
            if (item->orientation() != Qt::Horizontal)
                return;
        }
    }

    emit finished();
}

bool Fridge::isLocked() const
{
    if (pMachine)
        return pMachine->isRunning();
    return false;
}

void Fridge::link(QAbstractState* state)
{
    connect(state, &QAbstractState::entered, this, [state](){qDebug()<<"in"<<state->objectName();});
    connect(state, &QAbstractState::exited, this, [state](){qDebug()<<"out"<<state->objectName();});
}

void Fridge::onFinished()
{
    qDebug()<<"done";
}

void Fridge::initStateMachine()
{
    if (pMachine && pMachine->isRunning())
        return;

    if (pMachine){
        pMachine->stop();
        pMachine->deleteLater();
    }

    if (!this->item(mPressedPosition))
        return;

    pMachine = new QStateMachine(this);
    connect(pMachine, &QStateMachine::finished, this, &Fridge::onFinished);

    QState* s1 = new QState(pMachine);

    connect(s1, &QState::entered, this->item(mPressedPosition), &AnimatedSwitch::trigger);
    s1->setObjectName("s1");
    this->link(s1);

    QState* s2 = new QState(pMachine);
    s2->setObjectName("s2");
    this->link(s2);
    this->waveLeft(s2);




    QFinalState* s3 = new QFinalState(pMachine);
    s3->setObjectName("s3");
    this->link(s3);

    s1->addTransition(this->item(mPressedPosition), &AnimatedSwitch::triggered,  s2);
    s2->addTransition(s3);
    pMachine->setInitialState(s1);


    pMachine->start();

}

AnimatedSwitch*Fridge::item(QPoint point) const
{
    if (point.x() > 0 && point.y() > 0 &&
        point.x() < mItems.size() &&
        point.y() < mItems[point.x()].size())
        return mItems[point.x()][point.y()];
    return nullptr;
}

void Fridge::initialize(int size)
{
    mCanceledActions.clear();
    mLastActions.clear();
    emit canRedoChanged(this->canRedo());
    emit canUndoChanged(this->canUndo());

    foreach (QVector<AnimatedSwitch*> row, mItems) {
        qDeleteAll(row);
        row.clear();
    }

    mItems.clear();

    mItems.fill(QVector<AnimatedSwitch*>(size), size);

    for (int row = 0 ; row < size; row++){
        for(int column = 0; column < size; column++){
            Qt::Orientation orientation =
                    (std::rand() % 2 ) ? Qt::Vertical : Qt::Horizontal;

            AnimatedSwitch* item =
                    new fridge::AnimatedSwitch(orientation, QPoint(row, column));

            grid->addWidget(item, row, column);
            mItems[row][column] = item;

//            connect(item, &AnimatedSwitch::triggered,
//                    this, &Fridge::onTriggered);

            connect(item, &AnimatedSwitch::pressed,
                    this, &Fridge::onPressed, Qt::DirectConnection);
        }
    }

    emit started();

}

void Fridge::lock(bool locked)
{
//    if (!locked && mLocked){
//        mNeedToCheck = true;
//        QTimer::singleShot(domain::SettingsHolder::instance()->duration(), this, &Fridge::checkState);
//    }

    emit canRedoChanged(this->canRedo());
    emit canUndoChanged(this->canUndo());

//    foreach (QVector<AnimatedSwitch*> row, mItems) {
//        foreach (AnimatedSwitch* item, row)
//            item->lock(locked);
//    }
}

void Fridge::press(QPoint position)
{
    qDebug()<<Q_FUNC_INFO;

    mPressedPosition = position;
    this->initStateMachine();
    this->lock();
//    this->tryTrigger(position);
    emit pressed();
}

bool Fridge::triggerColumn(QPoint position)
{
    int column = position.y();
    int pressedColumn = mPressedPosition.y();

    if (column != pressedColumn)
        return false;

    int row = position.x();
    int pressedRow = mPressedPosition.x();

    bool triggered = false;

    if (row == pressedRow){
        triggered |= this->tryTrigger(row + 1, column);
        triggered |= this->tryTrigger(row - 1, column);

    }
    else if(row < pressedRow)
        triggered |= this->tryTrigger(row - 1, column);
    else
        triggered |= this->tryTrigger(row + 1, column);

    return triggered;
}

bool Fridge::triggerRow(QPoint position)
{

    int row = position.x();
    int pressedRow = mPressedPosition.x();
    if (row != pressedRow)
        return false;

    int column = position.y();
    int pressedColumn = mPressedPosition.y();

    bool triggered = false;
    if (column == pressedColumn){
        triggered |= this->tryTrigger(row, column + 1);
        triggered |= this->tryTrigger(row, column - 1);

    }
    else if(column < pressedColumn)
        triggered |= this->tryTrigger(row, column - 1);
    else
        triggered |= this->tryTrigger(row, column + 1);

    return triggered;
}

bool Fridge::tryTrigger(QPoint point)
{
    return this->tryTrigger(point.x(), point.y());
}

bool Fridge::tryTrigger(int row, int column)
{
    if (row < 0 || column < 0 ||
        row >= mItems.size() ||
        column >= mItems.at(row).size())
        return false;

    mItems[row][column]->trigger();

    return true;
}

void Fridge::waveLeft(QState* parent)
{
    int row = mPressedPosition.x();
    int column = mPressedPosition.y();

    QState *state = new QState(parent);
    for (int i = column - 1; i >= 0; i++){

    }

    QState *s1 = new QState(parent);
    s1->setObjectName("ls1");
    this->link(s1);

    QState *s2 = new QState(parent);
    s2->setObjectName("ls2");
    this->link(s2);

    QFinalState *s3 = new QFinalState(parent);
    s3->setObjectName("ls3");
    this->link(s3);

    parent->setInitialState(s1);
    s1->addTransition(s2);
    s2->addTransition(s3);
}

void Fridge::waveDown(QState* parent)
{

}

void Fridge::waveRight(QState* parent)
{

}

void Fridge::waveUp(QState* parent)
{

}

