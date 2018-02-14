#ifndef FRIDGE_H
#define FRIDGE_H

#include <QStack>
#include <QVector>
#include <QWidget>

class QGridLayout;

namespace fridge{

class AnimatedSwitch;

class Fridge : public QWidget
{
    Q_OBJECT
public:
    explicit Fridge(QWidget *parent = 0);

    Q_PROPERTY(bool canRedo READ canRedo NOTIFY canRedoChanged)
    Q_PROPERTY(bool canUndo READ canRedo NOTIFY canUndoChanged)

    bool canRedo() const;
    bool canUndo() const;

signals:
    void canRedoChanged(bool);
    void canUndoChanged(bool);
    void finished() const;
    void pressed();
    void started();

public slots:
    void initialize(int size);
    void onPressed(QPoint position);
    void onTriggered(QPoint position);
    void redo();
    void undo();

protected:
    virtual void paintEvent(QPaintEvent* event) override;
    virtual void resizeEvent(QResizeEvent *event) override;

private slots:
    void checkState();

private:

    void lock(bool locked = true);
    void press(QPoint point);
    bool triggerColumn(QPoint position);
    bool triggerRow(QPoint position);
    bool tryTrigger(QPoint point);
    bool tryTrigger(int row, int column);

    QGridLayout* grid;
    QWidget *panel;
    QVector< QVector<AnimatedSwitch*> > mItems;
    QPoint mPressedPosition;
    QStack <QPoint> mLastActions;
    QStack <QPoint> mCanceledActions;

    bool mNeedToCheck = false;
    bool mLocked = false;


};

}
#endif // FRIDGE_H
