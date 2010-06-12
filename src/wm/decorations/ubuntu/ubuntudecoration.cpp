#include "ubuntudecoration.hpp"
#include "titlebar.hpp"

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
    return BorderSize(32, 2, 2, 2);
}
