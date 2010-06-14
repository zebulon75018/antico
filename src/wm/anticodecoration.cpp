#include "anticodecoration.hpp"

AnticoDecoration::AnticoDecoration(Client *c)
    : Decoration(c)
{
    readSettings(); // Read settings from the old-style theme
}

BorderSize AnticoDecoration::borderSize() const
{
    return BorderSize(2, 2, 2, 2, 20);
}

void AnticoDecoration::readSettings()
{
}
