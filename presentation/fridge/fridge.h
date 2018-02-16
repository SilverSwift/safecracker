#ifndef FRIDGE_H
#define FRIDGE_H

#include <QStack>
#include <QVector>
#include <QWidget>

class QGridLayout;
class QPushButton;
class QState;
class QStateMachine;

namespace presentation{

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
    void initialize();
    void onPressed(QPoint position);
    void redo();
    void undo();

protected:
    virtual void paintEvent(QPaintEvent* event) override;
    virtual void resizeEvent(QResizeEvent *event) override;

private slots:
    void checkState();

private:
    bool isLocked() const;
    void makeTurn();

    AnimatedSwitch* item(QPoint point) const;
    AnimatedSwitch* item(int row, int column) const;
    void press(QPoint point);

    void makeWave(QVector<QPoint> points, QState* parent);

    QWidget *pPanel;
    QGridLayout* pGgrid;
    QVector< QVector<AnimatedSwitch*> > mItems;
    QPoint mPressedPosition;
    QStack <QPoint> mLastActions;
    QStack <QPoint> mCanceledActions;
    QStateMachine *pMachine;

    QPushButton *pRedoBtn;
    QPushButton *pUndoBtn;
    QPushButton *pRestartBtn;

};

}

}
#endif // FRIDGE_H
