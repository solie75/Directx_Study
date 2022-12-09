#include "pch.h"
#include "CEngine.h"
#include "CPathMgr.h"
#include "CTimeMgr.h"
#include "CKeyMgr.h"

#include "CDevice.h"
#include "Test.h"

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
	m_vResolution = Vec2((float)_iWidth, (float)_iHeight);

	// �ػ󵵿� �´� �۾����� ũ�� ����
	RECT rt = { 0, 0, (LONG)_iWidth, (LONG)_iHeight};

	AdjustWindowRect(&rt, WS_OVERLAPPEDWINDOW, false);
	SetWindowPos(m_hWnd, nullptr, 10, 10, rt.right - rt.left, rt.bottom - rt.top, 0);
	ShowWindow(m_hWnd, true);

	// Device �ʱ�ȭ
	if (FAILED(CDevice::GetInst()->DeviceInit(m_hWnd, _iWidth, _iHeight)))
	{
		MessageBox(nullptr, L"Device �ʱ�ȭ ����", L"����", MB_OK);
		return E_FAIL;
	}

	// Manager �ʱ�ȭ
	CPathMgr::GetInst()->PathMgrInit();
	CTimeMgr::GetInst()->TimeMgrInit();
	CKeyMgr::GetInst()->KeyMgrInit();

	TestInit();

	return S_OK;
}

void CEngine::EngineProgress()
{
	CEngineTick();
	CEngineRender();
}

void CEngine::CEngineTick()
{
	TestTick();
}

void CEngine::CEngineRender()
{
	// ������ ����
	float arrColor[4] = { 0.4f, 0.4f, 0.4f, 1.f };
	CDevice::GetInst()->ClearTarget(arrColor);

	TestRender();

	// ���� ����
	CDevice::GetInst()->Present();
}

