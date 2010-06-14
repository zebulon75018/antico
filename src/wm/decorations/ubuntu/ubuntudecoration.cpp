#include "ubuntudecoration.hpp"
#include "titlebar.hpp"
#include "client.hpp"

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
    connect(_titleBar, SIGNAL(buttonClicked(UbuntuDecoration::ButtonType)),
            SLOT(buttonClicked(UbuntuDecoration::ButtonType)));

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
    return BorderSize(2, 2, 2, 2, 32);
}

void UbuntuDecoration::buttonClicked(UbuntuDecoration::ButtonType button)
{
    switch (button)
    {
        case CloseButton:
            client()->close();
            break;

        case MinimizeButton:
            client()->minimize();
            break;

        case MaximizeButton:
            client()->maximize();
            break;
    }
}

