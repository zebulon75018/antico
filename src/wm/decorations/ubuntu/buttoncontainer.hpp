#ifndef _BUTTONCONTAINER_HPP
#define _BUTTONCONTAINER_HPP

#include <QWidget>
#include <QMap>

#include "titlebar.hpp"

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
    void addButton(TitleBar::ButtonType type);
    inline TitleBarButton *button(TitleBar::ButtonType type) { return _buttons.value(type); }

private slots:
    void buttonClicked(int button);

private:
    QMap<TitleBar::ButtonType, TitleBarButton *> _buttons;
    QSignalMapper *_signalMapper;

signals:
    void clicked(TitleBar::ButtonType);
};

#endif
