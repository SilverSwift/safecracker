#include "animatedswitch.h"
#include "galetteswitch.h"
#include "common_traits.h"
#include "settingsholder.h"

#include <QPropertyAnimation>
#include <QStateMachine>
#include <QSignalTransition>
#include <QVBoxLayout>

using namespace presentation::fridge;

AnimatedSwitch::AnimatedSwitch(Qt::Orientation orientation,
                               QPoint position, QWidget *parent) :
    QWidget(parent)
  , pItem(new GaletteSwitch)
  , pMachine (new QStateMachine(this))
  , pHState  (new QState(pMachine))
  , pVState  (new QState(pMachine))
  , mPosition (position)
{  
    this->initComponents();
    this->initStateMachine();

    connect(pItem, &GaletteSwitch::pressed,
            this, [=](){emit pressed(mPosition);});

    QState* state = (orientation == Qt::Vertical) ? pVState : pHState;
    pMachine->setInitialState(state);
    pMachine->start();
}

Qt::Orientation AnimatedSwitch::orientation()
{
    if (pMachine->configuration().contains(pHState))
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
    layout->addWidget(pItem);
}

void AnimatedSwitch::initStateMachine()
{
    pHState->assignProperty(pItem, "rotation", hRotation);
    pVState->assignProperty(pItem, "rotation", vRotation);

    QPropertyAnimation* animation = new QPropertyAnimation(pItem, "rotation");
    animation->setDuration(domain::SettingsHolder::instance()->duration());

    connect(animation, &QPropertyAnimation::finished,
            this, &AnimatedSwitch::onTransitionFinished);

    QSignalTransition* triggerToVTransition =
            pHState->addTransition(this, SIGNAL(triggerState()), pVState);
    triggerToVTransition->addAnimation(animation);

    QSignalTransition* triggerToHTransition =
            pVState->addTransition(this, SIGNAL(triggerState()), pHState);
    triggerToHTransition->addAnimation(animation);
}
