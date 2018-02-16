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
    const int minFieldSize = 4;
    const int maxFieldSize = 10;
}

namespace domain{

    enum Columns{
        UserName = 0,
        RoundTime,
        Points,
        DateTime
    };


    const int defaultDuration = 500;
    const int defaultFieldSize = 4;

    const QString usersArrayKey ("Users");
    const QString durationKey ("Duration");
    const QString fieldSizeKey("fieldSize");
    const QString userNameKey("userName");

    const QString appData = QStandardPaths::writableLocation(QStandardPaths::AppDataLocation);
    const QFileInfo appDataFile(QDir(appData), "data.json");
    const QString ratingFilePath(appDataFile.absoluteFilePath());
}

#endif // PRESENTATION_TRAITS_H
