#include "titlebarbutton.hpp"

TitleBarButton::TitleBarButton(UbuntuDecoration::ButtonType type, QWidget *parent)
    : QLabel(parent)
    , _type(type)
{
    setMouseTracking(true);

    switch (type)
    {
        case UbuntuDecoration::CloseButton:
            setPixmap(QPixmap(":/decorations/ubuntu/images/close.png"));
            setToolTip(trUtf8("Close"));
            break;
        case UbuntuDecoration::MinimizeButton:
            setPixmap(QPixmap(":/decorations/ubuntu/images/minimize.png"));
            setToolTip(trUtf8("Minimize"));
            break;
        case UbuntuDecoration::MaximizeButton:
            setPixmap(QPixmap(":/decorations/ubuntu/images/maximize.png"));
            setToolTip(trUtf8("Maximize"));
            break;
    }
}

void TitleBarButton::mousePressEvent(QMouseEvent *event)
{
    emit clicked();
}
