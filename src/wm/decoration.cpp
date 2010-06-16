#include "decoration.hpp"
#include "client.hpp"

#include <QMouseEvent>
#include <QDebug>

#include <X11/Xlib.h>

Decoration::Decoration(Client *c)
    : QWidget()
    , _client(c)
    , _hoverResizeArea(false)
    , _resizeGravity(ForgetGravity)
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
    _resizeGravity = pointGravity(e->pos());
}

void Decoration::mouseReleaseEvent(QMouseEvent *e)
{
    setMoveOffset(QPoint(0, 0)); // Clear offset
    _resizeGravity = ForgetGravity;
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
            QRect rect = geometry();
            QPoint pos = e->pos() - moveOffset();
            int width = rect.width();
            int height = rect.height();

            switch (_resizeGravity)
            {
                case NorthGravity:
                    height = rect.height() - pos.y();
                    break;

                case NorthWestGravity:
                {
                    width = rect.width() - pos.x();
                    height = rect.height() - pos.y();
                    break;
                }

                case WestGravity:
                    width = rect.width() - pos.x();
                    break;
                    
                case NorthEastGravity:
                {
                    width = rect.width() - (pos.x() * -1);
                    height = rect.height() - pos.y();
                    setMoveOffset(QPoint(width, 0));
                    break;
                }
                    
                case SouthWestGravity:
                {
                    width = rect.width() - pos.x();
                    height = rect.height() - (pos.y() * -1);
                    setMoveOffset(QPoint(0, height));
                    break;
                }
                    
                case SouthEastGravity:
                    width = rect.width() - (pos.x() * -1);
                    height = rect.height() - (pos.y() * -1);
                    setMoveOffset(QPoint(width, height));
                    break;
            }

            client()->resize(QSize(width, height), _resizeGravity);
        }
    }
    else
    {
        // Always assumes that the pointer is over an resizable area
        _hoverResizeArea = true;

        switch (pointGravity(e->pos()))
        {
            case NorthGravity:
            case SouthGravity:
                setCursor(Qt::SizeVerCursor);
                break;

            case WestGravity:
            case EastGravity:
                setCursor(Qt::SizeHorCursor);
                break;

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
    else if (p.y() <= border.top() + 10)
    {
        return NorthGravity;
    }
    else if (p.y() >= (rect.height() + border.measuredHeight()) - border.bottom() - 10)
    {
        return SouthGravity;
    }
    else if (p.x() <= border.left() + 10)
    {
        return WestGravity;
    }
    else if (p.x() >= (rect.width() + border.measuredWidth()) - 10)
    {
        return EastGravity;
    }

    return ForgetGravity;
}
