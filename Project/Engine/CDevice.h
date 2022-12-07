#pragma once
class CDevice
	: public CSingleton<CDevice>
{
private:
	HWND m_hWnd;

	ComPtr<ID3D11Device> m_Device; 
	// ���̷�Ʈ ���� dll ���� �������� �Ǳ� ������ �츮�� Ŭ������ �ڼ��� �˼��� ����.
	// ���� Ŭ���� Ÿ���� �ɹ��� �δ� ���� �ƴ϶� �� �ּҸ� �ɹ� ������ �ּ� Ȱ���Ѵ�.
	// gpu �޸� �Ҵ� , �޸� ������ �����Ѵ�. 
	ComPtr <ID3D11DeviceContext> m_Context; // gpu �� ����� , gpu ����, ������, ���� ����

	ComPtr <IDXGISwapChain> m_SwapChain; // 

	ComPtr <ID3D11Texture2D> m_RTTex; // gpu �޸� ���(�ؽ��İ� gpu �� ����� ���� �� �ּҸ� ��Ī�ϸ� �� �ּ��� ���ؼ� �����ȴ�. ) renderTarget Texture
	ComPtr <ID3D11RenderTargetView> m_RTV;

	ComPtr <ID3D11Texture2D> m_DSTex; // DepthStencilTexture
	ComPtr <ID3D11DepthStencilView> m_DSV;

	D3D11_VIEWPORT m_ViewPort;

	// RenderTarget Resolution
	Vec2 m_vRenderResolution;

public:
	int DeviceInit(HWND _hWnd, UINT _RenderWidth, UINT _RenderHeight);
	void ClearTarget(float(&_color)[4]);
	void Present() { m_SwapChain->Present(0, 0); }

private:
	int CreateSwapChain();
	int CreateView();

public:
	CDevice();
	~CDevice();
};

