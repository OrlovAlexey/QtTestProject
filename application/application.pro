QT       += core gui sql

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

LIBS += -L/usr/local/opt/libpq/lib

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    groupsdistribution.cpp \
    groupsedit.cpp \
    groupsselection.cpp \
    main.cpp \
    mainwindow.cpp \
    positionsedit.cpp \
    receiversedit.cpp \
    showtable.cpp

HEADERS += \
    groupsdistribution.h \
    groupsedit.h \
    groupsselection.h \
    mainwindow.h \
    positionsedit.h \
    receiversedit.h \
    showtable.h

FORMS += \
    groupsdistribution.ui \
    groupsedit.ui \
    groupsselection.ui \
    mainwindow.ui \
    positionsedit.ui \
    receiversedit.ui \
    showtable.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
