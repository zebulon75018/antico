#include "client.hpp"
#include "frame.h"

Client::Client(Qt::HANDLE window, const QString &type, Dockbar *dockBar, Desk *desk):
    QObject(),
    m_window(window),
    m_type(type),
    m_dockBar(dockBar),
    m_desk(desk)
{
    m_frame = new Frame(window, type, dockBar, desk);
}
