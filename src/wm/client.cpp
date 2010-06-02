#include <QX11Info>

#include "client.hpp"
#include "decoratedframe.hpp"

#include <X11/Xlib.h>

static Display *display()
{
    return QX11Info::display();
}

Client::Client(Qt::HANDLE window, QObject *parent):
    QObject(parent)
{
    m_decoratedFrame = new DecoratedFrame;

    XGrabServer(display());

    XSetWindowBorderWidth(display(), window, 0);
    XSetWindowBorderWidth(display(), m_decoratedFrame->winId(), 0);

    // TODO: Make border width and height dynamic
    XReparentWindow(display(), window, m_decoratedFrame->winId(), 2, 20);

    XSetWindowAttributes attrib;
    attrib.event_mask = ButtonPressMask | ButtonReleaseMask |
        KeymapStateMask | ButtonMotionMask | PointerMotionMask|
        EnterWindowMask | LeaveWindowMask | FocusChangeMask|
        ExposureMask | StructureNotifyMask | SubstructureRedirectMask | SubstructureNotifyMask;
    XChangeWindowAttributes(display(), m_decoratedFrame->winId(), CWEventMask, &attrib);

    attrib.event_mask = ColormapChangeMask | PropertyChangeMask | StructureNotifyMask | FocusChangeMask;
    XChangeWindowAttributes(display(), window, CWEventMask, &attrib);

    XAddToSaveSet(display(), window);

    // TODO

    m_decoratedFrame->show();

    XMapWindow(display(), window);
    XSync(display(), False);

    XUngrabServer(display());
}
