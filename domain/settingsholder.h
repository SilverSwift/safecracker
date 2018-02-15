#ifndef SETTINGSHOLDER_H
#define SETTINGSHOLDER_H

#include <QObject>

namespace domain{

class SettingsHolder : public QObject
{
    Q_OBJECT
public:
    static SettingsHolder* instance();

    int duration();

public slots:
    void onDurationChanged(int);

private:
    void load();

    int mDuration;
    bool mLoaded = false;
};

}

#endif // SETTINGSHOLDER_H
