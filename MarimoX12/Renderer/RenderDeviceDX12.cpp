#include <assert.h>
#include "RenderDeviceDX12.h"
#include "../Platform/PlatformWin32.h"

RenderDeviceDX12* RenderDeviceDX12::s_pDeviceX12 = NULL;

RenderDeviceDX12::RenderDeviceDX12()
{
	assert(RenderDeviceDX12::s_pDeviceX12 == NULL);
	RenderDeviceDX12::s_pDeviceX12 = this;
}

RenderDeviceDX12::~RenderDeviceDX12()
{
	// unregister handlers
	m_pWindow->unregisterListener(L"wnd_resize", L"RenderDeviceDX12::onResize");
}

bool RenderDeviceDX12::initializeDevice(IPlatformWindow* wnd)
{
	Win32Windows* pWnd = dynamic_cast<Win32Windows*>(wnd);
	if (wnd == NULL)
		return false;
	m_pWindow = wnd;
	HWND hWnd = pWnd->getWindowHandle();

	// register event handlers
	pWnd->registerListener<RenderDeviceDX12>(L"wnd_resize", L"RenderDeviceDX12::onResize",
		this, &RenderDeviceDX12::onResize);

	pWnd->registerListener<RenderDeviceDX12>(L"frame_begin", L"RenderDeviceDX12::draw",
		this, &RenderDeviceDX12::onDraw);

#if defined(DEBUG) || defined(_DEBUG)
	{
		Microsoft::WRL::ComPtr<ID3D12Debug> debugController;
		assert(SUCCEEDED(D3D12GetDebugInterface(IID_PPV_ARGS(&debugController))));
		debugController->EnableDebugLayer();
	}
#endif

	assert(SUCCEEDED(CreateDXGIFactory1(IID_PPV_ARGS(&m_dxgiFactory))));
	assert(SUCCEEDED(D3D12CreateDevice(NULL, D3D_FEATURE_LEVEL_12_0, IID_PPV_ARGS(&m_d3dDevice))));
	assert(SUCCEEDED(m_d3dDevice->CreateFence(0, D3D12_FENCE_FLAG_NONE, IID_PPV_ARGS(&m_fence))));

	m_rtvDescriptorSize = m_d3dDevice->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV);
	m_dsvDescriptorSize = m_d3dDevice->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_DSV);
	m_cbvSrvUavDescriptorSize = m_d3dDevice->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);

	this->createCommandObjects();
	this->createSwapChain();
	this->createRtvAndDsvDescriptorHeaps();

	onResize(Event(L""));
	return true;
}

void RenderDeviceDX12::createCommandObjects()
{
	D3D12_COMMAND_QUEUE_DESC queueDesc = {};

	queueDesc.Type = D3D12_COMMAND_LIST_TYPE_DIRECT;
	queueDesc.Flags = D3D12_COMMAND_QUEUE_FLAG_NONE;

	assert(SUCCEEDED(m_d3dDevice->CreateCommandQueue(&queueDesc, IID_PPV_ARGS(&m_commandQueue))));
	assert(SUCCEEDED(m_d3dDevice->CreateCommandAllocator(D3D12_COMMAND_LIST_TYPE_DIRECT,
		IID_PPV_ARGS(m_directCmdListAlloc.GetAddressOf()))));
	assert(SUCCEEDED(m_d3dDevice->CreateCommandList(0, D3D12_COMMAND_LIST_TYPE_DIRECT,
		m_directCmdListAlloc.Get(),
		nullptr,
		IID_PPV_ARGS(m_commandList.GetAddressOf()))));

	m_commandList->Close();
}

void RenderDeviceDX12::createSwapChain()
{
	m_swapChain.Reset();
	Win32Windows* pWnd = dynamic_cast<Win32Windows*>(m_pWindow);

	DXGI_SWAP_CHAIN_DESC sd;
	sd.BufferDesc.Width = pWnd->getWindowWidth();
	sd.BufferDesc.Height = pWnd->getWindowHeight();
	sd.BufferDesc.RefreshRate.Numerator = 60;
	sd.BufferDesc.RefreshRate.Denominator = 1;
	sd.BufferDesc.Format = m_backBufferFormat;
	sd.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
	sd.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;
	sd.SampleDesc.Count = 1;
	sd.SampleDesc.Quality = 0;
	sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	sd.BufferCount = SWAP_BUFFER_COUNT;
	sd.OutputWindow = pWnd->getWindowHandle();
	sd.Windowed = true;
	sd.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD;
	sd.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;

	assert(SUCCEEDED(m_dxgiFactory->CreateSwapChain(
	m_commandQueue.Get(),
		&sd,
		m_swapChain.GetAddressOf())));
}

