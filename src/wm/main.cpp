#include "windowmanager.hpp"

#include <QApplication>

bool x11EventFilter(void *message, long *result)
{
    return WindowManager::self()->x11EventFilter(reinterpret_cast<XEvent *>(message));
}

int main(int argc, char **argv)
{
    QApplication app(argc, argv);
    app.setQuitOnLastWindowClosed(false);

    WindowManager::self()->init();

    app.setEventFilter(x11EventFilter);

    return app.exec();
}
