#ifndef I_RENDER_DEVICE_H
#define I_RENDER_DEVICE_H

#include "../Platform/PlatformWindow.h"

class IRenderDevice
{
public:
	virtual bool initializeDevice(IPlatformWindow* wnd) = 0;
};

#endif