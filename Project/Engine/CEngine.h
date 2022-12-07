#pragma once

class CEngine
	: public CSingleton<CEngine>
{
private:
	HWND m_hWnd;

	// ������ �ػ�
	Vec2 m_vResolution;

public:
	Vec2 GetWindowResolution() { return m_vResolution; }

public:
	int EngineInit(HWND _hWnd, UINT _iWidth, UINT _iHeight);
	void EngineProgress();

private:
	void CEngineTick();
	void CEngineRender();

public:
	CEngine();
	~CEngine();
};

