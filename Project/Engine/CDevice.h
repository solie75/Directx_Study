#pragma once
class CDevice
	: public CSingleton<CDevice>
{
private:
	HWND m_hWnd;

	ComPtr<ID3D11Device> m_Device; 
	// 다이렉트 관련 dll 에서 생성까지 되기 때문에 우리는 클래스를 자세히 알수가 업다.
	// 따라서 클래스 타입을 맴버로 두는 것이 아니라 그 주소를 맴버 변수로 둬서 활용한다.
	// gpu 메모리 할당 , 메모리 공간을 제어한다. 
	ComPtr <ID3D11DeviceContext> m_Context; // gpu 에 명령함 , gpu 제어, 랜더링, 동작 수행

	ComPtr <IDXGISwapChain> m_SwapChain; // 

	ComPtr <ID3D11Texture2D> m_RTTex; // gpu 메모리 사용(텍스쳐가 gpu 에 만들어 지고 그 주소를 지칭하며 이 주소을 통해서 관리된다. ) renderTarget Texture
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

