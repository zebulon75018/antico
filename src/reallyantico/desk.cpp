////////////////////////////////////////
//  File      : desk.cpp              //
//  Written by: g_cigala@virgilio.it  //
//  Copyright : GPL                   //
////////////////////////////////////////

#include <QSettings>
#include <QDesktopWidget>
#include <QRubberBand>
#include <QMenu>
#include <QDBusConnection>
#include <QDBusInterface>
#include <QMouseEvent>
#include <QDebug>
#include <QAbstractItemView>
#include <QFileSystemModel>
#include <QDBusReply>

#include "desk.h"
#include "antico.h"
#include "utils.h"
#include "frame.h"

////////////////////////////////////////

Desk::Desk(Antico *a, QWidget *parent) : QLabel(parent)
{
    app = a;
    cat_menu = app->get_category_menu();
    read_settings();
    set_geometry();
    setAttribute(Qt::WA_AlwaysShowToolTips);
    show();
}

Desk::~Desk()
{
    delete antico;
    delete app;
    delete cat_menu;
}

void Desk::read_settings()
{
    // get style path
    antico = new QSettings(QSettings::UserScope, "antico", "antico", this);
    antico->beginGroup("Style");
    QString stl_name = antico->value("name").toString();
    QString stl_path = antico->value("path").toString();
    antico->endGroup(); //Style
    // get style values
    QSettings *style = new QSettings(stl_path + stl_name, QSettings::IniFormat,this);
    style->beginGroup("Desktop");
    wall_pix = stl_path + style->value("wall_pix").toString();
    style->endGroup(); //Desktop
    style->beginGroup("Dockbar");
    dock_height = style->value("dock_height").toInt();
    style->endGroup(); //Dockbar
    style->beginGroup("Other");
    folder_link_pix = stl_path + style->value("folder_link_pix").toString();
    file_link_pix = stl_path + style->value("file_link_pix").toString();
    app_link_pix = stl_path + style->value("app_link_pix").toString();
    style->endGroup(); //Other
}

void Desk::set_geometry()
{
    setPixmap(wall_pix);
    setScaledContents(true);
    setGeometry(0, 0, QApplication::desktop()->width(), QApplication::desktop()->height()-dock_height);
}

void Desk::update_style()
{
    read_settings();
    set_geometry();
}
