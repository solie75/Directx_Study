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
	// 메인 윈도우 해들
	m_hWnd = _hWnd;
	m_vResolution = Vec2((float)_iWidth, (float)_iHeight);

	// 해상도에 맞는 작업영역 크기 조정
	RECT rt = { 0, 0, (LONG)_iWidth, (LONG)_iHeight};

	AdjustWindowRect(&rt, WS_OVERLAPPEDWINDOW, false);
	SetWindowPos(m_hWnd, nullptr, 10, 10, rt.right - rt.left, rt.bottom - rt.top, 0);
	ShowWindow(m_hWnd, true);

	// Device 초기화
	if (FAILED(CDevice::GetInst()->DeviceInit(m_hWnd, _iWidth, _iHeight)))
	{
		MessageBox(nullptr, L"Device 초기화 실패", L"에러", MB_OK);
		return E_FAIL;
	}

}

void CEngine::EngineProgress()
{
	CEngineTick();
	CEngineRender();
}

void CEngine::CEngineTick()
{
}

void CEngine::CEngineRender()
{
	// 렌더링 시작
	float arrColor[4] = { 0.4f, 0.4f, 0.4f, 1.f };
	CDevice::GetInst()->ClearTarget(arrColor);

	// 렌더 종료
	CDevice::GetInst()->Present();
}

