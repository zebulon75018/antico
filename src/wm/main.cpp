#include "x11management.hpp"

#include <QX11Info>

#include <X11/Xlib.h>

int main(int argc, char **argv)
{
    Antico a(argc, argv);

    XSelectInput(QX11Info::display(), QX11Info::appRootWindow(),
                 KeyPressMask | KeyReleaseMask | ButtonPressMask | ButtonReleaseMask |
                 KeymapStateMask | ButtonMotionMask | PointerMotionMask | EnterWindowMask |
                 LeaveWindowMask | FocusChangeMask | VisibilityChangeMask |
                 ExposureMask | StructureNotifyMask | SubstructureRedirectMask | SubstructureNotifyMask);

    XClearWindow(QX11Info::display(), QX11Info::appRootWindow());
    XSync(QX11Info::display(), False);

    return a.exec();
}
