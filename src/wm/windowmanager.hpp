#ifndef _X11MANAGEMENT_HPP
#define _X11MANAGEMENT_HPP

#include <QApplication>
#include <QStringList>
#include <QHash>
#include <QX11Info>
#include <QMap>

class Client;

class WindowManager: public QObject
{
    Q_OBJECT

public:
    WindowManager(QObject *parent = 0);

    void init();
    bool x11EventFilter(void *message, long *result);

    static WindowManager *self();

private:
    Display *display() const;

    bool handleMapRequest(Qt::HANDLE window);

    void manageClient(Qt::HANDLE window);

private:
    Qt::HANDLE m_wmWindow;
    QMap<Qt::HANDLE, Client *> m_clients;
};

#endif // _X11MANAGEMENT_HPP
