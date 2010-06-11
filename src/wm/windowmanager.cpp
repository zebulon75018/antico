#include "windowmanager.hpp"
#include "client.hpp"
#include "atoms.hpp"
#include "debug.hpp"

#include <fixx11h.h>
#include <QDebug>
#include <QX11Info>

Q_GLOBAL_STATIC(WindowManager, windowManager)

WindowManager::WindowManager():
    QObject()
{
}

void WindowManager::init()
{
    ::createAtomList();

    XClearWindow(QX11Info::display(), QX11Info::appRootWindow());
    XSelectInput(QX11Info::display(), QX11Info::appRootWindow(), SubstructureRedirectMask);
    XSync(QX11Info::display(), False);
    XSelectInput(QX11Info::display(), QX11Info::appRootWindow(),
                 SubstructureNotifyMask | SubstructureRedirectMask |
                 StructureNotifyMask | ButtonPressMask | PropertyChangeMask|
                 KeyPressMask | ColormapChangeMask |EnterWindowMask);
    XSync(QX11Info::display(), False);
}

bool WindowManager::x11EventFilter(_XEvent *e)
{
    qDebug() << __PRETTY_FUNCTION__ << "got event" << ::eventName(e->type);

    switch (e->type)
    {
        case DestroyNotify:
        {
            Client *c = findClient(e->xdestroywindow.window);
            if (c)
            {
                _clients.remove(e->xdestroywindow.window);
                c->deleteLater();
                return true;
            }

            if (e->xdestroywindow.event != e->xdestroywindow.window)
            {
                return true;
            }

            return false;
        }

        case MapNotify:
            if (e->xmap.event != e->xmap.window)
                return true;
            return false;

        case UnmapNotify:
        {
            Client *c = findClient(e->xunmap.window);
            if (c)
            {
                if (e->xunmap.send_event)
                {
                }

                return true;
            }

            if (e->xunmap.event != e->xunmap.window)
            {
                return true;
            }

            return false;
        }

        case ConfigureRequest:
            return true;

        case MapRequest:
        {
            Client *c = findClient(e->xmaprequest.window);
            if (c)
            {
                c->map();
                return true;
            }

            if ((c = createClient(e->xmaprequest.window)))
                _clients.insert(e->xmaprequest.window, c);
        }
    }

    return false;
}

Client *WindowManager::createClient(Qt::HANDLE winId)
{
    return new Client(winId, this);
}

WindowManager *WindowManager::self()
{
    return windowManager();
}
