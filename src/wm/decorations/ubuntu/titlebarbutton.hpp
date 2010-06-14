#ifndef _TITLEBARBUTTON_HPP
#define _TITLEBARBUTTON_HPP

#include <QLabel>

#include "ubuntudecoration.hpp"

class TitleBarButton: public QLabel
{
    Q_OBJECT

public:
    TitleBarButton(UbuntuDecoration::ButtonType type, QWidget *parent);

    inline UbuntuDecoration::ButtonType type() const { return _type; }

protected:
    virtual void mousePressEvent(QMouseEvent *event);

private:
    UbuntuDecoration::ButtonType _type;

signals:
    void clicked();
};

#endif
