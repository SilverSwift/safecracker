#include "settingsholder.h"
#include "stats.h"

#include <QTimer>

using namespace domain;

Stats::Stats(QObject *parent) : QObject(parent)
{
    QTimer* timer = new QTimer(this);
    timer->start(200);
    connect(timer, &QTimer::timeout, this, &Stats::updatePoints);
}

int Stats::gameTime() const
{
    return mGameTime;
}

void Stats::onFinished()
{
    mStarted = false;
    mGameTime = mTime.elapsed()/1000;
}

void Stats::onPressed()
{
    mTurns++;
}

void Stats::onStarted()
{
    mStarted = true;
    mTime.start();
    mGameTime = 0;
    mTurns = 0;
}

int Stats::points() const
{
    int time = mGameTime ? mGameTime : mTime.elapsed()/1000;
    return qMax((mMaxPoints - time - mTurns), 0);
}

QString Stats::result()
{
    return tr("Congratulations %1 you've finished with %2 points!\r\n"
              "You've did it in %3 seconds!").arg(settings->userName()).arg(this->points()).arg(mGameTime);
}

QString Stats::statusInfo()
{
    if (mStarted)
        return tr("Points: %1").arg(this->points());
    return QString();
}

