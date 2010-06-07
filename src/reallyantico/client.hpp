#ifndef _CLIENT_HPP
#define _CLIENT_HPP

#include <QObject>
#include <QX11Info>
#include <QRect>
#include <QSet>

#include <X11/Xdefs.h>

class Frame;

union _XEvent; // avoid Xlib include

class Client: public QObject
{
    Q_OBJECT

public:
    // Supported window types
    enum Type
    {
	UnknownType,
	DesktopType,
	DockType,
	ToolBarType,
	MenuType,
	UtilityType,
	SplashType,
	DialogType,
	NormalType
    };

    // Supported window states
    enum MappingState
    {
	IconicMappingState
    };

    Client(Qt::HANDLE window, QObject *parent);

    bool manage(bool isMapped);
    bool windowEvent(_XEvent *e);

    inline Qt::HANDLE window() const { return m_window; }
    inline Type windowType() const { return m_windowType; }
    inline Frame *frame() const { return m_frame; }

private:
    void updateWindowType();
    void updateGeometry();
    void updateWMHints();
    void updateWMNormalHints();
    void updateWMProtocols();
    void updateIcon();
    void updateName();

    void reparent();

    inline bool hasWmProtocol(Atom protocol) { return m_wmProtocols.contains(protocol); }
    
private:
    Qt::HANDLE m_window;
    QString m_type; // TODO: this is ugly!
    Frame *m_frame;

    Type m_windowType;
    QRect m_geometry;
    MappingState m_mappingState;
    QSet<Atom> m_wmProtocols;
    QString m_name;
};

#endif
