#ifndef PLATFOEM_WIN32_H
#define PLATFORM_WIN32_H

#include <windows.h>
#include <string>

#include "./PlatformWindow.h"
#include "./Win32EngineTimer.h"

#pragma comment(lib,"d3dcompiler.lib")
#pragma comment(lib, "D3D12.lib")
#pragma comment(lib, "dxgi.lib")

class Win32Windows : public IPlatformWindow
{
public:
	Win32Windows();
	virtual ~Win32Windows();

public:
	virtual bool createWindow();
	virtual void destroyWindow();
public:
	HWND getWindowHandle() { return m_hWnd; }
	int getWindowWidth() { return m_wndWidth; }
	int getWindowHeight() { return m_wndHeight; }
	int getWindowLeft() { return m_wndLeft; }
	int getWindowTop() { return m_wndTop; }

	static Win32Windows* getWindow() { return Win32Windows::s_pWin32Window; };

	virtual int run();
	virtual void wndProc(unsigned int hwnd, unsigned int msg, unsigned int wParam, unsigned int lParam);
	virtual EngineTimer* getTimer();

protected:
	virtual void preCreateWindow();
	

protected:
	HINSTANCE m_hInstance;
	HWND m_hWnd;
	std::wstring m_sCaption;
	Win32EngineTimer m_oTimer;
	int m_wndWidth;
	int m_wndHeight;
	int m_wndLeft;
	int m_wndTop;
	bool m_bPaused;
	
	static Win32Windows* s_pWin32Window;
};

#endif