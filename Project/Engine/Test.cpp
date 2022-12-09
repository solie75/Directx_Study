#include "pch.h"
#include "Test.h"
#include "CPathMgr.h"

#include "CDevice.h" // gpu 에 명령을 할때  m_Device 가 필요하기 때문에

// 정점 버퍼
ComPtr<ID3D11Buffer> g_VB; // 정점정보를 저장하는 버퍼

// 쉐이더 (gpu 를 통해 연산 처리하는 것들)
// 정점 데이터를 받아서 rendertarget 에 색칠하는 과정을 그래픽스 쉐이더 혹은 그래픽스 파이프라인 과정이라고 하자
ComPtr<ID3DBlob> g_VSBlob; // 컴파일 완료된 vertex shader binary 코드를 저장.
ComPtr<ID3DBlob> g_PSBlob; // 컴파일 완료된 pixel shader binary 코드를 저장.
ComPtr<ID3DBlob> g_ErrBlob; // 컴파일이 실패했을 때;

ComPtr<ID3D11VertexShader> g_VS; // Vertex shader 객체 g_VSBlob 으로 만든다.
ComPtr<ID3D11PixelShader> g_PS; // Pixel shader 객체 g_PSBlob 으로 만든다.
// 위의 두 객체를 context 를 이용하여 전달

// Input Layout
ComPtr<ID3D11InputLayout> g_Layout;

void TestInit()
{
	Vtx arrVts[3] = {};

	arrVts[0].vPos = Vec3(0.f, 1.f, 0.5f);
	arrVts[0].vColor = Vec4(1.f, 1.f, 1.f, 1.f);
	arrVts[1].vPos = Vec3(1.f, -1.f, 0.5f);
	arrVts[1].vColor = Vec4(1.f, 1.f, 1.f, 1.f);
	arrVts[2].vPos = Vec3(-1.f, -1.f, 0.5f);
	arrVts[2].vColor = Vec4(1.f, 1.f, 1.f, 1.f);


	D3D11_BUFFER_DESC tBufferDesc = {};

	// 정저 저장 용도
	tBufferDesc.BindFlags = D3D11_BIND_FLAG::D3D11_BIND_VERTEX_BUFFER; // 버퍼의 용도. / 버퍼는 텍스쳐와 달리 정해져 있는 형체는 없는 메모리 정어리에 불과하나 그 용도는 명확히 해줘야 한다.

	// systemMemory 에서 수정 가능한 버퍼
	tBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_FLAG::D3D11_CPU_ACCESS_WRITE; 
	// 텍스쳐의 경우 gpu 상에서 색상을 색칠하고 거기에 그림을 그리기 때문에 cpu 에서 개입해서 값을  조작할 일이 없었다.
	// 지금의 정점 버퍼의 경우 gpu 상에 정점 버퍼 메모리를 할당하기는 하지만 그 정점의 위치를 옮기고 싶으면 버퍼의 내용을 사용자가 수정해야 한다.
	tBufferDesc.Usage = D3D11_USAGE_DYNAMIC; //cpu에 어떻게 접근할 것인지에 대한 방법
	// 이렇게 CPUAccessFlags 와 Usage를 조합하면 사용자가 cpu 에서 접근하여 버퍼를 조작할 수 있게 된다.

	// 버퍼의 크기
	tBufferDesc.ByteWidth = sizeof(Vtx) * 3; // 정점 3개 저장(삼각형)

	// 버퍼 생성
	D3D11_SUBRESOURCE_DATA tSub = {};
	tSub.pSysMem = arrVts;

	// tSub 쪽에 채워진 주소에서  tBufferDesc.ByteWidth 만큼의 메모리 크기로 버퍼가 만들어지면 g_VB.GetAddressOf() gpu 메모리에 값이 세팅된다.
	if (FAILED(DEVICE->CreateBuffer(&tBufferDesc, &tSub, g_VB.GetAddressOf())))
	{
		assert(nullptr);
	}
	
	// shader 파일 경로
	wstring strShaderFile = CPathMgr::GetInst()->GetContentPath();
	strShaderFile += L"shader\\test.fx";
	
	// VertexShader Compile
	if (FAILED(D3DCompileFromFile(strShaderFile.c_str(), nullptr, D3D_COMPILE_STANDARD_FILE_INCLUDE
		, "VS_Test", "vs_5_0", 0, 0, g_VSBlob.GetAddressOf(), g_ErrBlob.GetAddressOf())))
	{
		MessageBoxA(nullptr, (const char*)g_ErrBlob->GetBufferPointer()
			, "Vertex Shader Compile Failed!!", MB_OK);
	}

	// PixelShader Compile
	if (FAILED(D3DCompileFromFile(strShaderFile.c_str(), nullptr, D3D_COMPILE_STANDARD_FILE_INCLUDE
		, "PS_Test", "ps_5_0", 0, 0, g_PSBlob.GetAddressOf(), g_ErrBlob.GetAddressOf())))
	{
		MessageBoxA(nullptr, (const char*)g_ErrBlob->GetBufferPointer()
			, "Pixel Shader Compile Failed!!", MB_OK);
	}

	// 컴파일된 객체로 VertexShader, PixelShader 를 만든다.
	DEVICE->CreateVertexShader(g_VSBlob->GetBufferPointer(), g_VSBlob->GetBufferSize(), nullptr, g_VS.GetAddressOf());
	DEVICE->CreatePixelShader(g_PSBlob->GetBufferPointer(), g_PSBlob->GetBufferSize(), nullptr, g_PS.GetAddressOf());
	// 각  shader의 시작 주소와 크기를 가지고 객체를 생성한다.

	// Input Layout
	D3D11_INPUT_ELEMENT_DESC LayoutDesc[2] = {}; //(해당 자료형으로 딱 하나의 sementic 을 설명한다.)
	//현재 vPos , vColor 두 가지의 의미가 필요함으로  D3D11_INPUT_ELEMENT_DESC가 두개 필요하다.
	LayoutDesc[0].SemanticName = "POSITION";
	LayoutDesc[0].SemanticIndex = 0;
	LayoutDesc[0].AlignedByteOffset = 0; //POSITION 의 크기는 알았으나 하나의 정점 데이터에서 크기만큼의 데이터가 어디에 있는지는 모른다. 따라서 정점 시작 주소로부터 얼마만큼의 offset 이후에 POSITION 이 존재하는지 알아야 한다.
	// 현재 설정되어 있기로 POSITION 이 정점 구조체의 첫번째 맴버 변수 임으로 그 시작은 정점 데이터의 시작 주소와 같다. 고로 0을 대입
	LayoutDesc[0].Format = DXGI_FORMAT_R32G32B32_FLOAT;//sementic의 크기 , 픽셀 포멧 대입괼 vPos : POSITION 이 Vec3 크기이기 때문에 이에 맞는 것을 대입해준다.
	LayoutDesc[0].InputSlot = 0;
	LayoutDesc[0].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA; // 해당 데이터가 각 정점 한개당 어딘가에 들어있는 데이터임을 뜻한다.
	LayoutDesc[0].InstanceDataStepRate = 0;
	/*LayoutDesc[0].SemanticName = "POSITION"; // 같은  SemenricName 을 여러번 정의 할 수 있다. 이때는 인덱스로 분류하며 사용될때에는 뒤에 그 인덱스를 붙인다. ex(POSITION0, POSITION1 등)
	LayoutDesc[0].SemanticIndex = 1;*/


	LayoutDesc[1].SemanticName = "COLOR";
	LayoutDesc[1].SemanticIndex = 0;
	LayoutDesc[1].AlignedByteOffset = 12; // 정점 구조체의 두번째에 위치한 COLOR 는 정점 데이터의 시작 주소로 부터 정점 구조체의 첫번째인 POSITION 의 크기만큼의 OFFset을 갖는다.
	LayoutDesc[1].Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
	LayoutDesc[1].InputSlot = 0;
	LayoutDesc[1].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
	LayoutDesc[0].InstanceDataStepRate = 0;


	if (FAILED(DEVICE->CreateInputLayout(LayoutDesc, 2, g_VSBlob->GetBufferPointer(), g_VSBlob->GetBufferSize(), g_Layout.GetAddressOf())))
	{
		assert(nullptr);
	}

}