void RenderDeviceDX12::createRtvAndDsvDescriptorHeaps()
{
	D3D12_DESCRIPTOR_HEAP_DESC rtvHeapDesc;
	rtvHeapDesc.NumDescriptors = SWAP_BUFFER_COUNT;
	rtvHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_RTV;
	rtvHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_NONE;
	rtvHeapDesc.NodeMask = 0;
	assert(SUCCEEDED(m_d3dDevice->CreateDescriptorHeap(
		&rtvHeapDesc, IID_PPV_ARGS(m_rtvHeap.GetAddressOf()))));


	D3D12_DESCRIPTOR_HEAP_DESC dsvHeapDesc;
	dsvHeapDesc.NumDescriptors = 1;
	dsvHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_DSV;
	dsvHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_NONE;
	dsvHeapDesc.NodeMask = 0;
	assert(SUCCEEDED(m_d3dDevice->CreateDescriptorHeap(
		&dsvHeapDesc, IID_PPV_ARGS(m_dsvHeap.GetAddressOf()))));
}

void RenderDeviceDX12::onResize(const Event& event)
{
	if (m_pWindow == NULL)
		return;
	assert(m_d3dDevice);
	assert(m_swapChain);
	assert(m_directCmdListAlloc);
	Win32Windows* pWnd = dynamic_cast<Win32Windows*>(m_pWindow);
	assert(pWnd);

	flushCommandQueue();

	assert(SUCCEEDED(m_commandList->Reset(m_directCmdListAlloc.Get(), nullptr)));

	// resize swap chain buffers and depth-stencil buffer
	for (int i = 0; i < SWAP_BUFFER_COUNT; ++i)
	{
		m_swapChainBuffer[i].Reset();
	}
	m_depthStencilBuffer.Reset();

	CD3DX12_CPU_DESCRIPTOR_HANDLE rtvHeapHandle(m_rtvHeap->GetCPUDescriptorHandleForHeapStart());
	for (UINT i = 0; i < SWAP_BUFFER_COUNT; ++i)
	{
		assert(SUCCEEDED(m_swapChain->GetBuffer(i, IID_PPV_ARGS(&m_swapChainBuffer[i]))));
		m_d3dDevice->CreateRenderTargetView(m_swapChainBuffer[i].Get(), nullptr, rtvHeapHandle);
		rtvHeapHandle.Offset(1, m_rtvDescriptorSize);
	}

	D3D12_RESOURCE_DESC depthStencilDesc;
	depthStencilDesc.Dimension = D3D12_RESOURCE_DIMENSION_TEXTURE2D;
	depthStencilDesc.Alignment = 0;
	depthStencilDesc.Width = pWnd->getWindowWidth();
	depthStencilDesc.Height = pWnd->getWindowHeight();
	depthStencilDesc.DepthOrArraySize = 1;
	depthStencilDesc.MipLevels = 1;
	depthStencilDesc.Format = m_depthStencilFormat;
	depthStencilDesc.SampleDesc.Count = 1;
	depthStencilDesc.SampleDesc.Quality = 0;
	depthStencilDesc.Layout = D3D12_TEXTURE_LAYOUT_UNKNOWN;
	depthStencilDesc.Flags = D3D12_RESOURCE_FLAG_ALLOW_DEPTH_STENCIL;

	D3D12_CLEAR_VALUE optClear;
	optClear.Format = m_depthStencilFormat;
	optClear.DepthStencil.Depth = 1.0f;
	optClear.DepthStencil.Stencil = 0;

	assert(SUCCEEDED(m_d3dDevice->CreateCommittedResource(
		&CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_DEFAULT),
		D3D12_HEAP_FLAG_NONE,
		&depthStencilDesc,
		D3D12_RESOURCE_STATE_COMMON,
		&optClear,
		IID_PPV_ARGS(m_depthStencilBuffer.GetAddressOf())
		)));

	m_d3dDevice->CreateDepthStencilView(m_depthStencilBuffer.Get(), nullptr, 
		m_dsvHeap->GetCPUDescriptorHandleForHeapStart());

	m_commandList->ResourceBarrier(1, &CD3DX12_RESOURCE_BARRIER::Transition(m_depthStencilBuffer.Get(),
		D3D12_RESOURCE_STATE_COMMON, D3D12_RESOURCE_STATE_DEPTH_WRITE));

	assert(SUCCEEDED(m_commandList->Close()));
	ID3D12CommandList* cmdLists[] = { m_commandList.Get() };
	m_commandQueue->ExecuteCommandLists(_countof(cmdLists), cmdLists);

	flushCommandQueue();

	m_screenViewPort.TopLeftX = 0;
	m_screenViewPort.TopLeftY = 0;
	m_screenViewPort.Width = static_cast<float>(pWnd->getWindowWidth());
	m_screenViewPort.Height = static_cast<float>(pWnd->getWindowHeight());
	m_screenViewPort.MinDepth = 0.0f;
	m_screenViewPort.MaxDepth = 1.0f;

	m_scissorRect = { 0, 0, pWnd->getWindowWidth(), pWnd->getWindowHeight() };
}

