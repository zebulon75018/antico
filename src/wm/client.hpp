#ifndef _CLIENT_HPP
#define _CLIENT_HPP

#include <QObject>
#include <QX11Info>

class DecoratedFrame;

class Client: public QObject
{
    Q_OBJECT

public:
    Client(Qt::HANDLE window, QObject *parent = 0);

private:
    DecoratedFrame *m_decoratedFrame;
};

#endif // _CLIENT_HPP
