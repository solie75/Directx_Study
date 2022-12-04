#include "pch.h"
#include "CDevice.h"

CDevice::CDevice()
	: m_hWnd(nullptr)
	, m_Device(nullptr)
	, m_Context(nullptr)
	, m_SwapChain(nullptr)
	, m_RTTex(nullptr)
	, m_RTV(nullptr)
	, m_DXTex(nullptr)
	, m_DSV(nullptr)
	, m_ViewPort{}
{
}

CDevice::~CDevice()
{
}


int CDevice::DeviceInit(HWND _hWnd)
{
	m_hWnd = _hWnd;




	return S_OK; // E_FAIL;
}

