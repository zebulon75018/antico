#include "decoration.hpp"
#include "client.hpp"
#include "windowmanager.hpp"

#include <QMouseEvent>
#include <QDebug>
#include <QApplication>

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
    return false;
}

void Decoration::mousePressEvent(QMouseEvent *e)
{
    WindowManager::self()->setActiveClient(client());

    setMoveOffset(e->pos());
    _resizeGravity = pointGravity(e->pos());
}

void Decoration::mouseReleaseEvent(QMouseEvent *e)
{
    setMoveOffset(QPoint(0, 0)); // Clear offset
    _resizeGravity = ForgetGravity;

    if (pointGravity(e->pos()) == ForgetGravity)
    {
        QApplication::setOverrideCursor(Qt::ArrowCursor);
    }
}

void Decoration::mouseMoveEvent(QMouseEvent *e)
{
    if (e->buttons() & Qt::LeftButton && !moveOffset().isNull())
    {
        // Moving the window
        if (!_hoverResizeArea)
        {
            QApplication::setOverrideCursor(Qt::ClosedHandCursor);
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
                QApplication::setOverrideCursor(Qt::SizeVerCursor);
                break;

            case WestGravity:
            case EastGravity:
                QApplication::setOverrideCursor(Qt::SizeHorCursor);
                break;

            case NorthWestGravity:
            case SouthEastGravity:
                QApplication::setOverrideCursor(Qt::SizeFDiagCursor);
                break;

            case NorthEastGravity:
            case SouthWestGravity:
                QApplication::setOverrideCursor(Qt::SizeBDiagCursor);
                break;

            default:
                QApplication::setOverrideCursor(Qt::ArrowCursor);
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
