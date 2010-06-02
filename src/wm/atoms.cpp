#include <QX11Info>

#include "atoms.hpp"

static const char *atomNames = {
	"_NET_CLIENT_LIST\0"
	"_NET_ACTIVE_WINDOW\0"

	"_NET_WM_WINDOW_TYPE\0"
	"_NET_WM_WINDOW_TYPE_DESKTOP\0"
	"_NET_WM_WINDOW_TYPE_DOCK\0"
	"_NET_WM_WINDOW_TYPE_SPLASH\0"
	"_NET_WM_WINDOW_TYPE_NORMAL\0"
    "_NET_WM_WINDOW_TYPE_DIALOG\0"

	"_NET_WM_STATE\0"
	"_NET_WM_STATE_STICKY\0"
	"_NET_WM_STATE_SKIP_PAGER\0"
	"_NET_WM_STATE_SKIP_TASKBAR\0"

	"_NET_WM_STRUT_PARTIAL\0"

	"_NET_SYSTEM_TRAY_OPCODE\0"

	"_NET_WM_NAME\0"
	"_NET_WM_ICON\0"

	"WM_DELETE_WINDOW\0"
	"WM_PROTOCOLS\0"
	"WM_STATE\0"

	"UTF8_STRING\0"
};

InternalAtoms *atomList = 0;

void _createAtomList()
{
	atomList = new InternalAtoms;

	const char *names[InternalAtoms::NAtoms];
	const char *ptr = atomNames;

	int i = 0;
	while (*ptr)
	{
		names[i++] = ptr;
		while (*ptr)
			++ptr;
		++ptr;
	}

	Q_ASSERT(i == InternalAtoms::NAtoms);

	for (i = 0; i < InternalAtoms::NAtoms; ++i)
		atomList->atoms[i] = XInternAtom(QX11Info::display(), names[i], False);
}
