#include "animatedswitch.h"
#include "galetteswitch.h"
#include "common_traits.h"
#include "settingsholder.h"

#include <QPropertyAnimation>
#include <QStateMachine>
#include <QSignalTransition>
#include <QVBoxLayout>

using namespace fridge;

AnimatedSwitch::AnimatedSwitch(Qt::Orientation orientation,
                               QPoint position, QWidget *parent) :
    QWidget(parent)
  , item(new GaletteSwitch)
  , machine (new QStateMachine(this))
  , hState  (new QState(machine))
  , vState  (new QState(machine))
  , mPosition (position)
{  
    this->initComponents();
    this->initStateMachine();

    connect(item, &GaletteSwitch::pressed,
            this, [=](){emit pressed(mPosition);});

    QState* state = (orientation == Qt::Vertical) ? vState : hState;
    machine->setInitialState(state);
    machine->start();
}

Qt::Orientation AnimatedSwitch::orientation()
{
    if (machine->configuration().contains(hState))
        return Qt::Horizontal;
    return Qt::Vertical;
}

void AnimatedSwitch::trigger()
{
    if (mRotates)
        return;
    mRotates = true;
    emit triggerState();
}

void AnimatedSwitch::setOrientation(Qt::Orientation orientation)
{
    if (orientation == this->orientation())
        return;
    this->trigger();
}

void AnimatedSwitch::onTransitionFinished()
{
    mRotates = false;
    emit triggered(mPosition);
}

void AnimatedSwitch::initComponents()
{
    QVBoxLayout* layout = new QVBoxLayout(this);
    layout->addWidget(item);
}

void AnimatedSwitch::initStateMachine()
{
    hState->assignProperty(item, "rotation", hRotation);
    vState->assignProperty(item, "rotation", vRotation);

    QPropertyAnimation* animation = new QPropertyAnimation(item, "rotation");
    animation->setDuration(domain::SettingsHolder::instance()->duration());

    connect(animation, &QPropertyAnimation::finished,
            this, &AnimatedSwitch::onTransitionFinished);

    QSignalTransition* triggerToVTransition =
            hState->addTransition(this, SIGNAL(triggerState()), vState);
    triggerToVTransition->addAnimation(animation);

    QSignalTransition* triggerToHTransition =
            vState->addTransition(this, SIGNAL(triggerState()), hState);
    triggerToHTransition->addAnimation(animation);
}
