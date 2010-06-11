#ifndef _CLIENT_HPP
#define _CLIENT_HPP

#include <QObject>
#include <QRect>

class Decoration;

class Client: public QObject
{
public:
    Client(Qt::HANDLE winId, QObject *parent);
    ~Client();

    void map();
    void move(const QPoint &p);

private:
    Qt::HANDLE _winId;
    Decoration *_decoration;
    QRect _geometry;
};

#endif
