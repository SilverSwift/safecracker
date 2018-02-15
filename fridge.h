#ifndef FRIDGE_H
#define FRIDGE_H

#include <QStack>
#include <QVector>
#include <QWidget>

class QGridLayout;
class QAbstractState;
class QState;
class QStateMachine;

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
    bool isLocked() const;
    void link(QAbstractState* state);

    void onFinished();
    void initStateMachine();
    AnimatedSwitch* item(QPoint point) const;
    void lock(bool locked = true);
    void press(QPoint point);
    bool triggerColumn(QPoint position);
    bool triggerRow(QPoint position);
    bool tryTrigger(QPoint point);
    bool tryTrigger(int row, int column);

    void waveLeft(QState* parent);
    void waveDown(QState* parent);
    void waveRight(QState* parent);
    void waveUp(QState* parent);

    QGridLayout* grid;
    QWidget *panel;
    QVector< QVector<AnimatedSwitch*> > mItems;
    QPoint mPressedPosition;
    QStack <QPoint> mLastActions;
    QStack <QPoint> mCanceledActions;

    bool mNeedToCheck = false;

    QStateMachine *pMachine;


};

}
#endif // FRIDGE_H
