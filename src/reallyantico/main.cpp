////////////////////////////////////////
//  File      : main.cpp              //
//  Written by: g_cigala@virgilio.it  //
//  Copyright : GPL                   //
////////////////////////////////////////

#include <QX11Info>
#include <QTranslator>
#include <QDebug>
#include <QSettings>
#include <QLocale>

#include "antico.h"
#include "atoms.hpp"

////////////////////////////////////////

bool _x11EventFilter(void *message, long *result)
{
    return Antico::self()->x11EventFilter(message, result);
}

int main(int argc, char **argv)
{
    QApplication a(argc, argv);
    a.setQuitOnLastWindowClosed(false);

    _createAtomList();

    Antico::self()->init();
    
    QTranslator translator;
    qDebug() << "Language:" << QLocale::system().name();
    translator.load(QLocale::system().name(), QCoreApplication::applicationDirPath() + "/../share/antico/language/");
    a.installTranslator(&translator);

    a.setEventFilter(_x11EventFilter);

    return a.exec();
}