void TestTick()
{
}

void TestRender()
{
	// IA
	UINT iStride = sizeof(Vtx); //  버퍼에서는 연달아 있는 메모리상에서 버퍼별로 끊어 주어야 하는 데 그 때 알아야할 각 버퍼의 크기;
	UINT iOffset = 0; // 하나의 정점 버퍼 안에서 랜더링 을 시작하고 싶은 정점의 주소를 말한다. 현재는 전부 다 랜더링 할 것임으로 0;
	CONTEXT->IASetVertexBuffers(0, 1, g_VB.GetAddressOf(), &iStride, &iOffset);
	// 헨더링 파이프 라인의 과정이 시작되면 현재 지금 지정한 버퍼(g_VB)가 IA 단계가 시작 될때 전달된다.
	// 그렇다고 IASetVertexBuffer호출이 랜더링 파이프 라인의 과정중 IA 에 속하는 것은 아니다. 단지 IA 단계에 사용될 VertexBuffer 가 G_VB 라는 것을 알리는 것.
	CONTEXT->IASetInputLayout(g_Layout.Get());
	CONTEXT->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY::D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	// 정덤들을 기준으로 몇개씩 묶을지 정한다. 이때 TRIANGLELIST 이므로 세개의 정점을 묶어서 삼각형을 만들겠다는 의미.

	CONTEXT->VSSetShader(g_VS.Get(), nullptr, 0);
	CONTEXT->PSSetShader(g_PS.Get(), nullptr, 0);

	CONTEXT->Draw(3, 0); // 실제로 랜더링 파이프 라인을 시작시키는 함수
	// 첫번째 인자로 
}

void TestRelease()
{
}