void RenderDeviceDX12::flushCommandQueue()
{
	m_currentFence++;

	assert(SUCCEEDED(m_commandQueue->Signal(m_fence.Get(), m_currentFence)));

	if (m_fence->GetCompletedValue() < m_currentFence)
	{
		HANDLE eventHandle = CreateEventEx(nullptr, false, false, EVENT_ALL_ACCESS);

		assert(SUCCEEDED(m_fence->SetEventOnCompletion(m_currentFence, eventHandle)));

		WaitForSingleObject(eventHandle, INFINITE);
		CloseHandle(eventHandle);
	}
}

void RenderDeviceDX12::onDraw(const Event& event)
{
	assert(SUCCEEDED(m_directCmdListAlloc->Reset()));
	assert(SUCCEEDED(m_commandList->Reset(m_directCmdListAlloc.Get(), nullptr)));
	m_commandList->ResourceBarrier(1, 
		&CD3DX12_RESOURCE_BARRIER::Transition(m_swapChainBuffer[m_currBackBuffer].Get(),
			D3D12_RESOURCE_STATE_PRESENT, D3D12_RESOURCE_STATE_RENDER_TARGET));

	m_commandList->RSSetViewports(1, &m_screenViewPort);
	m_commandList->RSSetScissorRects(1, &m_scissorRect);

	// clear back buffers and depth-stencil buffer
	m_commandList->ClearRenderTargetView(
		CD3DX12_CPU_DESCRIPTOR_HANDLE(m_rtvHeap->GetCPUDescriptorHandleForHeapStart(),
			m_currBackBuffer, m_rtvDescriptorSize),
		DirectX::Colors::Black, 0, nullptr);

	m_commandList->ClearDepthStencilView(m_dsvHeap->GetCPUDescriptorHandleForHeapStart(),
		D3D12_CLEAR_FLAG_DEPTH | D3D12_CLEAR_FLAG_STENCIL, 1.0f, 0, 0, nullptr);

	// specify the buffers
	m_commandList->OMSetRenderTargets(1,
		&CD3DX12_CPU_DESCRIPTOR_HANDLE(m_rtvHeap->GetCPUDescriptorHandleForHeapStart(),
			m_currBackBuffer, m_rtvDescriptorSize),
		true,
		&m_dsvHeap->GetCPUDescriptorHandleForHeapStart());

	m_commandList->ResourceBarrier(1,
		&CD3DX12_RESOURCE_BARRIER::Transition(
			m_swapChainBuffer[m_currBackBuffer].Get(),
			D3D12_RESOURCE_STATE_RENDER_TARGET, D3D12_RESOURCE_STATE_PRESENT));

	assert(SUCCEEDED(m_commandList->Close()));

	ID3D12CommandList* cmdLists[] = { m_commandList.Get() };
	m_commandQueue->ExecuteCommandLists(_countof(cmdLists), cmdLists);

	assert(SUCCEEDED(m_swapChain->Present(0, 0)));
	m_currBackBuffer = (m_currBackBuffer + 1) % SWAP_BUFFER_COUNT;

	flushCommandQueue();
}

void RenderDeviceDX12::destroyDevice()
{

}
