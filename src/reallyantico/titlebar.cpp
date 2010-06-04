#include <QPainter>
#include <QMouseEvent>
#include <QApplication>

#include "titlebar.hpp"

TitleBar::TitleBar(const QPixmap &icn, const QString &nm, QWidget *parent) : QWidget(parent)
{
    setContentsMargins(0, 0, 0, 0);
    icon =  icn;
    app_name = nm;
    setToolTip(app_name);
}

TitleBar::~TitleBar()
{
}

void TitleBar::paintEvent(QPaintEvent *)
{
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);
    painter.setPen(title_color);// text color
    painter.drawPixmap(QRect(0, 0, width(), height()), back, QRect(0, 0, back.width(), back.height())); //background
    QString name = QApplication::fontMetrics().elidedText(app_name, Qt::ElideRight, width()-width()/3); // if app_name is too long, add ... at the end
    painter.drawText(QRect(30, 0, width(), height()), Qt::AlignVCenter, name); //app_name
    painter.setRenderHint(QPainter::SmoothPixmapTransform);
    painter.drawPixmap(QRect(5, 2, height()-4, height()-4), icon, QRect(0, 0, icon.width(), icon.height()));//icon
}

void TitleBar::set_pixmap(const QPixmap &active, const QPixmap &inactive, const QColor &clr)
{
    active_back = active;
    inactive_back = inactive;
    title_color = clr;
    
    if (active_state)
        back = active_back;
    else
        back = inactive_back;
    update();
}

void TitleBar::set_active()
{
    back = active_back;
    active_state = true;
    update();
}

void TitleBar::update_name(const QString &nm)
{
    app_name = nm;
    update();
}

void TitleBar::set_inactive()
{
    back = inactive_back;
    active_state = false;
    update();
}

void TitleBar::mousePressEvent(QMouseEvent *event)
{
    if (event->button() == Qt::RightButton)
    {
        emit mouse_right_press();
    }
    else if (event->button() == Qt::LeftButton)
    {
        emit mouse_left_press(event);
        emit mouse_left_press();
    }
    else if (event->button() == Qt::MidButton)
    {
        emit mouse_mid_press();
    }
}

void TitleBar::mouseMoveEvent(QMouseEvent *event)
{
    if (event->buttons() == Qt::LeftButton) // to block right button mouse move
    {
        emit mouse_move(event);
    }
}

void TitleBar::mouseReleaseEvent(QMouseEvent *event)
{
    Q_UNUSED(event);
    emit mouse_release(event);
}

void TitleBar::mouseDoubleClickEvent(QMouseEvent *event)
{
    Q_UNUSED(event);
    emit mouse_double_click();
}
