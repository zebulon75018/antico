#include <QDebug>
#include "windowmanager.hpp"
#include "atoms.hpp"

#include <QX11Info>

#include <X11/Xlib.h>

static bool _x11EventFilter(void *message, long *result)
{
    return WindowManager::self()->x11EventFilter(message, result);
}

int main(int argc, char **argv)
{
    QApplication app(argc, argv);

    app.setEventFilter(_x11EventFilter);

    _createAtomList();

    WindowManager::self()->init();

    return app.exec();
}
