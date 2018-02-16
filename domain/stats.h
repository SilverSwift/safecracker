#ifndef STATS_H
#define STATS_H

#include <QObject>
#include <QTime>

namespace domain{

class Stats : public QObject
{
    Q_OBJECT
public:
    explicit Stats(QObject *parent = 0);

    int points() const;
    int gameTime() const;
    QString result();
    QString statusInfo();

signals:
    void updatePoints();

public slots:
    void onFinished();
    void onPressed();
    void onStarted();


private:
    QTime mTime;
    int mGameTime;
    int mTurns;
    const int mMaxPoints = 5000;
    bool mStarted = false;
};

}

#endif // STATS_H
