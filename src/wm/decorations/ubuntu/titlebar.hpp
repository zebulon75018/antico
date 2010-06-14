#ifndef _TITLEBAR_HPP
#define _TITLEBAR_HPP

#include <QFrame>

#include "ubuntudecoration.hpp"

class ButtonContainer;

class TitleBar: public QWidget
{
    Q_OBJECT

public:
    TitleBar(QWidget *parent = 0);

protected:
    virtual void paintEvent(QPaintEvent *event);

private:
    ButtonContainer *_buttonContainer;

signals:
    void buttonClicked(UbuntuDecoration::ButtonType);
};

#endif
