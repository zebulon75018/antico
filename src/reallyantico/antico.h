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
#include <QMap>

class Dockbar;
class Frame;
class Desk;
class Categorymenu;
class QSettings;
class Client;

////////////////////////////////////////

class Antico : public QObject
{
    Q_OBJECT

public:
    static Antico *self();

    Antico();

    void init();
    void set_settings(); // TODO
    bool x11EventFilter(void *message, long *result);

private:
    void addClient(Client *c);
    Client *createClient(Qt::HANDLE window, bool isMapped = false);
    Client *findClient(Qt::HANDLE window) const;
    Frame *findClientFrame(Qt::HANDLE window) const;

private:
    QMap<Qt::HANDLE, Client *> m_clients;

    QSettings *antico; // TODO
};
#endif
