#ifndef ANIMATEDSWITCH_H
#define ANIMATEDSWITCH_H

#include <QWidget>

class QState;
class QStateMachine;

namespace presentation{
namespace fridge{

class GaletteSwitch;

class AnimatedSwitch : public QWidget
{
    Q_OBJECT
public:
    explicit AnimatedSwitch(Qt::Orientation orientation = Qt::Horizontal,
                            QPoint position = QPoint(),
                            QWidget *parent = 0);
    Qt::Orientation orientation();
public slots:
    void trigger();
    void setOrientation(Qt::Orientation orientation);

signals:
    void pressed(QPoint);
    void triggerState();
    void triggered(QPoint);

private slots:
    void onTransitionFinished();

private:
    void initComponents();
    void initStateMachine();

    GaletteSwitch* pItem;
    QStateMachine* pMachine;
    QState *pHState;
    QState *pVState;

    const QPoint mPosition;
    bool mRotates = false;

};

}

}
#endif // ANIMATEDSWITCH_H
