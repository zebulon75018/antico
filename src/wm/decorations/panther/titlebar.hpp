#ifndef _TITLEBAR_HPP
#define _TITLEBAR_HPP

#include <QFrame>

#include "pantherdecoration.hpp"

class QLabel;

class TitleBar: public QWidget
{
    Q_OBJECT

public:
    TitleBar(QWidget *parent = 0);

    void setTitle(const QString &title);

protected:
    virtual void paintEvent(QPaintEvent *event);

private:
    QLabel *_title;

signals:
    void buttonClicked(PantherDecoration::ButtonType);
};

#endif
