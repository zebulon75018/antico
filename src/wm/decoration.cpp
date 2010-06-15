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
            QRect rect = geometry();
            QSize size(rect.width() + pos.x(), rect.height() + pos.y());
            client()->resize(size);
        }
    }
    else
    {
        // Always assumes that the pointer is over an resizable area
        _hoverResizeArea = true;

        switch (pointGravity(e->pos()))
        {
            case NorthWestGravity:
                setCursor(Qt::SizeFDiagCursor);
                break;

            case NorthEastGravity:
                setCursor(Qt::SizeBDiagCursor);
                break;

            case SouthWestGravity:
                setCursor(Qt::SizeBDiagCursor);
                break;

            case SouthEastGravity:
                setCursor(Qt::SizeFDiagCursor);
                break;

            default:
                setCursor(Qt::ArrowCursor);
                _hoverResizeArea = false; // not
        }
    }
}

int Decoration::pointGravity(const QPoint &p)
{
    BorderSize border = borderSize();
    QRect rect = client()->geometry();

    // top-left
    if (p.x() <= border.left() + 10 && p.y() <= border.top() + 10)
    {
        return NorthWestGravity;
    }
    // top-right
    else if(p.x() >= (rect.width() + border.measuredWidth()) - 10 &&
            p.y() <= border.top() + 10)
    {
        return NorthEastGravity;
    }
    // bottom-left
    else if(p.x() <= border.left() + 10 &&
            p.y() >= (rect.height() + border.measuredHeight()) - border.bottom() - 10)
    {
        return SouthWestGravity;
    }
    // bottom-right
    else if(p.x() >= (rect.width() + border.measuredWidth()) - 10 &&
            p.y() >= (rect.height() + border.measuredHeight()) - border.bottom() - 10)
    {
        return SouthEastGravity;
    }

    return ForgetGravity;
}
