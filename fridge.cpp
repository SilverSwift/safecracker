#include "fridge.h"
#include "animatedswitch.h"

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
    this->press(position);

    mLastActions<<position;
    mCanceledActions.clear();

    emit canRedoChanged(this->canRedo());
    emit canUndoChanged(this->canUndo());
}

void Fridge::onTriggered(QPoint position)
{
    this->triggerRow(position);
    this->triggerColumn(position);
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

    QRect rect(w*0.21, h*0.2,
               w*0.6, h*0.6);

    panel->setGeometry(rect);

    QWidget::resizeEvent(event);
}

void Fridge::checkState() const
{
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
            Qt::Orientation orientation = (std::rand() % 2 ) ?
                        Qt::Vertical :
                        Qt::Horizontal;

            AnimatedSwitch* item = new fridge::AnimatedSwitch(orientation, QPoint(row, column));
            grid->addWidget(item, row, column);
            mItems[row][column] = item;
            connect(item, &AnimatedSwitch::triggered, this, &Fridge::onTriggered);
            connect(item, &AnimatedSwitch::pressed, this, &Fridge::onPressed);
        }
    }

    emit started();

}

void Fridge::lock(bool locked)
{
    mLocked = locked;

    emit canRedoChanged(this->canRedo());
    emit canUndoChanged(this->canUndo());
    foreach (QVector<AnimatedSwitch*> row, mItems) {
        foreach (AnimatedSwitch* item, row)
            emit item->lock(locked);
    }
    if (!locked)
        this->checkState();
}

void Fridge::press(QPoint position)
{
    this->lock();
    mPressedPosition = position;
    this->tryTrigger(position);
    emit pressed();
}

void Fridge::triggerColumn(QPoint position)
{
    int column = position.y();
    int pressedColumn = mPressedPosition.y();

    if (column != pressedColumn)
        return;

    int row = position.x();
    int pressedRow = mPressedPosition.x();

    if (row == pressedRow){
        this->tryTrigger(row + 1, column);
        this->tryTrigger(row - 1, column);

    }
    else if(row < pressedRow)
        this->tryTrigger(row - 1, column);
    else
        this->tryTrigger(row + 1, column);
}

void Fridge::triggerRow(QPoint position)
{

    int row = position.x();
    int pressedRow = mPressedPosition.x();
    if (row != pressedRow)
        return;

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

    if (!triggered)
        this->lock(false);

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
