#ifndef _UBUNTUDECORATION_HPP
#define _UBUNTUDECORATION_HPP

#include "decoration.hpp"

class UbuntuDecoration: public Decoration
{
    class TitleBar: public QWidget
    {
    public:
	TitleBar(UbuntuDecoration *d);

    protected:
	virtual void paintEvent(QPaintEvent *e);
    };
	
public:
    UbuntuDecoration(Client *c);

    BorderSize borderSize() const;

private:
    TitleBar *_titleBar;
};

#endif
