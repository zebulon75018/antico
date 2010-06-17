#ifndef _UBUNTUDECORATION_HPP
#define _UBUNTUDECORATION_HPP

#include "decoration.hpp"

class TitleBar;

class UbuntuDecoration: public Decoration
{
    Q_OBJECT

public:
    enum ButtonType
    {
        CloseButton,
        MinimizeButton,
        MaximizeButton
    };

    UbuntuDecoration(Client *c);

    BorderSize borderSize() const;

    void setTitle(const QString &title);

    void setActive();
    void setInactive();

protected:
    virtual void paintEvent(QPaintEvent *e);

private slots:
    void buttonClicked(UbuntuDecoration::ButtonType button);

private:
    TitleBar *_titleBar;
};

#endif
