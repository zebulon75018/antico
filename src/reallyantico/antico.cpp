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

#include <X11/extensions/shape.h>

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

   XSetWindowAttributes attrib;
   attrib.override_redirect = True;
   attrib.event_mask = PropertyChangeMask;

   Atom atom = XInternAtom(QX11Info::display(),
			   QString("WM_S%1").arg(QX11Info::appScreen()).toLatin1(),
			   False);

   Window window = XCreateWindow(QX11Info::display(), QX11Info::appRootWindow(),
				 -100, -100, 1, 1, 0,
				 CopyFromParent, InputOnly, CopyFromParent,
				 CWEventMask | CWOverrideRedirect,
				 &attrib);

   XMapWindow(QX11Info::display(), window);
   XLowerWindow(QX11Info::display(), window);

   XSelectInput(QX11Info::display(), window, PropertyChangeMask);
   XStoreName(QX11Info::display(), window, "Antico");
   XWindowEvent(QX11Info::display(), window, PropertyChangeMask, &event);
   XSelectInput(QX11Info::display(), window, StructureNotifyMask);
   
   XSetSelectionOwner(QX11Info::display(), atom, window, event.xproperty.time);

   event.type = ClientMessage;
   event.xclient.format = 32;
   event.xclient.window = QX11Info::appRootWindow();
   event.xclient.message_type = ATOM(XA_RESOURCE_MANAGER);
   event.xclient.data.l[0] = event.xproperty.time;
   event.xclient.data.l[1] = atom;
   event.xclient.data.l[2] = window;
   event.xclient.data.l[3] = event.xclient.data.l[4] = 0;
   XSendEvent(QX11Info::display(), QX11Info::appRootWindow(), False, StructureNotifyMask, &event);

   XSelectInput(QX11Info::display(), QX11Info::appRootWindow(), SubstructureRedirectMask);
   XSync(QX11Info::display(), False);

   set_settings(); // TODO

   int err;
   servershapes = XShapeQueryExtension(QX11Info::QX11Info::display(), &ShapeEventBase, &err);
}

