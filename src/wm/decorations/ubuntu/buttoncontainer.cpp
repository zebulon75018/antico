#include "buttoncontainer.hpp"
#include "titlebarbutton.hpp"

#include <QHBoxLayout>
#include <QLabel>
#include <QPainter>
#include <QSignalMapper>
#include <QDebug>

ButtonContainer::ButtonContainer(QWidget *parent):
    QFrame(parent)
{
    QHBoxLayout *layout = new QHBoxLayout(this);
    layout->setSpacing(1);
    layout->setContentsMargins(0, 3, 0, 1);

    _signalMapper = new QSignalMapper(this);
    connect(_signalMapper, SIGNAL(mapped(int)), SLOT(buttonClicked(int)));

    addButton(TitleBar::CloseButton);
    addButton(TitleBar::MinimizeButton);
    addButton(TitleBar::MaximizeButton);

    layout->addWidget(button(TitleBar::CloseButton));
    layout->addWidget(button(TitleBar::MinimizeButton));
    layout->addWidget(button(TitleBar::MaximizeButton));

    setSizePolicy(QSizePolicy::Maximum, QSizePolicy::Maximum);
}

void ButtonContainer::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);

    QLinearGradient grad(QPointF(rect().left(), rect().top()), QPointF(rect().left(), rect().bottom()));
    grad.setColorAt(0, QColor(60, 59, 55));
    grad.setColorAt(0.5, QColor(107, 106, 99));

    painter.setPen(Qt::transparent);
    painter.setBrush(grad);
    painter.drawRoundedRect(rect(), 10, 10);
}

void ButtonContainer::addButton(TitleBar::ButtonType type)
{
    TitleBarButton *button = new TitleBarButton(type, this);
    connect(button, SIGNAL(clicked()), _signalMapper, SLOT(map()));
    _signalMapper->setMapping(button, type);
    _buttons.insert(type, button);
}

void ButtonContainer::buttonClicked(int button)
{
    TitleBar::ButtonType type = static_cast<TitleBar::ButtonType>(button);
    emit clicked(type);
}
