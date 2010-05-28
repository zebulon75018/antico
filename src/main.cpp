////////////////////////////////////////
//  File      : main.cpp              //
//  Written by: g_cigala@virgilio.it  //
//  Copyright : GPL                   //
////////////////////////////////////////

#include <QX11Info>
#include <QTranslator>
#include <QDebug>
#include <QSettings>

#include "antico.h"

////////////////////////////////////////


int main(int argc, char **argv)
{
    Antico a(argc, argv);
    XSelectInput(QX11Info::display(), QX11Info::appRootWindow(QX11Info::appScreen()), KeyPressMask | KeyReleaseMask | ButtonPressMask | ButtonReleaseMask |
                 KeymapStateMask | ButtonMotionMask | PointerMotionMask | EnterWindowMask | LeaveWindowMask | FocusChangeMask | VisibilityChangeMask |
                 ExposureMask | StructureNotifyMask | SubstructureRedirectMask | SubstructureNotifyMask);

    XClearWindow(QX11Info::display(), QX11Info::appRootWindow(QX11Info::appScreen()));
    XSync(QX11Info::display(), False);
    
    QTranslator translator;
    qDebug() << "Language:" << QLocale::system().name();
    translator.load(QLocale::system().name(), QCoreApplication::applicationDirPath() + "/../share/antico/language/");
    a.installTranslator(&translator);
    
    qDebug() << "GUI creation...";
    a.create_gui();
    
    a.connect(&a, SIGNAL(lastWindowClosed()), &a, SLOT(quit()));
    return a.exec();
}

