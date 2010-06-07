////////////////////////////////////////
//  File      : antico.cpp            //
//  Written by: g_cigala@virgilio.it  //
//  Copyright : GPL                   //
////////////////////////////////////////

#include <QDebug>
#include <QSettings>
#include <QX11Info>
#include <QDesktopWidget>
#include <QProcess>
#include <QDBusConnection>
#include <QDBusInterface>
#include <QDir>

#include "antico.h"
#include "debug.hpp"
#include "atoms.hpp"
#include "frame.h"
#include "client.hpp"

#include <X11/extensions/shape.h>
#include <X11/Xatom.h>

////////////////////////////////////////

Q_GLOBAL_STATIC(Antico, instance)

Antico *Antico::self()
{
    return instance();
}

Antico::Antico(): QObject()
{
}

void Antico::init()
{
   XEvent event;

   Atom atom = XInternAtom(QX11Info::display(),
			   QString("WM_S%1").arg(QX11Info::appScreen()).toLatin1(),
			   False);

   XSetWindowAttributes attrs;
   attrs.override_redirect = True;

   Window window = XCreateWindow(QX11Info::display(), QX11Info::appRootWindow(),
				 -100, -100, 1, 1, 0,
				 CopyFromParent, InputOnly, CopyFromParent,
				 CWOverrideRedirect,
				 &attrs);

   Atom tmp = XA_ATOM;
   XSelectInput(QX11Info::display(), window, PropertyChangeMask);
   XChangeProperty(QX11Info::display(), window, XA_ATOM, XA_ATOM, 32,
		   PropModeReplace, (unsigned char *)&tmp, 1);

   XEvent ev;

   XSync(QX11Info::display(), False);
   XCheckTypedWindowEvent(QX11Info::display(), window, PropertyNotify, &ev);
   XSetSelectionOwner(QX11Info::display(), atom, window, ev.xproperty.time);

   event.type = ClientMessage;
   event.xclient.window = QX11Info::appRootWindow();
   event.xclient.display = QX11Info::display();
   event.xclient.message_type = XInternAtom(QX11Info::display(), "MANAGER", False);
   event.xclient.format = 32;
   event.xclient.data.l[0] = ev.xproperty.time;
   event.xclient.data.l[1] = atom;
   event.xclient.data.l[2] = window;
   event.xclient.data.l[3] = event.xclient.data.l[4] = 0;
   XSendEvent(QX11Info::display(), QX11Info::appRootWindow(), False, StructureNotifyMask, &ev);


   XSelectInput(QX11Info::display(), QX11Info::appRootWindow(), SubstructureRedirectMask);
   XSync(QX11Info::display(), False);

   set_settings(); // TODO
}

bool Antico::x11EventFilter(void *message, long *result)
{
    XEvent *e = reinterpret_cast<XEvent *>(message);

    qDebug().nospace() << __PRETTY_FUNCTION__ << ": get event: " << eventName(e->type);

    switch (e->type)
    {
        case MapRequest:
        {
            Client *c = findClient(e->xmaprequest.window);
            if (!c)
            {
                c = createClient(e->xmaprequest.window);
                if (!c)
                    XMapRaised(QX11Info::display(), e->xmaprequest.window);

                return true;
            }
            else
            {
                c->windowEvent(e);
                return true;
            }
        }
    }

    return false;
}

void Antico::set_settings()
{
    // default path
    antico = new QSettings(QSettings::UserScope, "antico", "antico", this);
    // set default style on first installation, if no "/antico.cfg" is set
    if (antico->childGroups().isEmpty())
    {
        qDebug() << "Set default settings ...";
        antico->beginGroup("Style");
        antico->setValue("name", "default.stl");
        antico->setValue("path", QCoreApplication::applicationDirPath() + "/../share/antico/themes/default/");
        antico->endGroup(); //Style
        /////////////////////////////////////////////////////////////////////////
	antico->beginGroup("Deskbar");
	antico->setValue("no_hide_iconify", "yes"); //The default is hide deskbar icon on iconify window
	antico->endGroup(); //Deskbar
        antico->sync();
    }

    if (antico->status() == QSettings::AccessError)
    {
        qFatal ("Error on setting antico.cfg");
    }
}

void Antico::addClient(Client *c)
{
    m_clients.insert(c->window(), c);
}

Client *Antico::createClient(Qt::HANDLE window, bool isMapped)
{
    Client *c = new Client(window, this);
    if (!c->manage(isMapped))
    {
//        Client::deleteClient(
        return NULL;
    }

    addClient(c);
}

Client *Antico::findClient(Qt::HANDLE window) const
{
    return m_clients.value(window);
}

Frame *Antico::findClientFrame(Qt::HANDLE window) const
{
    Client *client = NULL;

    foreach (Client *c, m_clients)
    {
        if (c->frame()->winId() == window)
        {
            client = c;
            break;
        }
    }
    if (!client)
        return NULL;

    return client->frame();
}
