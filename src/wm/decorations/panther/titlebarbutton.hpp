#ifndef _TITLEBARBUTTON_HPP
#define _TITLEBARBUTTON_HPP

#include <QLabel>

#include "pantherdecoration.hpp"

class TitleBarButton: public QLabel
{
    Q_OBJECT

public:
    TitleBarButton(PantherDecoration::ButtonType type, QWidget *parent);

    inline PantherDecoration::ButtonType type() const { return _type; }

protected:
    virtual void mousePressEvent(QMouseEvent *event);

private:
    PantherDecoration::ButtonType _type;

signals:
    void clicked();
};

#endif
