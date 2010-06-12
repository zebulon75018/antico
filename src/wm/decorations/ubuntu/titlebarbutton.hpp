#ifndef _TITLEBARBUTTON_HPP
#define _TITLEBARBUTTON_HPP

#include <QLabel>

#include "titlebar.hpp"

class TitleBarButton: public QLabel
{
    Q_OBJECT

public:
    TitleBarButton(TitleBar::ButtonType type, QWidget *parent);

    inline TitleBar::ButtonType type() const { return _type; }

protected:
    virtual void mousePressEvent(QMouseEvent *event);

private:
    TitleBar::ButtonType _type;

signals:
    void clicked();
};

#endif
