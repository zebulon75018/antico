#ifndef _TITLEBAR_HPP
#define _TITLEBAR_HPP

#include <QFrame>

class ButtonContainer;

class TitleBar: public QWidget
{
    Q_OBJECT

public:
    enum ButtonType
    {
        CloseButton,
        MinimizeButton,
        MaximizeButton
    };

    TitleBar(QWidget *parent = 0);

protected:
    virtual void paintEvent(QPaintEvent *event);

private:
    ButtonContainer *_buttonContainer;
};

#endif
