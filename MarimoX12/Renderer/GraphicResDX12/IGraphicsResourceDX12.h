#ifndef I_GRAPHICS_RESOURCE_DX12_H
#define I_GRAPHICS_RESOURCE_DX12_H

#include <d3d12.h>
#include <wrl.h>
#include "../d3dx12.h"
#include "../IGraphicsResource.h"

#include <wrl.h>
#include <d3d12.h>
#include "../d3dx12.h"

class IGraphicsResourceDX12 : IGraphicsResource
{
public:
	D3D12_GPU_VIRTUAL_ADDRESS getGpuVirtualAddress() { return m_gpuVirtualAddr; }
	D3D12_RESOURCE_STATES getUsageState() { return m_usageState; }
	D3D12_RESOURCE_STATES getTransitionState() { return m_transitionState; }
	Microsoft::WRL::ComPtr<ID3D12Resource> getResource() { return m_pResource; }

	virtual void destroyResource();
protected:
	Microsoft::WRL::ComPtr<ID3D12Resource> m_pResource;
	D3D12_RESOURCE_STATES m_usageState;
	D3D12_RESOURCE_STATES m_transitionState;
	D3D12_GPU_VIRTUAL_ADDRESS m_gpuVirtualAddr;
};

#endif