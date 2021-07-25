QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    animation.cpp \
    blood_pack.cpp \
    bomb.cpp \
    boss.cpp \
    bullet.cpp \
    double_bullet.cpp \
    enemy.cpp \
    intro.cpp \
    main.cpp \
    maingame.cpp \
    mainwindow.cpp \
    map.cpp \
    player.cpp \
    record.cpp

HEADERS += \
    animation.h \
    blood_pack.h \
    bomb.h \
    boss.h \
    bullet.h \
    config.h \
    double_bullet.h \
    enemy.h \
    intro.h \
    maingame.h \
    mainwindow.h \
    map.h \
    player.h \
    record.h

FORMS += \
    intro.ui \
    mainwindow.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    resource.qrc

DISTFILES += \
    resource/321.gif
