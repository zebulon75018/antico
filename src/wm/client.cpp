#include "client.hpp"
#include "anticodecoration.hpp"
#include "decorations/ubuntu/ubuntudecoration.hpp"

#include <QX11Info>
#include <QDebug>

#include <X11/Xutil.h>

Client::Client(Qt::HANDLE winId, QObject *parent)
    : QObject(parent)
    , _winId(winId)
{
    XWindowAttributes attr;
    XGetWindowAttributes(QX11Info::display(), _winId, &attr);

    _geometry.setX(attr.x);
    _geometry.setY(attr.y);
    _geometry.setWidth(attr.width);
    _geometry.setHeight(attr.height);

    /*
      If you have a custom decoration, pass the pointer to it.
      Yes. I know it is ugly but there are no plans to add support for dynamic plugins (the custom decoration would be a dynamic library).
      Once compiled it will not be possible to change the decoration unless the decoration be a engine (like KWin)
    */

    _decoration = new UbuntuDecoration(this);

    XSetWindowBorderWidth(QX11Info::display(), _winId, 0);
    XSetWindowBorderWidth(QX11Info::display(), _decoration->winId(), 0);
    XReparentWindow(QX11Info::display(), _winId, _decoration->winId(),
                    _decoration->borderSize().left(),
                    _decoration->borderSize().top() + _decoration->borderSize().titleBarHeight());
    XAddToSaveSet(QX11Info::display(), _winId);

    _decoration->setGeometry(_geometry.x(), _geometry.y(),
                             _geometry.width() + _decoration->borderSize().measuredWidth(),
                             _geometry.height() + _decoration->borderSize().measuredHeight());

    XSelectInput(QX11Info::display(), _decoration->winId(),
                 KeyPressMask | KeyReleaseMask |
                 ButtonPressMask | ButtonReleaseMask |
                 KeymapStateMask | ButtonMotionMask |
                 PointerMotionMask | EnterWindowMask |
                 LeaveWindowMask | FocusChangeMask |
                 ExposureMask |StructureNotifyMask |
                 SubstructureRedirectMask | SubstructureNotifyMask);

    XSetWindowAttributes sattr;
    sattr.event_mask = ColormapChangeMask | PropertyChangeMask;
    XChangeWindowAttributes(QX11Info::display(), _winId, CWEventMask, &sattr);

    map();
}

Client::~Client()
{
    _decoration->close();
    _decoration->deleteLater();
}

bool Client::x11EventFilter(_XEvent *e)
{
    return false;
}

void Client::move(const QPoint &p)
{
    _decoration->setGeometry(p.x(), p.y(),
			     _decoration->geometry().width(),
			     _decoration->geometry().height());    
}

void Client::resize(const QSize &size)
{
}

void Client::minimize()
{
}

void Client::maximize()
{    
}

void Client::close()
{
}

void Client::map()
{
    _decoration->show();
    XMapWindow(QX11Info::display(), _winId);
    XSync(QX11Info::display(), False);
}
