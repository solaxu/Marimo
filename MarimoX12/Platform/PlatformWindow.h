#ifndef IPLATFORM_H
#define IPLATFORM_H

#include "../Event/Event.h"
#include "./EngineTimer.h"

#if defined(DEBUG) || defined(_DEBUG)
#define _CRTDBG_MAP_ALLOC
#include <crtdbg.h>
#endif

class IPlatformWindow : public IEventDispatcher
{
protected:
	virtual void preCreateWindow() = 0;
	virtual void wndProc(unsigned int hwnd, unsigned int msg, unsigned int wParam, unsigned int lParam) = 0;
	virtual int run() = 0;
	virtual EngineTimer* getTimer() = 0;
public:
	virtual bool createWindow() = 0;
	virtual void destroyWindow() = 0;
};

#endif