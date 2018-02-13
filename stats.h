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

    int points();
    QString result();
    QString statusInfo();

signals:
    void updatePoints();

public slots:
    void onFinished();
    void onPressed();
    void onStarted();
    void setName(QString name);

private:
    QTime mTime;
    int mGameTime;
    int mTurns;
    const int mMaxPoints = 10000;
    QString mName;
    bool mStarted = false;
};

}

#endif // STATS_H
