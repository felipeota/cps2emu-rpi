#-------------------------------------------------
#
# Project created by QtCreator 2010-10-22T08:23:53
#
#-------------------------------------------------

QT       += core gui

TARGET = cps2gui
TEMPLATE = app

PREFIX = debian/<application-name>

INSTALLS += target desktop

desktop.path = $$PREFIX/usr/share/applications/hildon-home
desktop.files = *.desktop

target.path = $$PREFIX/opt/<application-name>/

SOURCES += main.cpp\
        cps2emu.cpp

HEADERS  += cps2emu.h

FORMS    += cps2emu.ui

CONFIG += mobility
MOBILITY = 

symbian {
    TARGET.UID3 = 0xe927785c
    # TARGET.CAPABILITY += 
    TARGET.EPOCSTACKSIZE = 0x14000
    TARGET.EPOCHEAPSIZE = 0x020000 0x800000
}

RESOURCES += \
    images.qrc
