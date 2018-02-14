#ifndef PRESENTATION_TRAITS_H
#define PRESENTATION_TRAITS_H

#include <QDir>
#include <QFileInfo>
#include <QObject>
#include <QString>
#include <QStandardPaths>

namespace fridge{
    const int duration = 50;
    const int hRotation = 0;
    const int vRotation = -90;

}

namespace presentation{
    const int minSize = 4;
    const int maxSize = 10;
    const QString intro = QObject::tr("Press Game->New game to start");

    const QString lastPlayerKey ("lastPlayerKey");
}

namespace domain{

    const QString usersArrayKey ("Users");
    const QString appData = QStandardPaths::writableLocation(QStandardPaths::AppDataLocation);
    const QFileInfo appDataFile(QDir(appData), "data.json");
    const QString ratingFilePath(appDataFile.absoluteFilePath());
}

#endif // PRESENTATION_TRAITS_H
