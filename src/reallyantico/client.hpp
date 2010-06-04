#ifndef _CLIENT_HPP
#define _CLIENT_HPP

#include <QObject>
#include <QX11Info>

class Dockbar;
class Desk;
class Frame;

class Client: public QObject
{
    Q_OBJECT

public:
    // Somewhere in the future will not need the pointer to DockBar and Desk
    Client(Qt::HANDLE window, const QString &type, Dockbar *dockBar, Desk *desk);

private:
    Qt::HANDLE m_window;
    QString m_type;
    Dockbar *m_dockBar;
    Desk *m_desk;
    Frame *m_frame;
};

#endif
