QT += dbus

CONFIG += warn_on debug
DEFINES = QT_FATAL_WARNINGS

release {
  CONFIG += warn_on release
  DEFINES = QT_NO_DEBUG_OUTPUT
}

OBJECTS_DIR += build
MOC_DIR += build

isEmpty(PREFIX):PREFIX=/usr/local

TARGET = antico

TRANSLATIONS = \
 language/it_IT.ts \
 language/cs_CZ.ts \
 language/ru_RU.ts \
 language/pl_PL.ts \
 language/de_DE.ts \
 language/es_ES.ts \
 language/pt_BR.ts

SOURCES = \
 main.cpp \
 antico.cpp \
 frame.cpp \
 border.cpp \
 dockbar.cpp \
 launcher.cpp \
 dockicon.cpp \
 desk.cpp \
 header.cpp \
 systray.cpp \
 manager.cpp \
 filedialog.cpp \
 colordialog.cpp \
 colorsel.cpp \
 msgbox.cpp \
 runner.cpp \
 dateclock.cpp \
 sysicon.cpp \
 deskapp.cpp \
 utils.cpp \
 deskdev.cpp \
 deskfile.cpp \
 deskfolder.cpp \
 dockapp.cpp \
 trash.cpp \
 trashdialog.cpp \
 deskicon.cpp \
 dockmenu.cpp \
 settings.cpp

HEADERS = \
 antico.h \
 frame.h \
 border.h \
 dockbar.h \
 launcher.h \
 dockicon.h \
 desk.h \
 header.h \
 systray.h \
 manager.h \
 filedialog.h \
 colordialog.h \
 colorsel.h \
 msgbox.h \
 runner.h \
 dateclock.h \
 sysicon.h \
 deskapp.h \
 utils.h \
 deskdev.h \
 deskfile.h \
 deskfolder.h \
 dockapp.h \
 trash.h \
 trashdialog.h \
 deskicon.h \
 dockmenu.h \
 settings.h

target.path = $$PREFIX/bin/

INSTALLS += target
