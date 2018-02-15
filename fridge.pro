TEMPLATE = app
TARGET = safecracker

QMAKE_CXXFLAGS += -std=c++14

QT += \
    core \
    widgets

INCLUDEPATH += \
    presentation \
    presentation/fridge \
    domain


SOURCES += \
    main.cpp \
    presentation/mainwidget.cpp \
    presentation/fridge/fridge.cpp \
    presentation/fridge/galetteswitch.cpp \
    presentation/fridge/animatedswitch.cpp \
    presentation/menubar.cpp \
    domain/stats.cpp \
    presentation/startgamedialog.cpp \
    domain/ratingmodel.cpp \
    presentation/ratingdialog.cpp \
    presentation/settingsdialog.cpp \
    domain\settingsholder.cpp

HEADERS += \
    presentation/mainwidget.h \
    presentation/fridge/fridge.h \
    presentation/fridge/galetteswitch.h \
    presentation/fridge/animatedswitch.h \
    presentation/menubar.h \
    domain/stats.h \
    presentation/startgamedialog.h \
    domain/ratingmodel.h \
    presentation/ratingdialog.h \
    common_traits.h \
    presentation/settingsdialog.h \
    domain/settingsholder.h

RESOURCES += \
    main.qrc
