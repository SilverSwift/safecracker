#ifndef ANIMATEDSWITCH_H
#define ANIMATEDSWITCH_H

#include <QWidget>

class QState;
class QStateMachine;

namespace fridge{

class GaletteSwitch;

class AnimatedSwitch : public QWidget
{
    Q_OBJECT
public:
    explicit AnimatedSwitch(Qt::Orientation orientation = Qt::Horizontal, QPoint position = QPoint(),
                            QWidget *parent = 0);
    Qt::Orientation orientation();
public slots:
    void trigger();
    void setOrientation(Qt::Orientation orientation);

signals:
    void lock(bool);
    void pressed(QPoint);
    void triggerState();
    void triggered(QPoint);

private slots:
    void onTransitionFinished();

private:
    void initComponents();
    void initStateMachine();

    GaletteSwitch* item;
    QStateMachine* machine;
    QState *hState;
    QState *vState;
    const QPoint mPosition;
    bool mRotates = false;

};

}
#endif // ANIMATEDSWITCH_H