bool Antico::x11EventFilter(void *message, long *result)
{
    XEvent *event = reinterpret_cast<XEvent *>(message);

    Frame *frm;
    XWindowAttributes wa;
    XPropertyEvent *pev;
    XClientMessageEvent *mev;
    KeySym sym;
    uint mod;
    uint keymask1 = Mod1Mask & 0x0F;

    if (event->type != 6 && event->type != 12) // ignore Motion/Expose event
    {
        qDebug() << "--------------------------------------------------------------------------------------------";
        qDebug() << "XEvent:" << eventName(event->type) << " ( WId:" << event->xany.window << ")";
    }

    switch (event->type)
    {
        /////////////////// REQUEST EVENTS ///////////////////
    case MapRequest:
        qDebug() << "[MapRequest]" << event->xmaprequest.window;

        if (!XGetWindowAttributes(QX11Info::QX11Info::display(), event->xmaprequest.window, &wa))
            return true;

        if (wa.override_redirect)
        {
            qDebug() << "No Frame for Override redirect on Client:" << event->xmaprequest.window;
            return true;
        }

        if ((frm = mapping_clients.value(event->xmaprequest.window)) != NULL)
        {
            qDebug() << "--> MapRequest for already mapped Client:" << event->xmaprequest.window;
            frm->map_it();
        }
        else
        {
            qDebug() << "--> MapRequest for new Client:" << event->xmaprequest.window;
            create_frame(event->xmaprequest.window, NULL, NULL); // create new Frame for Client
        }
        return false;
        break;

    case ConfigureRequest:
        qDebug() << "[ConfigureRequest]";
        XWindowChanges wc;

        if ((frm = mapping_clients.value(event->xconfigurerequest.window)) != NULL)
        {
            qDebug() << "--> Request from map frame:" << frm->winId() << "- Name:" << frm->cl_name() << "- Client:" << event->xconfigurerequest.window;

            if (event->xconfigurerequest.value_mask & (CWWidth|CWHeight|CWX|CWY))
            {
                int cx,cy,cw,ch;

                if (event->xconfigurerequest.value_mask & CWWidth)
                    cw = event->xconfigurerequest.width;
                else
                    cw = frm->cl_w();

                if (event->xconfigurerequest.value_mask & CWHeight)
                    ch = event->xconfigurerequest.height;
                else
                    ch = frm->cl_h();

                if ((event->xconfigurerequest.value_mask & CWX))
                    cx = event->xconfigurerequest.x;
                else
                    cx = frm->cl_x();

                if ((event->xconfigurerequest.value_mask & CWY))
                    cy = event->xconfigurerequest.y;
                else
                    cy = frm->cl_y();

                frm->resize_request(cw, ch);
                event->xconfigurerequest.value_mask &= ~(CWWidth|CWHeight|CWX|CWY);
            }

            if (! event->xconfigurerequest.value_mask)
                return true;

            wc.width = frm->width();
            wc.height = frm->height();
            wc.x = frm->x();
            wc.y = frm->y();
            wc.border_width = 0;
            wc.sibling = event->xconfigurerequest.above;
            wc.stack_mode = event->xconfigurerequest.detail;
            XConfigureWindow(QX11Info::QX11Info::display(), frm->winId(), event->xconfigurerequest.value_mask, &wc);
            send_configurenotify(frm);
        }
        else  // never mapped window
        {
            qDebug() << "--> Request from unmap client:" << event->xconfigurerequest.window;
            wc.x = event->xconfigurerequest.x;
            wc.y = event->xconfigurerequest.y;
            wc.width = event->xconfigurerequest.width;
            wc.height = event->xconfigurerequest.height;
            event->xconfigurerequest.value_mask &= (CWX|CWY|CWWidth|CWHeight);
            XConfigureWindow(QX11Info::QX11Info::display(), event->xconfigurerequest.window, event->xconfigurerequest.value_mask, &wc);
        }
        return false;
        break;

    case CirculateRequest:
        qDebug() << "[CirculateRequest]";

        if ((frm = mapping_clients.value(event->xcirculaterequest.window)) != NULL)
        {
            if (event->xcirculaterequest.place == PlaceOnTop)
            {
                frm->raise_it();
                qDebug() << "--> CirculateRequest for frame:" << frm->winId() << "- Name:" << frm->cl_name() << "- Client:" << event->xcirculaterequest.window;
            }
        }
        else
            XLowerWindow(QX11Info::QX11Info::display(), event->xcirculaterequest.window);

        return false;
        break;

        /////////////////// NOTIFY EVENTS ///////////////////

    case MapNotify:
        qDebug() << "[MapNotify]";

        if ((frm = mapping_clients.value(event->xunmap.window)) != NULL)
        {
            frm->map_it();
            qDebug() << "MapNotify for frame:" << frm->winId() << "- Name:" << frm->cl_name() << " - Client:" << frm->cl_win();
            return true;
        }
        if (event->xmap.event != event->xmap.window)
            return true;

        return false;
        break;

    case UnmapNotify:
        qDebug() << "[UnmapNotify]";

        if ((frm = mapping_clients.value(event->xunmap.window)) != NULL)
        {
            if (event->xunmap.send_event)
            {
                frm->withdrawn_it();
                qDebug() << "Withdrawn for frame:" << frm->winId() << "- Name:" << frm->cl_name() << " - Client:" << frm->cl_win();
            }
            else
            {
                frm->unmap_it();
                qDebug() << "Unmap for frame:" << frm->winId() << "- Name:" << frm->cl_name() << " - Client:" << frm->cl_win();
            }
            return true;
        }
        if (event->xunmap.event != event->xunmap.window)
            return true;

        return false;
        break;

    case DestroyNotify:
        qDebug() << "[DestroyNotify]";

        if ((frm = mapping_clients.value(event->xdestroywindow.window)) != NULL ||
            (frm = mapping_frames.value(event->xdestroywindow.window)) != NULL)
        {
            qDebug() << "--> Destroy frame:" << frm->winId() << "- Name:" << frm->cl_name() << "- Client:" << event->xdestroywindow.window;
            mapping_clients.remove(event->xdestroywindow.window);
            mapping_frames.remove(frm->winId());
            delete frm;
            return true;
        }
        if (event->xdestroywindow.event != event->xdestroywindow.window)
        {
            return true;
        }
        qDebug() << "Mapping clients num:" << mapping_clients.size() << "Mapping frames num:" << mapping_frames.size();
        return false;
        break;

    case EnterNotify:
        qDebug() << "[EnterNotify]";

        if ((frm = mapping_frames.value(event->xcrossing.window)) != NULL)
        {
            qDebug() << "Enter in map frame:" << frm->winId();
        }
        else
        {
            qDebug() << "Enter in not map client:" << event->xcrossing.window;
        }
        return false;
        break;

    case LeaveNotify:
        qDebug() << "[LeaveNotify]";

        return false;
        break;

    case VisibilityNotify:
        qDebug() << "[VisibilityNotify]";

        return false;
        break;

    case ConfigureNotify:
        qDebug() << "[ConfigureNotify]";

        if (event->xconfigure.event != event->xconfigure.window)
            return true;

        return false;
        break;

    case ReparentNotify:
        qDebug() << "[ReparentNotify]";

        if ((frm = mapping_clients.value(event->xreparent.window)) != NULL && event->xreparent.parent != frm->winId())
        {
            qDebug() << "ReparentNotify for frame:" << frm->winId() << "- Name:" << frm->cl_name();
            mapping_clients.remove(event->xreparent.window);
            return true;
        }
        else
        {
            qDebug() << "ReparentNotify for client:" << event->xreparent.window << "- new Parent:" << event->xreparent.parent;
        }
        return false;
        break;

    case PropertyNotify:
        qDebug() << "[PropertyNotify]";
        qDebug() << "Atom: " << XGetAtomName(QX11Info::QX11Info::display(), event->xproperty.atom) << '\n';
        pev = &event->xproperty;

        if ((frm = mapping_clients.value(event->xproperty.window)) != NULL)
        {
            qDebug() << "Client already mapped by frame:" << frm->winId() << "- Name:" << frm->cl_name() << "- Client:" << event->xproperty.window;

            if (pev->atom == ATOM(WM_HINTS))
            {
                qDebug() << "---> wm_hints";
                frm->get_wm_hints();
                return true;
            }
            if (pev->atom == ATOM(WM_NORMAL_HINTS))
            {
                qDebug() << "---> wm_normal_hints";
                frm->get_wm_normal_hints();
                return true;
            }
            if (pev->atom == ATOM(WM_NAME) || pev->atom == ATOM(_NET_WM_NAME))
            {
                qDebug() << "---> wm_name";
                frm->get_wm_name();
                frm->update_name();
                return true;
            }
            if (pev->atom == ATOM(WM_STATE) || pev->atom == ATOM(_NET_WM_STATE))
            {
                qDebug() << "---> wm_state";
                qDebug() << "Window:" << pev->window << "changing state";
                return true;
            }
            if (pev->atom == ATOM(WM_COLORMAPS))
            {
                qDebug() << "---> wm_colormap_windows";
                frm->get_colormaps();
                return true;
            }
            if (pev->atom == ATOM(WM_TRANSIENT_FOR))
            {
                qDebug() << "---> wm_transient_for";
                return true;
            }
            if (pev->atom == ATOM(_NET_WM_USER_TIME))
            {
                qDebug() << "---> _net_wm_user_time";
                frm->set_focus(CurrentTime);
                set_active_frame(frm);
                return true;
            }
            return true;
        }
        else if ((frm = mapping_frames.value(event->xproperty.window)) != NULL)
        {
            qDebug() << "Frame already mapped:" << event->xproperty.window << "- Name:" << frm->cl_name() << "- Client:" << frm->cl_win();
            return true;
        }
        else
        {
            qDebug() << "Client not mapped:" << event->xproperty.window;
            return false;
        }
        break;

    case ColormapNotify:
        qDebug() << "[ColormapNotify]";

        if ((frm = mapping_clients.value(event->xcolormap.window)) != NULL)
        {
            qDebug() << "ColormapNotify for frame:" << frm->winId() << " - Name:" << frm->cl_name() << " - Client:" << event->xproperty.window;
            frm->set_colormaps(event->xcolormap.colormap);
            return true;
        }
        return false;
        break;

        /////////////////// OTHER EVENTS ///////////////////
    case ButtonPress:
        qDebug() << "[ButtonPress]";

        if ((frm = mapping_frames.value(event->xbutton.window)) != NULL) // get the frame from his winId
        {
            qDebug() << "Button press:" <<  event->xbutton.button << "for map frame:" << event->xbutton.window;
            set_active_frame(frm);
            XSetInputFocus(QX11Info::QX11Info::display(), event->xbutton.window, RevertToNone, CurrentTime);
        }
        else
        {
            qDebug() << "Button press:" <<  event->xbutton.button << "for client:" << event->xbutton.window;
            XSetInputFocus(QX11Info::QX11Info::display(), event->xbutton.window, RevertToNone, CurrentTime);
        }
        return false;
        break;

    case ClientMessage:
        qDebug() << "[ClientMessage]";
        mev = &event->xclient;

        if (mev->message_type == ATOM(WM_CHANGE_STATE) && event->xclient.format == 32 && event->xclient.data.l[0] == IconicState)
        {
            qDebug() << "---> wm_change_state: IconicState";

            if ((frm = mapping_clients.value(event->xclient.window)) != NULL)
                frm->iconify_it();
            return true;
        }

        return false;
        break;

    case KeyPress:
        qDebug() << "[KeyPress]";

        sym = (int)XLookupKeysym(&event->xkey, 0);
        mod = event->xkey.state & keymask1 ;

        return false;
        break;

    default:

        if (servershapes && event->type == (ShapeEventBase + ShapeNotify))
        {
            qDebug() << "[ShapeNotify]";
            XShapeEvent *sev = (XShapeEvent *)event;

            if ((frm = mapping_clients.value(sev->window)) != NULL)
            {
                qDebug() << "ShapeNotify for frame:" << frm->winId() << " - Name:" << frm->cl_name() << " - Client:" << sev->window;
                frm->reshape();
                return true;
            }
        }
        return false;
        break;
    }
}

