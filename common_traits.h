#ifndef PRESENTATION_TRAITS_H
#define PRESENTATION_TRAITS_H

#include <QDir>
#include <QFileInfo>
#include <QObject>
#include <QString>
#include <QStandardPaths>


namespace presentation{
    namespace fridge{
        const int hRotation = 0;
        const int vRotation = -90;
    }
    const int minSize = 4;
    const int maxSize = 10;
    const QString intro = QObject::tr("Press Game->New game to start");

    const QString lastPlayerKey ("lastPlayerKey");
}

namespace domain{

    enum Columns{
        UserName = 0,
        RoundTime,
        Points,
        DateTime
    };


    const int defaultDuration = 500;
    const QString usersArrayKey ("Users");
    const QString durationKey ("Duration");
    const QString appData = QStandardPaths::writableLocation(QStandardPaths::AppDataLocation);
    const QFileInfo appDataFile(QDir(appData), "data.json");
    const QString ratingFilePath(appDataFile.absoluteFilePath());
}

#endif // PRESENTATION_TRAITS_H
