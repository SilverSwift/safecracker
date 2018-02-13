#include "stats.h"

#include <QTimer>

using namespace domain;

Stats::Stats(QObject *parent) : QObject(parent)
{
    QTimer* timer = new QTimer(this);
    timer->start(1000);
    connect(timer, &QTimer::timeout, this, &Stats::updatePoints);
}

void Stats::onFinished()
{
    mGameTime = mTime.elapsed()/1000;
}

void Stats::onPressed()
{
    mTurns++;
}

void Stats::onStarted()
{
    mTime.start();
    mGameTime = 0;
    mTurns = 0;
}

void Stats::setName(QString name)
{
    mName = name;
}

int Stats::points()
{
    int time = mGameTime ? mGameTime : mTime.elapsed()/100;
    return qMax(mMaxPoints - time - mTurns*100, 0);
}

QString Stats::result()
{
    return tr("Congratulations %1 you've finished with %2 points!\r\n"
              "You've did it in %3 seconds!").arg(mName).arg(this->points()).arg(mGameTime);
}

QString Stats::shortResult()
{
    return tr("Points: %1").arg(this->points());
}

