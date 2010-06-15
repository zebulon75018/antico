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

    // These methods are interesting if you want to create a custom decoration     
    void move(const QPoint &p);
    void minimize();
    void maximize();
    void close();

    inline Decoration *decoration() const { return _decoration; }

    inline QRect geometry() const { return _geometry; }

protected:
    friend class WindowManager;

    void map();

private:
    Qt::HANDLE _winId;
    Decoration *_decoration;
    QRect _geometry;
};

#endif
