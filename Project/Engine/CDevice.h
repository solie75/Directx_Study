#pragma once
class CDevice
	: public CSingleton<CDevice>
{
private:
	HWND m_hWnd;

	ID3D11Device* m_Device; 
	// ���̷�Ʈ ���� dll ���� �������� �Ǳ� ������ �츮�� Ŭ������ �ڼ��� �˼��� ����.
	// ���� Ŭ���� Ÿ���� �ɹ��� �δ� ���� �ƴ϶� �� �ּҸ� �ɹ� ������ �ּ� Ȱ���Ѵ�.
	// gpu �޸� �Ҵ� , �޸� ������ �����Ѵ�. 
	ID3D11DeviceContext* m_Context; // gpu �� ����� , gpu ����, ������, ���� ����

	IDXGISwapChain* m_SwapChain;

	ID3D11Texture2D* m_RTTex;
	ID3D11RenderTargetView* m_RTV;

	ID3D11Texture2D* m_DXTex;
	ID3D11DepthStencilView* m_DSV;

	D3D11_VIEWPORT m_ViewPort;

public:
	int DeviceInit(HWND _hWnd);

public:
	CDevice();
	~CDevice();
};

