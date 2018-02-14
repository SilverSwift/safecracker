#include "fridge.h"
#include "animatedswitch.h"
#include "presentation_traits.h"

#include <QGridLayout>
#include <QPainter>
#include <QPaintEvent>
#include <QPixmap>
#include <QTimer>

#include <ctime>
#include <cstdlib>

using namespace fridge;

Fridge::Fridge(QWidget *parent) : QWidget(parent)
  , grid (new QGridLayout())
  , panel(new QWidget(this))
{
    grid->setHorizontalSpacing(0);
    grid->setVerticalSpacing(0);
    panel->setLayout(grid);
    std::srand(unsigned(std::time(0)));

}

bool Fridge::canRedo() const
{
    return (!mCanceledActions.isEmpty() && !mLocked);
}

bool Fridge::canUndo() const
{
    return (!mLastActions.isEmpty() && !mLocked);
}

void Fridge::onPressed(QPoint position)
{
    if (mLocked)
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

    if (mLocked)
        return;

    foreach (QVector<AnimatedSwitch*> row, mItems) {
        foreach (AnimatedSwitch* item, row){
            if (item->orientation() != Qt::Horizontal)
                return;
        }
    }

    emit finished();
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

            connect(item, &AnimatedSwitch::triggered,
                    this, &Fridge::onTriggered);

            connect(item, &AnimatedSwitch::pressed,
                    this, &Fridge::onPressed, Qt::DirectConnection);
        }
    }

    emit started();

}

void Fridge::lock(bool locked)
{
    if (!locked && mLocked){
        mNeedToCheck = true;
        QTimer::singleShot(fridge::duration, this, &Fridge::checkState);
    }

    mLocked = locked;
    this->setEnabled(!locked);
    emit canRedoChanged(this->canRedo());
    emit canUndoChanged(this->canUndo());

    foreach (QVector<AnimatedSwitch*> row, mItems) {
        foreach (AnimatedSwitch* item, row)
            item->lock(locked);
    }
}

void Fridge::press(QPoint position)
{
    this->lock();
    mPressedPosition = position;
    this->tryTrigger(position);
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
