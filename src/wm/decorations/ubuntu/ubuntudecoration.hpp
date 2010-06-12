#ifndef _UBUNTUDECORATION_HPP
#define _UBUNTUDECORATION_HPP

#include "decoration.hpp"

class TitleBar;

class UbuntuDecoration: public Decoration
{	
public:
    UbuntuDecoration(Client *c);

    BorderSize borderSize() const;

private:
    TitleBar *_titleBar;
};

#endif
