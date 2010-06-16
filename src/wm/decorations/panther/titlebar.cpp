#include "titlebar.hpp"

#include <QPainter>
#include <QHBoxLayout>
#include <QLabel>
#include <QMouseEvent>
#include <QBitmap>
#include <QDebug>
#include <QApplication>

TitleBar::TitleBar(QWidget *parent):
    QWidget(parent)
{
    setMouseTracking(true);

    QHBoxLayout *layout = new QHBoxLayout(this);
    layout->setSpacing(20);
    layout->setContentsMargins(10, 2, 2, 2);

    QFont f(font());
    f.setBold(true);
    f.setPixelSize(12);

    QPalette pal(palette());
    pal.setColor(QPalette::WindowText, QColor(0, 0, 0));

    _title = new QLabel(this);
    _title->setMouseTracking(true);
    _title->setFont(f);
    _title->setPalette(pal);
    _title->setSizePolicy(QSizePolicy::Ignored, QSizePolicy::Preferred);

    layout->addWidget(_title);
}

void TitleBar::setTitle(const QString &title)
{
    _title->setText(title);
}

void TitleBar::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);
    painter.setRenderHints(QPainter::Antialiasing | QPainter::SmoothPixmapTransform);

    QLinearGradient grad(QPointF(rect().left(), rect().top()), QPointF(rect().left(), rect().bottom()));
    grad.setColorAt(0, QColor(239, 239, 239));
    grad.setColorAt(1, QColor(202, 202, 202));

    painter.setPen(Qt::NoPen);
    painter.setBrush(grad);
    painter.drawRect(rect());

    painter.setPen(QColor(249, 249, 249));
    painter.drawLine(0, 0, rect().width(), 0);

    painter.setPen(QColor(140, 140, 140));
    painter.drawLine(0, rect().height(), rect().width(), rect().height());
}
