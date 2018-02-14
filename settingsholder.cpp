#include "settingsholder.h"
#include "common_traits.h"

#include <QDebug>
#include <QSettings>

using namespace domain;

Q_GLOBAL_STATIC(SettingsHolder,  settings)


int SettingsHolder::duration()
{
    return mDuration;
}

SettingsHolder* SettingsHolder::instance()
{
    if (!settings()->mLoaded){
        settings()->load();
        settings()->mLoaded = true;
    }
    return settings();
}

void SettingsHolder::onDurationChanged(int duration)
{
    mDuration = duration;
    QSettings().setValue(durationKey, mDuration);
    QSettings().sync();
}

void SettingsHolder::load()
{
    QSettings settings;
    mDuration = settings.value(durationKey, defaultDuration).toInt();
}

