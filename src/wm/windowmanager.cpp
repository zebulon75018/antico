#include <QX11Info>
#include <QDebug>

#include "windowmanager.hpp"
#include "atoms.hpp"
#include "client.hpp"
#include "debug.hpp"

#include <X11/extensions/shape.h>

Q_GLOBAL_STATIC(WindowManager, windowManager)

WindowManager::WindowManager(QObject *parent):
    QObject(parent)
{
}

void WindowManager::init()
{
    XEvent event;

    XSetWindowAttributes attrib;
    attrib.override_redirect = True;
    attrib.event_mask = PropertyChangeMask;

    Atom wmAtom = XInternAtom(display(),
                              QString("WM_S%1").arg(QX11Info::appScreen()).toLatin1(),
                              False);

    m_wmWindow = XCreateWindow(display(), QX11Info::appRootWindow(),
                               -100, -100, 1, 1, 0,
                               CopyFromParent, InputOnly, CopyFromParent,
                               CWEventMask | CWOverrideRedirect,
                               &attrib);

    XMapWindow(display(), m_wmWindow);
    XLowerWindow(display(), m_wmWindow);

    XSelectInput(display(), m_wmWindow, PropertyChangeMask);
    XStoreName(display(), m_wmWindow, "Antico");
    XWindowEvent(display(), m_wmWindow, PropertyChangeMask, &event);
    XSelectInput(display(), m_wmWindow, KeyPressMask | StructureNotifyMask);

    XSetSelectionOwner(display(), wmAtom, m_wmWindow, event.xproperty.time);

    event.type = ClientMessage;
    event.xclient.format = 32;
    event.xclient.window = QX11Info::appRootWindow();
    event.xclient.message_type = ATOM(XA_RESOURCE_MANAGER);
    event.xclient.data.l[0] = event.xproperty.time;
    event.xclient.data.l[1] = wmAtom;
    event.xclient.data.l[2] = m_wmWindow;
    event.xclient.data.l[3] = event.xclient.data.l[4] = 0;
    XSendEvent(display(), QX11Info::appRootWindow(), False, StructureNotifyMask, &event);

    XSelectInput(display(), QX11Info::appRootWindow(),
                 KeyPressMask | KeyReleaseMask | ButtonPressMask |
                 ButtonReleaseMask | EnterWindowMask | LeaveWindowMask |
                 PropertyChangeMask | SubstructureNotifyMask |  SubstructureRedirectMask);

    XSync(display(), False);
}

bool WindowManager::x11EventFilter(void *message, long *result)
{
    XEvent *event = reinterpret_cast<XEvent *>(message);

    qDebug().nospace() << "x11EventFilter(" << eventName(event->type) << ")";

    Window window;

    switch (event->type)
    {
        case MapRequest:
            return handleMapRequest(event->xmaprequest.window);
    }

    return qApp->x11EventFilter(event);
}

WindowManager *WindowManager::self()
{
    return windowManager();
}

Display *WindowManager::display() const
{
    return QX11Info::display();
}

/*
Time WindowManager::serverTime() const
{
    XEvent event;

    XChangeProperty(display(), m_wmWindow, ATOM(WM_CLASS), ATOM(STRING), 8, PropModeAppend, NULL, 0);
    XWindowEvent(display(), m_wmWindow, PropertyChangeMask, &event);

    return event.xproperty.time;
    }*/

bool WindowManager::handleMapRequest(Qt::HANDLE window)
{
    XEvent event;
    XWindowAttributes attrib;
    bool manage = true;

    XGrabServer(display());
    XSync(display(), False);

    if (XCheckTypedWindowEvent(display(), window, DestroyNotify, &event) ||
        XCheckTypedWindowEvent(display(), window, UnmapNotify, &event))
    {
        XPutBackEvent(display(), &event);
        manage = false;
    }

    if (!XGetWindowAttributes(display(), window, &attrib))
        manage = false;

    if (!manage)
    {
        XUngrabServer(display());
        XSync(display(), False);
        qDebug() << "FAILED to manage window" << window;
        return false;
    }

    manageClient(window);
}

void WindowManager::manageClient(Qt::HANDLE window)
{
    Client *client;

    if ((client = m_clients.value(window)))
    {
    }
    else
    {
        client = new Client(window, this);
        m_clients.insert(window, client);
    }

    XUngrabServer(display());
    XSync(display(), False);
}
