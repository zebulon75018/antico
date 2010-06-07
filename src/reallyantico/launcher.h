////////////////////////////////////////
//  File      : launcher.h            //
//  Written by: g_cigala@virgilio.it  //
//  Copyright : GPL                   //
////////////////////////////////////////

// the "Start" menu in the bottom-left of the dockbar
// manage the menu entry

////////////////////////////////////////

#ifndef LAUNCHER_H
#define LAUNCHER_H

#include <QLabel>

class Antico;
class Categorymenu;
class Fileicon;
class QSettings;
class QMenu;

////////////////////////////////////////

class Launcher : public QLabel
{
    Q_OBJECT

public:
    Launcher(Antico *, QWidget *parent=0);
    ~Launcher();
    void read_settings();
    void init();
    void update_style();

public slots:
    void run_command(QAction *);
    
signals:
 
protected:
    void mousePressEvent(QMouseEvent *);
    void enterEvent(QEvent *);
    void leaveEvent(QEvent *);

private:
    QString launcher_pix;
    QString quit_pix;
    QString shutdown_pix;
    QString restart_pix;
    QString show_pix;
    QString refresh_pix;
    int dock_position;
    int dock_height;
    Antico *app;
    QAction *quit;
    QAction *shutdown;
    QAction *restart;
    QAction *refresh;
    QAction *show_desk;
    QSettings *antico;
    QMenu *main_menu;
};

#endif
