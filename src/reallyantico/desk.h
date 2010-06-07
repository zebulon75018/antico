////////////////////////////////////////
//  File      : desk.h                //
//  Written by: g_cigala@virgilio.it  //
//  Copyright : GPL                   //
////////////////////////////////////////

// the desktop on the screen

////////////////////////////////////////

#ifndef DESK_H
#define DESK_H

#include <QLabel>
#include <QHash>

class Antico;
class Deskapp;
class Deskdev;
class Deskfile;
class Deskfolder;
class Deskicon;
class Frame;
class QDBusInterface;
class QRubberBand;
class Appicon;
class QMenu;
class QSettings;
class Categorymenu;

////////////////////////////////////////

class Desk : public QLabel
{
    Q_OBJECT

public:
    Desk(Antico *, QWidget *parent=0);
    ~Desk();
    void read_settings();
    void set_geometry();
    void update_style();

private:
    int dock_height;
    QList<Deskfolder *> desk_folders;
    QList<Deskfolder *> desk_folders_selected;
    QList<Deskfile *> desk_files;
    QList<Deskfile *> desk_files_selected;
    QList<Deskapp *> desk_apps;
    QList<Deskapp *> desk_apps_selected;
    QHash<QString, Deskdev *> desk_dev;
    QHash<int, Deskicon *> desk_icons; // mapping Frame and Deskicon (key=frame win_id value=Deskicon)
    QString wall_pix;
    QString folder_link_pix;
    QString file_link_pix;
    QString app_link_pix;
    QPoint rubber_press;
    QDBusInterface *dbus_interface;
    QRubberBand *rubber_band;
    QSettings *antico;
    Antico *app;
    Categorymenu *cat_menu;
    Deskfolder *d_folder;
    Deskfile *d_file;
    Deskapp *d_app;
    Deskicon *d_icon;
    Appicon *app_icon;
};


#endif
