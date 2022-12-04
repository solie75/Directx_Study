#pragma once

class CEngine
	: public CSingleton<CEngine>
{
private:
	HWND m_hWnd;

public:
	void EngineInit(HWND _hWnd, UINT _iWidth, UINT _iHeight);
	void EngineProgress();

public:
	CEngine();
	~CEngine();
};

