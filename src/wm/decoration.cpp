#include "decoration.hpp"
#include "client.hpp"

#include <QMouseEvent>

Decoration::Decoration(Client *c)
    : QWidget()
    , _client(c)
{
    setMouseTracking(true);
}

void Decoration::mousePressEvent(QMouseEvent *e)
{
    setMoveOffset(e->pos());
}

void Decoration::mouseMoveEvent(QMouseEvent *e)
{
    if (e->buttons() & Qt::LeftButton)
	client()->move(mapToGlobal(e->pos()) - moveOffset());
}
