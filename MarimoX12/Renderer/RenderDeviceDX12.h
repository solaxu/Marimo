#ifndef REDNER_DEVICE_DX12_H
#define REDNER_DEVICE_DX12_H

#include <wrl.h>
#include <d3d12sdklayers.h>
#include <dxgi1_4.h>
#include <d3d12.h>
#include <D3Dcompiler.h>
#include <DirectXMath.h>
#include <DirectXPackedVector.h>
#include <DirectXColors.h>
#include <DirectXCollision.h>

#include "IRenderDevice.h"
#include "d3dx12.h"

#define SWAP_BUFFER_COUNT 2

class RenderDeviceDX12 : public IRenderDevice
{
public:
	RenderDeviceDX12();
	virtual ~RenderDeviceDX12();

public:
	virtual bool initializeDevice(IPlatformWindow* wnd);
	virtual void onDraw(const Event& event);
protected:
	void createCommandObjects();
	void createSwapChain();
	void createRtvAndDsvDescriptorHeaps();
	void onResize(const Event& event);
	void flushCommandQueue();
	void destroyDevice();
protected:
	static RenderDeviceDX12* getRenderDevice() { return RenderDeviceDX12::s_pDeviceX12; }

protected:

	Microsoft::WRL::ComPtr<IDXGIFactory4> m_dxgiFactory;
	Microsoft::WRL::ComPtr<IDXGISwapChain> m_swapChain;
	Microsoft::WRL::ComPtr<ID3D12Device> m_d3dDevice;

	Microsoft::WRL::ComPtr<ID3D12Fence> m_fence;
	UINT64 m_currentFence = 0;

	Microsoft::WRL::ComPtr<ID3D12CommandQueue> m_commandQueue;
	Microsoft::WRL::ComPtr<ID3D12CommandAllocator> m_directCmdListAlloc;
	Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList> m_commandList;

	int m_currBackBuffer = 0;
	Microsoft::WRL::ComPtr<ID3D12Resource> m_swapChainBuffer[2];
	Microsoft::WRL::ComPtr<ID3D12Resource> m_depthStencilBuffer;

	Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> m_rtvHeap;
	Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> m_dsvHeap;

	D3D12_VIEWPORT m_screenViewPort;
	D3D12_RECT m_scissorRect;

	UINT m_rtvDescriptorSize = 0;
	UINT m_dsvDescriptorSize = 0;
	UINT m_cbvSrvUavDescriptorSize = 0;

	D3D_DRIVER_TYPE m_d3dDriverType = D3D_DRIVER_TYPE_HARDWARE;
	DXGI_FORMAT m_backBufferFormat = DXGI_FORMAT_R8G8B8A8_UNORM;
	DXGI_FORMAT m_depthStencilFormat = DXGI_FORMAT_D24_UNORM_S8_UINT;

	IPlatformWindow* m_pWindow;

	static RenderDeviceDX12* s_pDeviceX12;
};

#endif