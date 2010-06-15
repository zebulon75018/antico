#include "decoration.hpp"
#include "client.hpp"

#include <QMouseEvent>

#include <X11/Xlib.h>

Decoration::Decoration(Client *c)
    : QWidget()
    , _client(c)
    , _hoverResizeArea(false)
{
    setMouseTracking(true);
}

bool Decoration::x11EventFilter(_XEvent *e)
{
    if (e->type == LeaveNotify)
    {
        setCursor(Qt::ArrowCursor);
        return true;
    }

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
        // Moving the window
        if (!_hoverResizeArea)
        {
            setCursor(Qt::ClosedHandCursor);
            client()->move(mapToGlobal(e->pos()) - moveOffset());
        }
        else // Resizing the window
        {
            QPoint pos = e->pos() - moveOffset();
            QRect rect = client()->geometry();
            QSize size(rect.width() + pos.x(), rect.height() + pos.y());
            client()->resize(size);
        }
    }
    else
    {
        QPoint pos = e->pos();
        BorderSize border = borderSize();
        QRect rect = client()->geometry();

        // Always assumes that the pointer is over an resizable area
        _hoverResizeArea = true;

        // top-left
        if (pos.x() <= border.left() + 10 && pos.y() <= border.top() + 10)
        {
            setCursor(Qt::SizeFDiagCursor);
        }
        // top-right
        else if(pos.x() >= (rect.width() + border.measuredWidth()) - 10 &&
                pos.y() <= border.top() + 10)
        {
            setCursor(Qt::SizeBDiagCursor);
        }
        // bottom-left
        else if(pos.x() <= border.left() + 10 &&
                pos.y() >= (rect.height() + border.measuredHeight()) - border.bottom() - 10)
        {
            setCursor(Qt::SizeBDiagCursor);
        }
        // bottom-right
        else if(pos.x() >= (rect.width() + border.measuredWidth()) - 10 &&
                pos.y() >= (rect.height() + border.measuredHeight()) - border.bottom() - 10)
        {
            setCursor(Qt::SizeFDiagCursor);
        }
        else
        {
            setCursor(Qt::ArrowCursor);
            _hoverResizeArea = false; // not
        }
    }
}
