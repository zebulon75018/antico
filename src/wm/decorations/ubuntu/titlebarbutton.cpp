#include "titlebarbutton.hpp"

TitleBarButton::TitleBarButton(TitleBar::ButtonType type, QWidget *parent)
    : QLabel(parent)
    , _type(type)
{
    switch (type)
    {
        case TitleBar::CloseButton:
            setPixmap(QPixmap(":/decorations/ubuntu/images/close.png"));
            setToolTip(trUtf8("Close"));
            break;
        case TitleBar::MinimizeButton:
            setPixmap(QPixmap(":/decorations/ubuntu/images/minimize.png"));
            setToolTip(trUtf8("Minimize"));
            break;
        case TitleBar::MaximizeButton:
            setPixmap(QPixmap(":/decorations/ubuntu/images/maximize.png"));
            setToolTip(trUtf8("Maximize"));
            break;
    }
}

void TitleBarButton::mousePressEvent(QMouseEvent *event)
{
    emit clicked();
}
