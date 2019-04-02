#include "PlatformWin32.h"
#include "../Event/WindowEvent.h"
#include "../Event/FrameEvent.h"

Win32Windows* Win32Windows::s_pWin32Window = NULL;

LRESULT CALLBACK
MainWndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	if (Win32Windows::getWindow() != NULL)
	{
		Win32Windows::getWindow()->wndProc((unsigned int)hwnd, (unsigned int)msg, (unsigned int)wParam, (unsigned int)lParam);
	}
	return ::DefWindowProc(hwnd, msg, wParam, lParam);
}


Win32Windows::Win32Windows() :
	m_hWnd(NULL),m_hInstance(NULL),
	m_wndLeft(0), m_wndTop(0),
	m_wndWidth(1024), m_wndHeight(768),
	m_sCaption(L"MarimoX12"),
	m_bPaused(false)
{
	Win32Windows::s_pWin32Window = this;
}

Win32Windows::~Win32Windows()
{

}

bool Win32Windows::createWindow()
{
	this->preCreateWindow();

	WNDCLASS wc;
	wc.style = CS_HREDRAW | CS_VREDRAW;
	wc.lpfnWndProc = MainWndProc;
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.hInstance = m_hInstance;
	wc.hIcon = LoadIcon(0, IDI_APPLICATION);
	wc.hCursor = LoadCursor(0, IDC_ARROW);
	wc.hbrBackground = (HBRUSH)GetStockObject(NULL_BRUSH);
	wc.lpszMenuName = 0;
	wc.lpszClassName = m_sCaption.c_str();

	if (!RegisterClass(&wc))
	{
		MessageBox(0, L"RegisterClass Failed.", 0, 0);
		return false;
	}

	// Compute window rectangle dimensions based on requested client area dimensions.
	RECT R = { 0, 0, m_wndWidth, m_wndHeight };
	AdjustWindowRect(&R, WS_OVERLAPPEDWINDOW, false);
	int width = R.right - R.left;
	int height = R.bottom - R.top;

	m_hWnd = CreateWindow(m_sCaption.c_str(), m_sCaption.c_str(),
		WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT, width, height, 0, 0, m_hInstance, 0);
	if (!m_hWnd)
	{
		MessageBox(0, L"CreateWindow Failed.", 0, 0);
		return false;
	}

	ShowWindow(m_hWnd, SW_SHOW);
	UpdateWindow(m_hWnd);

	return true;
}

void Win32Windows::destroyWindow()
{
	if (m_hWnd)
	{
		::DestroyWindow(m_hWnd);
		m_hWnd = NULL;
	}
}

void Win32Windows::preCreateWindow()
{
	m_hInstance = GetModuleHandle(0);
}

EngineTimer* Win32Windows::getTimer()
{
	return &m_oTimer;
}

int Win32Windows::run()
{
	MSG msg = { 0 };

	m_oTimer.reset();

	while (msg.message != WM_QUIT)
	{
		// If there are Window messages then process them.
		if (PeekMessage(&msg, 0, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		// Otherwise, do animation/game stuff.
		else
		{
			m_oTimer.tick();

			if (!m_bPaused)
			{
				//CalculateFrameStats();
				//Update(mTimer);
				//Draw(mTimer);
				fireEvent(L"frame_begin", EventFrameBegin());
			}
			else
			{
				Sleep(100);
			}
		}
	}

	return (int)msg.wParam;
}

void Win32Windows::wndProc(unsigned int hwnd, unsigned int msg, unsigned int wParam, unsigned int lParam)
{
	switch (msg)
	{
	case WM_DESTROY:
		PostQuitMessage(0);
		return;
	case WM_SIZE:
		RECT rc;
		GetClientRect(m_hWnd, &rc);
		m_wndWidth = rc.right - rc.left;
		m_wndHeight = rc.bottom - rc.top;
		fireEvent(L"wnd_resize", EventWindowResize((HWND)hwnd, wParam, lParam));
		break;
	default:
		break;
	}
}
