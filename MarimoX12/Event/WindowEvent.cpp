#include "WindowEvent.h"

//////////////////////////////////////////////////////////////////////////

EventWindow::EventWindow(HWND hWnd, unsigned int wParam, unsigned int lParam) :
	Event(L"wnd_msg")
{

}

EventWindow::EventWindow(HWND hWnd, unsigned int wParam, unsigned int lParam, const std::wstring& id) :
	Event(id)
{

}

EventWindow::~EventWindow()
{

}

//////////////////////////////////////////////////////////////////////////
// window resize
EventWindowResize::EventWindowResize(HWND hWnd, unsigned int wParam, unsigned int lParam) :
	EventWindow(hWnd, wParam, lParam, L"wnd_resize")
{
	
}

EventWindowResize::~EventWindowResize()
{

}

//////////////////////////////////////////////////////////////////////////
// mouse
EventMouse::EventMouse(HWND hWnd, unsigned int wParam, unsigned int lParam, const std::wstring& name) :
	EventWindow(hWnd, wParam, lParam, L"mouse")
{

}

EventMouse::~EventMouse()
{

}

//////////////////////////////////////////////////////////////////////////
// mouse left button
EventMouseLButtonDown::EventMouseLButtonDown(HWND hWnd, unsigned int wParam, unsigned int lParam) :
	EventMouse(hWnd, wParam, lParam, L"mouse_lbtndown")
{

}

EventMouseLButtonDown::~EventMouseLButtonDown()
{

}

EventMouseLButtonUp::EventMouseLButtonUp(HWND hWnd, unsigned int wParam, unsigned int lParam) :
	EventMouse(hWnd, wParam, lParam, L"mouse_lbtnup")
{

}

EventMouseLButtonUp::~EventMouseLButtonUp()
{

}

//////////////////////////////////////////////////////////////////////////
// middle button
EventMouseMButtonDown::EventMouseMButtonDown(HWND hWnd, unsigned int wParam, unsigned int lParam) :
	EventMouse(hWnd, wParam, lParam, L"mouse_mbtndown")
{

}

EventMouseMButtonDown::~EventMouseMButtonDown()
{

}


EventMouseMButtonUp::EventMouseMButtonUp(HWND hWnd, unsigned int wParam, unsigned int lParam) :
	EventMouse(hWnd, wParam, lParam, L"mouse_mbtnup")
{

}

EventMouseMButtonUp::~EventMouseMButtonUp()
{

}

//////////////////////////////////////////////////////////////////////////
// right button
EventMouseRButtonDown::EventMouseRButtonDown(HWND hWnd, unsigned int wParam, unsigned int lParam) :
	EventMouse(hWnd, wParam, lParam, L"mouse_rbtndown")
{

}

EventMouseRButtonDown::~EventMouseRButtonDown()
{

}

EventMouseRButtonUp::EventMouseRButtonUp(HWND hWnd, unsigned int wParam, unsigned int lParam) :
	EventMouse(hWnd, wParam, lParam, L"mouse_rbtnup")
{

}

EventMouseRButtonUp::~EventMouseRButtonUp()
{

}

//////////////////////////////////////////////////////////////////////////
// mouse move
EventMouseMove::EventMouseMove(HWND hWnd, unsigned int wParam, unsigned int lParam) :
	EventMouse(hWnd, wParam, lParam, L"mouse_move")
{

}

EventMouseMove::~EventMouseMove()
{

}

//////////////////////////////////////////////////////////////////////////
// mouse wheel
EventMouseWheel::EventMouseWheel(HWND hWnd, unsigned int wParam, unsigned int lParam):
	EventMouse(hWnd, wParam, lParam, L"mouse_wheel")
{

}

EventMouseWheel::~EventMouseWheel()
{

}

//////////////////////////////////////////////////////////////////////////
// keyboard
EventKeyboard::EventKeyboard(HWND hWnd, unsigned int wParam, unsigned int lParam) :
	EventWindow(hWnd, wParam, lParam, L"key")
{

}

EventKeyboard::EventKeyboard(HWND hWnd, unsigned int wParam, unsigned int lParam, const std::wstring& name) :
	EventWindow(hWnd, wParam, lParam, name)
{

}

EventKeyboard::~EventKeyboard()
{

}

//////////////////////////////////////////////////////////////////////////
// key down
EventKeyDown::EventKeyDown(HWND hWnd, unsigned int wParam, unsigned int lParam) :
	EventKeyboard(hWnd, wParam, lParam, L"key_down")
{

}

EventKeyDown::~EventKeyDown()
{

}
// key up
EventKeyUp::EventKeyUp(HWND hWnd, unsigned int wParam, unsigned int lParam) :
	EventKeyboard(hWnd, wParam, lParam, L"key_up")
{

}

EventKeyUp::~EventKeyUp()
{

}

//////////////////////////////////////////////////////////////////////////
// key char
EventChar::EventChar(HWND hWnd, unsigned int wParam, unsigned int lParam) :
	EventKeyboard(hWnd, wParam, lParam, L"key_char")
{

}

EventChar::~EventChar()
{

}
