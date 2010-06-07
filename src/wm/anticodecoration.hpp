#ifndef _ANTICODECORATION_HPP
#define _ANTICODECORATION_HPP

#include "decoration.hpp"

class AnticoDecoration: public Decoration
{
public:
    AnticoDecoration(Client *c);

    BorderSize borderSize() const;

private:
    void readSettings();
};

#endif
