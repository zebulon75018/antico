#ifndef _CLIENT_HPP
#define _CLIENT_HPP

#include <QObject>
#include <QX11Info>

class Dockbar;
class Desk;
class Frame;

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

    // Somewhere in the future will not need the pointer to DockBar and Desk
    Client(Qt::HANDLE window, const QString &type, Dockbar *dockBar, Desk *desk);

    inline Type windowType() const { return m_windowType; }

private:
    void updateWindowType();

private:
    Qt::HANDLE m_window;
    QString m_type; // TODO: this is ugly!
    Dockbar *m_dockBar;
    Desk *m_desk;
    Frame *m_frame;

    Type m_windowType;
};

#endif
