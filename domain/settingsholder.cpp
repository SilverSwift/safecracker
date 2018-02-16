#include "settingsholder.h"
#include "common_traits.h"

#include <QSettings>

using namespace domain;

Q_GLOBAL_STATIC(SettingsHolder,  ptr)

SettingsHolder::SettingsHolder(QObject* parent) : QObject(parent)
{

}

int SettingsHolder::duration()
{
    return QSettings().value(durationKey, defaultDuration).toInt();
}

QString SettingsHolder::userName()
{
    return QSettings().value(userNameKey).toString();
}

void SettingsHolder::setDuration(int value)
{
    QSettings().setValue(durationKey, value);
    emit durationChanged(value);
}

void SettingsHolder::setFieldSize(int value)
{
    QSettings().setValue(fieldSizeKey, value);
    emit fieldSizeChanged(value);
}

void SettingsHolder::setUserName(QString userName)
{
    QSettings().setValue(userNameKey, userName);
    emit userNameChanged(userName);
}


SettingsHolder* SettingsHolder::instance()
{
    return ptr();
}

int SettingsHolder::fieldSize()
{
    return QSettings().value(fieldSizeKey, defaultFieldSize).toInt();
}