void Antico::create_frame(Window c_win, Dockbar *dock, Desk *desk) // create new frame around the client app
{
    print_window_prop(c_win);

    check_wm_transient_for(c_win);
    check_window_type(c_win);

    /////// MAP THE NEW CLIENT ////////
    qDebug() << "Mapping window type:" << frame_type.at(0);

    if (frame_type.at(0) != "Splash")
    {
        frm = new Frame(c_win, frame_type.at(0), NULL, NULL); // select always the first type in list (preferred)
        mapping_clients.insert(c_win, frm); // save the client winId/frame
        mapping_frames.insert(frm->winId(), frm); // save the frame winId/frame
    }
    else
    {
        XMapRaised(QX11Info::QX11Info::display(), c_win);
    }

    frame_type.clear(); // clear the window type list
    qDebug() << "Mapping clients num:" << mapping_clients.size() << "Mapping frames num:" << mapping_frames.size();
}

void Antico::check_window_type(Window c_win) // chech the window type before mapping
{
    Atom type_ret = None;
    Atom *win_type;
    unsigned char *data = 0;
    int format = 0;
    unsigned long n = 0;
    unsigned long extra = 0;

    if (XGetWindowProperty(QX11Info::QX11Info::display(), c_win, ATOM(_NET_WM_WINDOW_TYPE), 0, 100, False,
                           AnyPropertyType, &type_ret, &format, &n, &extra, (unsigned char **) &data) == Success && data)
    {
        win_type = (Atom *)data;

        for (unsigned i = 0; i < n; i++)
        {
            if (win_type[i] == ATOM(_NET_WM_WINDOW_TYPE_NORMAL))
            {
                frame_type << "Normal";
                qDebug() << "Window type: NORMAL TYPE";
            }
            else if (win_type[i] == ATOM(_NET_WM_WINDOW_TYPE_DIALOG))
            {
                frame_type << "Dialog";
                qDebug() << "Window type: DIALOG TYPE";
            }
            else if (win_type[i] == ATOM(_NET_WM_WINDOW_TYPE_SPLASH))
            {
                frame_type << "Splash";
                qDebug() << "Window type: SPLASH TYPE";
            }
            else if (win_type[i] == ATOM(_NET_WM_WINDOW_TYPE_DESKTOP))
            {
                frame_type << "Desktop";
                qDebug() << "Window type: DESKTOP TYPE";
            }
            else // if other window type
            {
                frame_type << "Normal";
                qDebug() << "Other Window type. Set as NORMAL TYPE";
            }
        }
        return;
    }
    /// IF PROPERTY NOT SET ///
    frame_type << "Normal";
    qDebug() << "Window type not set: SET AS NORMAL";

    XFree(data);
}

