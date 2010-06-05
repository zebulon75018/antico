////////////////////////////////////////
//  File      : antico.h              //
//  Written by: g_cigala@virgilio.it  //
//  Copyright : GPL                   //
////////////////////////////////////////

// X event handler
// create new frame
// set window manager init settings to "antico.cfg"
// quit window manager

////////////////////////////////////////

#ifndef ANTICO_H
#define ANTICO_H

#include <QApplication>
#include <QStringList>
#include <QHash>

class Dockbar;
class Frame;
class Desk;
class Categorymenu;
class QSettings;

#include <X11/Xutil.h>

////////////////////////////////////////

class Antico : public QObject
{
    Q_OBJECT

public:
    static Antico *self();

    Antico();

    void init();
    void create_frame(WId, Dockbar *, Desk *);
    void raise_next_frame();
    void set_active_frame(Frame *);
    void send_configurenotify(Frame *);
    void set_settings();
    void run_app_at_startup();
    void get_atoms();
    void create_gui();
    void check_window_type(WId);
    void print_window_prop(WId);
    bool check_net_sys_tray_for(WId) const;
    void check_wm_transient_for(WId);
    Desk * get_desktop();
    Dockbar * get_dockbar();
    Categorymenu * get_category_menu();
    bool x11EventFilter(void *message, long *result);
   
private:
    QHash<int, Frame *> mapping_clients; // mapping client and frame (key=client_win_id value=frame)
    QHash<int, Frame *> mapping_frames; // mapping frame with their winId (key=frame_win_id value=frame)
    QList<Frame *> frm_list;
    QStringList frame_type;
    int servershapes; // server supports shapes
    int ShapeEventBase; // event base for shape extension
    QSettings *antico;
    Dockbar *dock;
    Desk *dsk;
    Frame *frm;
    Categorymenu *cat_menu;
};
#endif
