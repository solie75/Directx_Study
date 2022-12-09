#include "pch.h"
#include "CDevice.h"

#include "CEngine.h"

CDevice::CDevice()
	//: m_hWnd(nullptr)
	//, m_Device(nullptr)
	//, m_Context(nullptr)
	//, m_SwapChain(nullptr)
	//, m_RTTex(nullptr)
	//, m_RTV(nullptr)
	//, m_DSTex(nullptr)
	//, m_DSV(nullptr)
	//, m_ViewPort{}
{
}

CDevice::~CDevice()
{
	//if (nullptr != m_Device) { m_Device->Release(); }
	//if (nullptr != m_Context) { m_Context->Release(); }
	//if (nullptr != m_SwapChain) { m_SwapChain->Release(); }
	//if (nullptr != m_RTTex) { m_RTTex->Release(); }
	//if (nullptr != m_RTV) { m_RTV->Release(); }
	//if (nullptr != m_DSTex) { m_DSTex->Release(); }
	//if (nullptr != m_DSV) { m_DSV->Release(); }
}


int CDevice::DeviceInit(HWND _hWnd, UINT _RenderWidth, UINT _RenderHeight)
{
	m_hWnd = _hWnd;
	m_vRenderResolution = Vec2((float)_RenderWidth, (float)_RenderHeight);

	int iFlag = 0;

#ifdef _DEBUG
	iFlag = D3D11_CREATE_DEVICE_DEBUG;
	// 다이렉트 dll 코드를 사용자가 열어 볼 수 없기 때문에 그 안에서 일어나는 오류들을 직접 돌 수 는 없고
	// 다이렉트 dll 안에서 설정되어 있는 에러 검출 코드들로 하여굼 알수 있다.
#endif
	// 이 오류 검출은 결국 debug 에서만 이루어지기 때문에 전처리기 에서 분기 처리한

	D3D_FEATURE_LEVEL eLevel = D3D_FEATURE_LEVEL::D3D_FEATURE_LEVEL_11_0;

	if (FAILED(D3D11CreateDevice(nullptr, D3D_DRIVER_TYPE_HARDWARE, nullptr, iFlag, nullptr, 0, D3D11_SDK_VERSION
		, m_Device.GetAddressOf(), &eLevel, m_Context.GetAddressOf())))
	{
		MessageBox(nullptr, L"Device, Context 생성 실패", L"Device 초기화 에러", MB_OK);
		return E_FAIL;
	}
	// D3D_DRIVER_TYPE 에 대해서 gpu 에ㅣ 대해서는  D3D_DRIVE_TYPE_HARDWARE gpu 가 없어 cpu에서 다이렉트 연산을 진행할 때에는 D3D_DRIVE_TYPE_SOFTWARE

	if (FAILED(CreateSwapChain()))
	{
		MessageBox(nullptr, L"SwapChain 생성 실패", L"Device 초기화 에러", MB_OK);
		return E_FAIL;
	}

	if (FAILED(CreateView()))
	{
		MessageBox(nullptr, L"View 생성 실패", L"Device 초기화 에러", MB_OK);
		return E_FAIL;
	}

	// 출력 타겟 설정
	m_Context->OMSetRenderTargets(1, m_RTV.GetAddressOf(), m_DSV.Get());

	// viewPort 설정
	m_ViewPort.TopLeftX = 0.f;
	m_ViewPort.TopLeftY = 0.f;

	Vec2 vWindowResol = CEngine::GetInst()->GetWindowResolution();
	m_ViewPort.Width = vWindowResol.x;
	m_ViewPort.Height = vWindowResol.y;

	m_ViewPort.MinDepth = 0.f;
	m_ViewPort.MaxDepth = 1.f; // 깊이 값의 최대

	m_Context->RSSetViewports(1, &m_ViewPort);
	
	return S_OK; // E_FAIL;
}


int CDevice::CreateSwapChain()
{
	DXGI_SWAP_CHAIN_DESC tDesc = {};

	tDesc.OutputWindow = m_hWnd;    // 출력 윈도우
	tDesc.Windowed = true;          // 창모드, 전체화면 모드

	tDesc.BufferCount = 1;
	tDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	tDesc.BufferDesc.Width = (UINT)m_vRenderResolution.x;
	tDesc.BufferDesc.Height = (UINT)m_vRenderResolution.y;
	tDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	tDesc.BufferDesc.RefreshRate.Denominator = 1;
	tDesc.BufferDesc.RefreshRate.Numerator = 60;
	tDesc.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;
	tDesc.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER::DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;

	tDesc.SwapEffect = DXGI_SWAP_EFFECT::DXGI_SWAP_EFFECT_DISCARD;

	tDesc.SampleDesc.Count = 1;
	tDesc.SampleDesc.Quality = 0;
	tDesc.Flags = 0;

	// 스왑체인 생성
	ComPtr<IDXGIDevice>  pDXGIDevice;
	ComPtr<IDXGIAdapter> pAdapter;
	ComPtr<IDXGIFactory> pFactory;


	m_Device->QueryInterface(__uuidof(IDXGIDevice), (void**)pDXGIDevice.GetAddressOf());
	pDXGIDevice->GetParent(__uuidof(IDXGIAdapter), (void**)pAdapter.GetAddressOf());
	pAdapter->GetParent(__uuidof(IDXGIFactory), (void**)pFactory.GetAddressOf());

	pFactory->CreateSwapChain(m_Device.Get(), &tDesc, &m_SwapChain);

	//pDXGIDevice->Release();
	//pAdapter->Release();
	//pFactory->Release();

	return S_OK;
}

int CDevice::CreateView()
{
	m_SwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (void**)m_RTTex.GetAddressOf());

	if (FAILED(m_Device->CreateRenderTargetView(m_RTTex.Get(), nullptr, m_RTV.GetAddressOf())))
	{
		return E_FAIL;
	}

	// DepthStencil 용도 텍스쳐 생성
	D3D11_TEXTURE2D_DESC tDesc = {};
	
	tDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	tDesc.Width = (UINT)m_vRenderResolution.x; // 반드시 렌더타겟과 같은 해상도록 설정해야 한다.
	tDesc.Height = (UINT)m_vRenderResolution.y;
	tDesc.ArraySize = 1;
	tDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
	tDesc.Usage = D3D11_USAGE_DEFAULT;
	tDesc.CPUAccessFlags = 0;

	tDesc.MipLevels = 1; // 원본만 생성
	tDesc.SampleDesc.Count = 1;
	tDesc.SampleDesc.Quality = 0;

	if (FAILED(m_Device->CreateTexture2D(&tDesc, nullptr, m_DSTex.GetAddressOf())))
	{
		return E_FAIL;
	}

	// DepthStencilView 생성
	if (FAILED(m_Device->CreateDepthStencilView(m_DSTex.Get(), nullptr, m_DSV.GetAddressOf())))
	{
		return E_FAIL;
	}

	return S_OK;
}

void CDevice::ClearTarget(float(&_color)[4])
{
	m_Context->ClearRenderTargetView(m_RTV.Get(), _color);
	m_Context->ClearDepthStencilView(m_DSV.Get(), D3D10_CLEAR_DEPTH | D3D10_CLEAR_STENCIL, 1.f, 0);
}