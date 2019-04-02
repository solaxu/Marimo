#include "./Platform/PlatformWin32.h"
#include "./Renderer/RenderDeviceDX12.h"

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE prevInstance,
	PSTR cmdLine, int showCmd)
{
	// Enable run-time memory check for debug builds.
#if defined(DEBUG) | defined(_DEBUG)
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
#endif

	Win32Windows theApp;
	theApp.createWindow();

	RenderDeviceDX12 deviceDX12;
	deviceDX12.initializeDevice(Win32Windows::getWindow());

	int code = theApp.run();
	theApp.destroyWindow();
	return code;
}