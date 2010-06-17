#include "titlebarbutton.hpp"

TitleBarButton::TitleBarButton(UbuntuDecoration::ButtonType type, QWidget *parent)
    : QLabel(parent)
    , _type(type)
{
    setMouseTracking(true);
    setActive(true);

    switch (type)
    {
        case UbuntuDecoration::CloseButton:
            setToolTip(trUtf8("Close"));
            break;
        case UbuntuDecoration::MinimizeButton:
            setToolTip(trUtf8("Minimize"));
            break;
        case UbuntuDecoration::MaximizeButton:
            setToolTip(trUtf8("Maximize"));
            break;
    }
}

void TitleBarButton::setActive(bool active)
{
    switch (_type)
    {
        case UbuntuDecoration::CloseButton:
	    if (active)
		setPixmap(QPixmap(":/decorations/ubuntu/images/close.png"));
	    else
		setPixmap(QPixmap(":/decorations/ubuntu/images/close_unfocused.png"));
            break;
        case UbuntuDecoration::MinimizeButton:
	    if (active)
		setPixmap(QPixmap(":/decorations/ubuntu/images/minimize.png"));
	    else
		setPixmap(QPixmap(":/decorations/ubuntu/images/minimize_unfocused.png"));
            break;
        case UbuntuDecoration::MaximizeButton:
	    if (active)
		setPixmap(QPixmap(":/decorations/ubuntu/images/maximize.png"));
	    else
		setPixmap(QPixmap(":/decorations/ubuntu/images/maximize_unfocused.png"));
            break;
    }
}

void TitleBarButton::mousePressEvent(QMouseEvent *event)
{
    emit clicked();
}
