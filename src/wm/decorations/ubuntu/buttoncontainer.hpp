#ifndef _BUTTONCONTAINER_HPP
#define _BUTTONCONTAINER_HPP

#include <QFrame>
#include <QMap>

#include "ubuntudecoration.hpp"

class QSignalMapper;
class TitleBarButton;

class ButtonContainer: public QFrame
{
    Q_OBJECT

public:
    ButtonContainer(QWidget *parent);

protected:
    virtual void paintEvent(QPaintEvent *event);

private:
    void addButton(UbuntuDecoration::ButtonType type);
    inline TitleBarButton *button(UbuntuDecoration::ButtonType type) { return _buttons.value(type); }

private slots:
    void buttonClicked(int button);

private:
    QMap<UbuntuDecoration::ButtonType, TitleBarButton *> _buttons;
    QSignalMapper *_signalMapper;

signals:
    void clicked(UbuntuDecoration::ButtonType);
};

#endif