void Antico::check_wm_transient_for(Window c_win)
{
    Atom type_ret = None;
    Window prop_window_return;
    unsigned char *data = 0;
    int format = 0;
    unsigned long n = 0;
    unsigned long extra = 0;

    if (XGetWindowProperty(QX11Info::QX11Info::display(), c_win, ATOM(WM_TRANSIENT_FOR), 0, 100, False,
                           AnyPropertyType, &type_ret, &format, &n, &extra, (unsigned char **)&data) == Success && data)
    {
        qDebug() << "Window type: WM_TRANSIENT_FOR. Set as DIALOG";

        if (XGetTransientForHint(QX11Info::QX11Info::display(), c_win, &prop_window_return))
        {
            qDebug() << "Window return:" << prop_window_return;
            frame_type << "Dialog";
        }
    }
    XFree(data);
}

void Antico::print_window_prop(Window c_win) // print the window properties
{
    int num_prop_ret = 0;
    Atom *list_prop = XListProperties(QX11Info::QX11Info::display(), c_win, &num_prop_ret);
    qDebug() << "Properties num:" << num_prop_ret;

    qDebug() << "---------------------------------------------";

    for (int i = 0; i < num_prop_ret; i++)
    {
        char *name = XGetAtomName(QX11Info::QX11Info::display(), list_prop[i]);
        qDebug() << i << "- Window Property:" << name;
        XFree(name);
    }

    qDebug() << "---------------------------------------------";
}

void Antico::set_active_frame(Frame *frm) // activation of selected frame and unactivation of others
{
    foreach(Frame *frame, mapping_clients)
    {
        if (frame == frm)
        {
            qDebug() << "Frame raise and active: " << frame->winId() << "- Name:" << frame->cl_name() << "- state:" << frm->win_state();
            frame->set_active();
            frame->raise_it();
        }
        else
        {
            qDebug() << "Frame inactive:"  << frame->winId() << "- Name:" << frame->cl_name() << "- state:" << frm->win_state() << "- is Splash:" << frm->is_splash();
            frame->set_inactive();
        }
    }
}

void Antico::send_configurenotify(Frame *frm)
{
    XConfigureEvent ce;

    ce.type = ConfigureNotify;
    ce.event = frm->cl_win();
    ce.window = ce.event;
    ce.x = frm->cl_x();
    ce.y = frm->cl_y();
    ce.width = frm->cl_w();
    ce.height = frm->cl_h();
    ce.above = None;
    ce.border_width = 0;
    ce.override_redirect = 0;
    XSendEvent(QX11Info::display(), ce.window, False, StructureNotifyMask, (XEvent *)&ce);
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
