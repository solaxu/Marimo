#ifndef WINDOW_EVENT_H
#define WINDOW_EVENT_H

#include <windows.h>
#include "./Event.h"

#ifdef WIN32

class EventWindow : public Event
{
public:
	EventWindow(HWND hWnd, unsigned int wParam, unsigned int lParam);
	EventWindow(HWND hWnd, unsigned int wParam, unsigned int lParam, const std::wstring& id);
	virtual ~EventWindow();

protected:
	HWND m_hWnd;
	WPARAM m_wParam;
	LPARAM m_lParam;
};

//////////////////////////////////////////////////////////////////////////
// window resize

class EventWindowResize : public EventWindow
{
public:
	EventWindowResize(HWND hWnd, unsigned int wParam, unsigned int lParam);
	virtual ~EventWindowResize();

	int newWidth() { return m_lParam & 0xffff; }
	int newHeight() { return m_lParam & 0xffff0000; }
	bool isMaxHide() { return m_wParam == SIZE_MAXHIDE; }
	bool isMaximized() { return m_wParam == SIZE_MAXIMIZED; }
	bool isMaxShow() { return m_wParam == SIZE_MAXSHOW; }
	bool isMinized() { return m_wParam == SIZE_MINIMIZED; }
	bool isRestored() { return m_wParam == SIZE_RESTORED; }
};

//////////////////////////////////////////////////////////////////////////
// mouse event
class EventMouse : public EventWindow
{
public:
	EventMouse(HWND hWnd, unsigned int wParam, unsigned int lParam, const std::wstring& name);
	virtual ~EventMouse();

	bool controlKeydown() { return m_wParam & MK_CONTROL; }
	bool leftButtonDown() { return m_wParam & MK_LBUTTON; }
	bool rightButtonDown() { return m_wParam & MK_RBUTTON; }
	bool middleButtonDown() { return m_wParam & MK_MBUTTON; }
	bool shiftKeyDown() { return m_wParam & MK_SHIFT; }

	int getX() { return m_lParam & 0xffff; }
	int getY() { return (m_lParam & 0xffff0000) >> 16; }
};

//////////////////////////////////////////////////////////////////////////
// left button down
class EventMouseLButtonDown : public EventMouse
{
public:
	EventMouseLButtonDown(HWND hWnd, unsigned int wParam, unsigned int lParam);
	virtual ~EventMouseLButtonDown();
};

// left button up
class EventMouseLButtonUp : public EventMouse
{
public:
	EventMouseLButtonUp(HWND hWnd, unsigned int wParam, unsigned int lParam);
	virtual ~EventMouseLButtonUp();
};

//////////////////////////////////////////////////////////////////////////
// middle button down
class EventMouseMButtonDown : public EventMouse
{
public:
	EventMouseMButtonDown(HWND hWnd, unsigned int wParam, unsigned int lParam);
	virtual ~EventMouseMButtonDown();
};

// middle button up
class EventMouseMButtonUp : public EventMouse
{
public:
	EventMouseMButtonUp(HWND hWnd, unsigned int wParam, unsigned int lParam);
	virtual ~EventMouseMButtonUp();
};

//////////////////////////////////////////////////////////////////////////
// right button down
class EventMouseRButtonDown : public EventMouse
{
public:
	EventMouseRButtonDown(HWND hWnd, unsigned int wParam, unsigned int lParam);
	virtual ~EventMouseRButtonDown();
};

// right button up
class EventMouseRButtonUp : public EventMouse
{
public:
	EventMouseRButtonUp(HWND hWnd, unsigned int wParam, unsigned int lParam);
	virtual ~EventMouseRButtonUp();
};

//////////////////////////////////////////////////////////////////////////
// mouse move
class EventMouseMove : public EventMouse
{
public:
	EventMouseMove(HWND hWnd, unsigned int wParam, unsigned int lParam);
	virtual ~EventMouseMove();
};

//////////////////////////////////////////////////////////////////////////
// mouse wheel
class EventMouseWheel : public EventMouse
{
public:
	EventMouseWheel(HWND hWnd, unsigned int wParam, unsigned int lParam);
	virtual ~EventMouseWheel();

	int getWheelDelta() { return (0xffff0000 & m_wParam) >> 16; }
};

//////////////////////////////////////////////////////////////////////////
// keyboard
class EventKeyboard : public EventWindow
{
public:
	EventKeyboard(HWND hWnd, unsigned int wParam, unsigned int lParam);
	EventKeyboard(HWND hWnd, unsigned int wParam, unsigned int lParam, const std::wstring& name);
	virtual ~EventKeyboard();

	unsigned int getCharacterCode() { return m_wParam; }
	unsigned int getRepeatCount() { return 0xffff & m_lParam; }
	unsigned int getScanCode() { return (0xf0000 & m_lParam) >> 16; }
	bool getExtendedKey() { return (0x01000000 & m_lParam) > 0; }
	bool getContextCode() { return (0x20000000 & m_lParam) > 0; }
	bool getPreviousKeyState() { return (0x40000000 & m_lParam) > 0; }
	bool getTransitionState() { return (0x80000000 & m_lParam) > 0; }
};


//////////////////////////////////////////////////////////////////////////
// key down
class EventKeyDown : public EventKeyboard
{
public:
	EventKeyDown(HWND hWnd, unsigned int wParam, unsigned int lParam);
	virtual ~EventKeyDown();
};

// key up
class EventKeyUp : public EventKeyboard
{
public:
	EventKeyUp(HWND hWnd, unsigned int wParam, unsigned int lParam);
	virtual ~EventKeyUp();
};


//////////////////////////////////////////////////////////////////////////
// key char
class EventChar : public EventKeyboard
{
public:
	EventChar(HWND hWnd, unsigned int wParam, unsigned int lParam);
	virtual ~EventChar();
};

#else

#endif

#endif