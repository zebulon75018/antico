#include "client.hpp"
#include "frame.h"
#include "atoms.hpp"

#include <X11/Xatom.h>

Client::Client(Qt::HANDLE window, const QString &type, Dockbar *dockBar, Desk *desk):
    QObject(),
    m_window(window),
    m_type(type),
    m_dockBar(dockBar),
    m_desk(desk)
{
    m_frame = new Frame(window, type, dockBar, desk);
}

void Client::updateWindowType()
{
    Atom typeReturn;
    int formatReturn;
    unsigned long count, unused;
    unsigned char *data = NULL;

    if (XGetWindowProperty(QX11Info::display(), m_window, ATOM(_NET_WM_WINDOW_TYPE),
			   0l, 2048l, False, XA_ATOM, &typeReturn, &formatReturn,
			   &count, &unused, &data) == Success)
    {
	if (typeReturn == XA_ATOM && formatReturn == 32 && count > 0)
	{
	    long *types = (long *)data;

	    for (unsigned long i = 0; i < count; ++i)
	    {
		if ((Atom)types[i] == ATOM(_NET_WM_WINDOW_TYPE_NORMAL))
		    m_windowType = NormalType;
		if ((Atom)types[i] == ATOM(_NET_WM_WINDOW_TYPE_DESKTOP))
		    m_windowType = DesktopType;
		if ((Atom)types[i] == ATOM(_NET_WM_WINDOW_TYPE_DOCK))
		    m_windowType = DockType;
		if ((Atom)types[i] == ATOM(_NET_WM_WINDOW_TYPE_TOOLBAR))
		    m_windowType = ToolBarType;
		if ((Atom)types[i] == ATOM(_NET_WM_WINDOW_TYPE_MENU))
		    m_windowType = MenuType;
		if ((Atom)types[i] == ATOM(_NET_WM_WINDOW_TYPE_DIALOG))
		    m_windowType = DialogType;
		if ((Atom)types[i] == ATOM(_NET_WM_WINDOW_TYPE_UTILITY))
		    m_windowType = UtilityType;
		if ((Atom)types[i] == ATOM(_NET_WM_WINDOW_TYPE_SPLASH))
		    m_windowType = SplashType;
	    }
	}
    }

    if (data)
	XFree(data);
}
