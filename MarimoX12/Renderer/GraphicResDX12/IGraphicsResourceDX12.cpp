#include "IGraphicsResourceDX12.h"

void IGraphicsResourceDX12::destroyResource()
{
	this->m_pResource = nullptr;
	m_gpuVirtualAddr = NULL;
}