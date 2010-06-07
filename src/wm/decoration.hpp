#ifndef _DECORATION_HPP
#define _DECORATION_HPP

#include "bordersize.hpp"

#include <QWidget>

class Client;

class Decoration: public QWidget
{
public:
    Decoration(Client *c)
        : QWidget()
    {
    }

    virtual BorderSize borderSize() const = 0;

    inline Client *client() const { return _client; }

private:
    Client *_client;
};

#endif
