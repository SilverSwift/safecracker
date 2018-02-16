#include "animatedswitch.h"
#include "common_traits.h"
#include "fridge.h"
#include "settingsholder.h"

#include <QFinalState>
#include <QGridLayout>
#include <QPainter>
#include <QPaintEvent>
#include <QPixmap>
#include <QPushButton>
#include <QState>
#include <QStateMachine>

#include <ctime>
#include <cstdlib>

using namespace presentation::fridge;

Fridge::Fridge(QWidget *parent) : QWidget(parent)
  , pPanel(new QWidget(this))
  , pGgrid (new QGridLayout(pPanel))
  , pMachine(nullptr)
  , pRedoBtn (new QPushButton(this))
  , pUndoBtn (new QPushButton(this))
  , pRestartBtn (new QPushButton(tr("Restart"),this))
{
    std::srand(unsigned(std::time(0)));

    pUndoBtn->setObjectName("undo");
    pRedoBtn->setObjectName("redo");
    pRestartBtn->setObjectName("restart");

    connect(this, &Fridge::canRedoChanged, pRedoBtn, &QPushButton::setVisible);
    connect(this, &Fridge::canUndoChanged, pUndoBtn, &QPushButton::setVisible);
    connect(pRedoBtn, &QPushButton::clicked, this, &Fridge::redo);
    connect(pUndoBtn, &QPushButton::clicked, this, &Fridge::undo);
    connect(pRestartBtn, &QPushButton::clicked, this, &Fridge::initialize);

    this->setStyleSheet("QPushButton{"
                        ""
                        "   background-color: transparent;"
                        "   background-repeat: no-repeat;"
                        "   border-radius: 15px;"
                        "   border: none;"
                        "}"
                        "QPushButton:hover{"
                        "   background-color: #55f96b01;"
                        "}"
                        "QPushButton:pressed{"
                        "   background-color: #aaf96b01;"
                        "}"
                        ""
                        "QPushButton#undo{"
                        "   border-image: url(:/icons/undo.png) 0 0 0 0 stretch stretch;"
                        "}"
                        "QPushButton#redo{"
                        "   border-image: url(:/icons/redo.png) 0 0 0 0 stretch stretch;"
                        "}"
                        "QPushButton#restart{"
                        "   color: white;"
                        "   font: bold italic large;"
                        "   font-size: 16px;"
                        "   border-image: url(:/icons/textbutton.png) 0 0 0 0 stretch stretch;"
                        "}"
                        "QPushButton:hover#restart{"
                        "   background-color: #aa3a443b;"
                        "}"
                        "QPushButton:pressed#restart{"
                        "   background-color: #ff3a443b;"
                        "}");

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

    QPixmap pixmap(":/icons/fridge.png");
    painter.drawPixmap(this->rect(), pixmap);

    QWidget::paintEvent(event);
}

void Fridge::resizeEvent(QResizeEvent *event)
{
    int w = this->width();
    int h = this->height();

    QRect safeRect(w*0.275, h*0.15, w*0.375, h*0.7);

    pPanel->setGeometry(safeRect);

    int buttonWidth = 60;
    int buttonHeight = 3*buttonWidth/4;

    pUndoBtn->setGeometry(w - 2.2*buttonWidth, 0.05*h,buttonWidth, buttonHeight);
    pRedoBtn->setGeometry(w - 1.1*buttonWidth, 0.05*h, buttonWidth,buttonHeight);

    pRestartBtn->setGeometry(w - 2.2*buttonWidth, 0.05*h + 1.1*buttonHeight,
                             2.1*buttonWidth, 0.9*buttonHeight);

    QWidget::resizeEvent(event);
}

void Fridge::checkState()
{
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

void Fridge::makeTurn()
{
    if (pMachine && pMachine->isRunning())
        return;

    if (!this->item(mPressedPosition))
        return;

    if (pMachine){
        pMachine->stop();
        pMachine->deleteLater();
    }


    pMachine = new QStateMachine(this);
    connect(pMachine, &QStateMachine::finished, this, &Fridge::checkState);

    QState* triggerPressedItem = new QState(pMachine);

    connect(triggerPressedItem, &QState::entered,
            this->item(mPressedPosition), &AnimatedSwitch::trigger);

    QState* triggerCascade = new QState(pMachine);
    triggerCascade->setChildMode(QState::ParallelStates);

    int row = mPressedPosition.x();
    int column = mPressedPosition.y();

    QVector<QPoint> left, down, right, up;

    for (int i = column - 1; i >= 0; i--)
        left<<QPoint(row,i);

    for (int i = row + 1; i < mItems.size(); i++)
        down<<QPoint(i, column);

    for (int i = column + 1; i < mItems.size(); i++)
        right<<QPoint(row,i);

    for (int i = row - 1; i >= 0; i--)
        up<<QPoint(i, column);


    this->makeWave(left, triggerCascade);
    this->makeWave(down, triggerCascade);
    this->makeWave(right, triggerCascade);
    this->makeWave(up, triggerCascade);

    QFinalState* finishTurn = new QFinalState(pMachine);

    triggerPressedItem->addTransition(this->item(mPressedPosition),
                                      &AnimatedSwitch::triggered,  triggerCascade);
    triggerCascade->addTransition(triggerCascade, &QState::finished, finishTurn);

    pMachine->setInitialState(triggerPressedItem);
    pMachine->start();

}

AnimatedSwitch*Fridge::item(QPoint point) const
{
    if (point.x() >= 0 && point.y() >= 0 &&
        point.x() < mItems.size() &&
        point.y() < mItems[point.x()].size())
        return mItems[point.x()][point.y()];
    return nullptr;
}

AnimatedSwitch *Fridge::item(int row, int column) const
{
    return this->item(QPoint(row, column));
}

void Fridge::initialize()
{
    if (this->isLocked())
        return;

    mCanceledActions.clear();
    mLastActions.clear();

    emit canRedoChanged(this->canRedo());
    emit canUndoChanged(this->canUndo());

    foreach (QVector<AnimatedSwitch*> row, mItems) {
        qDeleteAll(row);
        row.clear();
    }

    int size = settings->fieldSize();
    mItems.fill(QVector<AnimatedSwitch*>(size), size);

    for (int row = 0 ; row < size; row++){
        for(int column = 0; column < size; column++){
            Qt::Orientation orientation =
                    (std::rand() % 2 ) ? Qt::Vertical : Qt::Horizontal;

            AnimatedSwitch* item =
                    new fridge::AnimatedSwitch(orientation, QPoint(row, column));

            pGgrid->addWidget(item, row, column);
            mItems[row][column] = item;

            connect(item, &AnimatedSwitch::pressed,
                    this, &Fridge::onPressed, Qt::DirectConnection);
        }
    }

    emit started();
}

void Fridge::press(QPoint position)
{
    mPressedPosition = position;
    this->makeTurn();

    emit pressed();
}

void Fridge::makeWave(QVector<QPoint> points, QState *parent)
{
    QState *waveRightState = new QState(parent);

    QState *state = new QState(waveRightState);
    waveRightState->setInitialState(state);

    foreach (QPoint point, points) {
        AnimatedSwitch* item = this->item(point.x(), point.y());
        if (!item)
            continue;
        connect(state, &QState::entered, item, &AnimatedSwitch::trigger);
        QState* nextState = new QState(waveRightState);
        state->addTransition(item, &AnimatedSwitch::triggered, nextState);
        state = nextState;
    }
    QFinalState* final = new QFinalState(waveRightState);
    state->addTransition(final);
}

