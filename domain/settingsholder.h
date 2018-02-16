#ifndef SETTINGSHOLDER_H
#define SETTINGSHOLDER_H

#include <QObject>

#define settings domain::SettingsHolder::instance()

namespace domain{

class SettingsHolder : public QObject
{
    Q_OBJECT
public:
    explicit SettingsHolder(QObject* parent = nullptr);
    static SettingsHolder* instance();

    Q_PROPERTY(int duration READ duration WRITE setDuration NOTIFY durationChanged)
    Q_PROPERTY(int fieldSize READ fieldSize WRITE setFieldSize NOTIFY fieldSizeChanged)
    Q_PROPERTY(QString userName READ userName WRITE setUserName NOTIFY userNameChanged)

    int fieldSize();
    int duration();
    QString userName();

public slots:
    void setDuration(int);
    void setFieldSize(int);
    void setUserName(QString);

signals:
    void durationChanged(int);
    void fieldSizeChanged(int);
    void userNameChanged(QString);

private:
    int mDuration;
};

}


#endif // SETTINGSHOLDER_H
