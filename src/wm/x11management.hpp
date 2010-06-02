#ifndef _X11MANAGEMENT_HPP
#define _X11MANAGEMENT_HPP

#include <QApplication>
#include <QStringList>
#include <QHash>

class Frame;
class Categorymenu;
class QSettings;

#include <X11/Xutil.h>

class Antico: public QApplication
{
    Q_OBJECT

protected:
    virtual bool x11EventFilter(XEvent *);

public:
    Antico(int &argc, char **argv);
    ~Antico();
    void create_frame(WId);
    void raise_next_frame();
    void set_active_frame(Frame *);
    void send_configurenotify(Frame *);
    void set_settings();
    void get_atoms();
    void send_supported_hints();
    void check_window_type(WId);
    void print_window_prop(WId);
    bool check_net_sys_tray_for(WId) const;
    void check_wm_transient_for(WId);

private:
    QHash<int, Frame *> mapping_clients;   
    QHash<int, Frame *> mapping_frames;   
    QList<Frame *> frm_list;
    int next_frame;
    QStringList frame_type;
    int servershapes;  
    int ShapeEventBase;

    Atom _net_wm_name;
    Atom _net_wm_icon;
    Atom _net_supported;
    Atom _net_wm_user_time;
    Atom _net_wm_state;
    Atom _net_wm_window_type;
    Atom _net_wm_window_type_normal;
    Atom _net_wm_window_type_desktop;
    Atom _net_wm_window_type_dialog;
    Atom _net_wm_window_type_splash;
    Atom _net_wm_window_type_dnd;
    Atom _kde_net_wm_system_tray_window_for;

    QSettings *antico;
    Frame *frm;
};

#endif // _X11MANAGEMENT_HPP
