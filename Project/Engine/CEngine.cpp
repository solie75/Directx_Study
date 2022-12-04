#include "pch.h"
#include "CEngine.h"

#include "CDevice.h"

CEngine::CEngine()
	: m_hWnd(nullptr)
{
}

CEngine::~CEngine()
{
}


int CEngine::EngineInit(HWND _hWnd, UINT _iWidth, UINT _iHeight)
{
	// ���� ������ �ص�
	m_hWnd = _hWnd;

	// �ػ󵵿� �´� �۾����� ũ�� ����
	RECT rt = { 0, 0, (LONG)_iWidth, (LONG)_iHeight};

	AdjustWindowRect(&rt, WS_OVERLAPPEDWINDOW, false);
	SetWindowPos(m_hWnd, nullptr, 10, 10, rt.right - rt.left, rt.bottom - rt.top, 0);
	ShowWindow(m_hWnd, true);

	// Device �ʱ�ȭ
	if (FAILED(CDevice::GetInst()->DeviceInit(m_hWnd)))
	{
		MessageBox(nullptr, L"Device �ʱ�ȭ ����", L"����", MB_OK);
		return E_FAIL;
	}

}

void CEngine::EngineProgress()
{
}

