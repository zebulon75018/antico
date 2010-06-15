#include "decoration.hpp"
#include "client.hpp"

#include <QMouseEvent>

Decoration::Decoration(Client *c)
    : QWidget()
    , _client(c)
{
    setMouseTracking(true);
}

bool Decoration::x11EventFilter(_XEvent *e)
{
    return false;
}

void Decoration::mousePressEvent(QMouseEvent *e)
{
    setMoveOffset(e->pos());
}

void Decoration::mouseReleaseEvent(QMouseEvent *e)
{
    setMoveOffset(QPoint(0, 0)); // Clear offset
    setCursor(Qt::ArrowCursor);
}

void Decoration::mouseMoveEvent(QMouseEvent *e)
{
    if (e->buttons() & Qt::LeftButton && !moveOffset().isNull())
    {
	setCursor(Qt::ClosedHandCursor);
	client()->move(mapToGlobal(e->pos()) - moveOffset());
    }
}
