#ifndef _PANTHERDECORATION_HPP
#define _PANTHERDECORATION_HPP

#include "decoration.hpp"

class TitleBar;

class PantherDecoration: public Decoration
{
    Q_OBJECT

public:
    enum ButtonType
    {
        CloseButton,
        MinimizeButton,
        MaximizeButton
    };

    PantherDecoration(Client *c);

    BorderSize borderSize() const;

    void setTitle(const QString &title);

protected:
    virtual void paintEvent(QPaintEvent *e);

private slots:
    void buttonClicked(PantherDecoration::ButtonType button);

private:
    TitleBar *_titleBar;
};

#endif
