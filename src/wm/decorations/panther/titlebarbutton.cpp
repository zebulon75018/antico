#include "titlebarbutton.hpp"

TitleBarButton::TitleBarButton(PantherDecoration::ButtonType type, QWidget *parent)
    : QLabel(parent)
    , _type(type)
{
    setMouseTracking(true);

    switch (type)
    {
        case PantherDecoration::CloseButton:
            setPixmap(QPixmap(":/decorations/ubuntu/images/close.png"));
            setToolTip(trUtf8("Close"));
            break;
        case PantherDecoration::MinimizeButton:
            setPixmap(QPixmap(":/decorations/ubuntu/images/minimize.png"));
            setToolTip(trUtf8("Minimize"));
            break;
        case PantherDecoration::MaximizeButton:
            setPixmap(QPixmap(":/decorations/ubuntu/images/maximize.png"));
            setToolTip(trUtf8("Maximize"));
            break;
    }
}

void TitleBarButton::mousePressEvent(QMouseEvent *event)
{
    emit clicked();
}
