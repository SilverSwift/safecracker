TEMPLATE = app
TARGET = safecracker

QMAKE_CXXFLAGS += -std=c++14

QT += \
    core \
    widgets

SOURCES += \
    main.cpp \
    mainwidget.cpp \
    fridge.cpp \
    galetteswitch.cpp \
    animatedswitch.cpp \
    menubar.cpp \
    stats.cpp \
    startgamedialog.cpp \
    ratingmodel.cpp \
    ratingdialog.cpp

HEADERS += \
    mainwidget.h \
    fridge.h \
    galetteswitch.h \
    animatedswitch.h \
    presentation_traits.h \
    menubar.h \
    stats.h \
    startgamedialog.h \
    ratingmodel.h \
    ratingdialog.h

RESOURCES += \
    main.qrc
