QT       += core gui multimedia multimediawidgets

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    main.cpp \
    playerwindow.cpp

HEADERS += \
    playerwindow.h

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    res.qrc

DISTFILES += \
    icons/btn_play1.png \
    icons/btn_play2.png \
    icons/btn_screen1.png \
    icons/btn_screen2.png \
    icons/btn_volumedown1.png \
    icons/btn_volumedown2.png \
    icons/btn_volumeup1.png \
    icons/btn_volumeup2.png \
    icons/handle.png \
    icons/set.png \
    icons/set_check.png \
    icons/sort.png \
    icons/sort_check.png
