#include "ubuntudecoration.hpp"

#include <QHBoxLayout>
#include <QPalette>
#include <QLabel>
#include <QPainter>
#include <QLinearGradient>

UbuntuDecoration::UbuntuDecoration(Client *c)
    : Decoration(c)
{
    _titleBar = new TitleBar(this);
    _titleBar->setFixedHeight(borderSize().top());

    QVBoxLayout *layout = new QVBoxLayout(this);
    layout->setContentsMargins(0, 0, 0, 0);
    layout->addWidget(_titleBar);
    layout->addStretch();

    setLayout(layout);
    
    QPalette pal = palette();
    pal.setColor(QPalette::Window, QColor(135, 122, 98));
    setPalette(pal);
}

BorderSize UbuntuDecoration::borderSize() const
{
    return BorderSize(20, 2, 2, 2);
}

UbuntuDecoration::TitleBar::TitleBar(UbuntuDecoration *d)
    : QWidget(d)
{
    QHBoxLayout *layout = new QHBoxLayout(this);
    layout->setSpacing(20);
    layout->setContentsMargins(10, 2, 2, 2);
    layout->addWidget(new QLabel("Test Window"));

    setLayout(layout);
    
    QFont f(font());
    f.setBold(true);
    f.setPointSize(9);
    setFont(f);

    QPalette pal(palette());
    pal.setColor(QPalette::WindowText, QColor(223, 223, 223));
    setPalette(pal);
}

void UbuntuDecoration::TitleBar::paintEvent(QPaintEvent *e)
{    
    QPainter painter(this);
    painter.setRenderHints(QPainter::Antialiasing | QPainter::SmoothPixmapTransform);

    QLinearGradient grad(QPointF(rect().left(), rect().top()), QPointF(rect().left(), rect().bottom()));
    grad.setColorAt(0, QColor(107, 106, 99));
    grad.setColorAt(0.5, QColor(60, 59, 55));

    painter.setPen(Qt::NoPen);
    painter.setBrush(grad);
    painter.drawRect(rect());
}
