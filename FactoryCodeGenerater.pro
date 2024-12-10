QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    codegenerateutils.cpp \
    get_uuid.cpp \
    main.cpp \
    mainwidget.cpp \
    numberinput.cpp \
    pathSelect.cpp \
    textinput.cpp

HEADERS += \
    codegenerateutils.h \
    get_uuid.h \
    mainwidget.h \
    numberinput.h \
    pathSelect.h \
    textinput.h

INCLUDEPATH += D:\Boost\boostVS2015\include

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
