#include "pch.h"
#include "Test.h"
#include "CPathMgr.h"

#include "CDevice.h" // gpu �� ����� �Ҷ�  m_Device �� �ʿ��ϱ� ������

// ���� ����
ComPtr<ID3D11Buffer> g_VB; // ���������� �����ϴ� ����

// ���̴� (gpu �� ���� ���� ó���ϴ� �͵�)
// ���� �����͸� �޾Ƽ� rendertarget �� ��ĥ�ϴ� ������ �׷��Ƚ� ���̴� Ȥ�� �׷��Ƚ� ���������� �����̶�� ����
ComPtr<ID3DBlob> g_VSBlob; // ������ �Ϸ�� vertex shader binary �ڵ带 ����.
ComPtr<ID3DBlob> g_PSBlob; // ������ �Ϸ�� pixel shader binary �ڵ带 ����.
ComPtr<ID3DBlob> g_ErrBlob; // �������� �������� ��;

ComPtr<ID3D11VertexShader> g_VS; // Vertex shader ��ü g_VSBlob ���� �����.
ComPtr<ID3D11PixelShader> g_PS; // Pixel shader ��ü g_PSBlob ���� �����.
// ���� �� ��ü�� context �� �̿��Ͽ� ����

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

	// ���� ���� �뵵
	tBufferDesc.BindFlags = D3D11_BIND_FLAG::D3D11_BIND_VERTEX_BUFFER; // ������ �뵵. / ���۴� �ؽ��Ŀ� �޸� ������ �ִ� ��ü�� ���� �޸� ����� �Ұ��ϳ� �� �뵵�� ��Ȯ�� ����� �Ѵ�.

	// systemMemory ���� ���� ������ ����
	tBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_FLAG::D3D11_CPU_ACCESS_WRITE; 
	// �ؽ����� ��� gpu �󿡼� ������ ��ĥ�ϰ� �ű⿡ �׸��� �׸��� ������ cpu ���� �����ؼ� ����  ������ ���� ������.
	// ������ ���� ������ ��� gpu �� ���� ���� �޸𸮸� �Ҵ��ϱ�� ������ �� ������ ��ġ�� �ű�� ������ ������ ������ ����ڰ� �����ؾ� �Ѵ�.
	tBufferDesc.Usage = D3D11_USAGE_DYNAMIC; //cpu�� ��� ������ �������� ���� ���
	// �̷��� CPUAccessFlags �� Usage�� �����ϸ� ����ڰ� cpu ���� �����Ͽ� ���۸� ������ �� �ְ� �ȴ�.

	// ������ ũ��
	tBufferDesc.ByteWidth = sizeof(Vtx) * 3; // ���� 3�� ����(�ﰢ��)

	// ���� ����
	D3D11_SUBRESOURCE_DATA tSub = {};
	tSub.pSysMem = arrVts;

	// tSub �ʿ� ä���� �ּҿ���  tBufferDesc.ByteWidth ��ŭ�� �޸� ũ��� ���۰� ��������� g_VB.GetAddressOf() gpu �޸𸮿� ���� ���õȴ�.
	if (FAILED(DEVICE->CreateBuffer(&tBufferDesc, &tSub, g_VB.GetAddressOf())))
	{
		assert(nullptr);
	}
	
	// shader ���� ���
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

	// �����ϵ� ��ü�� VertexShader, PixelShader �� �����.
	DEVICE->CreateVertexShader(g_VSBlob->GetBufferPointer(), g_VSBlob->GetBufferSize(), nullptr, g_VS.GetAddressOf());
	DEVICE->CreatePixelShader(g_PSBlob->GetBufferPointer(), g_PSBlob->GetBufferSize(), nullptr, g_PS.GetAddressOf());
	// ��  shader�� ���� �ּҿ� ũ�⸦ ������ ��ü�� �����Ѵ�.

	// Input Layout
	D3D11_INPUT_ELEMENT_DESC LayoutDesc[2] = {}; //(�ش� �ڷ������� �� �ϳ��� sementic �� �����Ѵ�.)
	//���� vPos , vColor �� ������ �ǹ̰� �ʿ�������  D3D11_INPUT_ELEMENT_DESC�� �ΰ� �ʿ��ϴ�.
	LayoutDesc[0].SemanticName = "POSITION";
	LayoutDesc[0].SemanticIndex = 0;
	LayoutDesc[0].AlignedByteOffset = 0; //POSITION �� ũ��� �˾����� �ϳ��� ���� �����Ϳ��� ũ�⸸ŭ�� �����Ͱ� ��� �ִ����� �𸥴�. ���� ���� ���� �ּҷκ��� �󸶸�ŭ�� offset ���Ŀ� POSITION �� �����ϴ��� �˾ƾ� �Ѵ�.
	// ���� �����Ǿ� �ֱ�� POSITION �� ���� ����ü�� ù��° �ɹ� ���� ������ �� ������ ���� �������� ���� �ּҿ� ����. ��� 0�� ����
	LayoutDesc[0].Format = DXGI_FORMAT_R32G32B32_FLOAT;//sementic�� ũ�� , �ȼ� ���� ���Ա� vPos : POSITION �� Vec3 ũ���̱� ������ �̿� �´� ���� �������ش�.
	LayoutDesc[0].InputSlot = 0;
	LayoutDesc[0].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA; // �ش� �����Ͱ� �� ���� �Ѱ��� ��򰡿� ����ִ� ���������� ���Ѵ�.
	LayoutDesc[0].InstanceDataStepRate = 0;
	/*LayoutDesc[0].SemanticName = "POSITION"; // ����  SemenricName �� ������ ���� �� �� �ִ�. �̶��� �ε����� �з��ϸ� ���ɶ����� �ڿ� �� �ε����� ���δ�. ex(POSITION0, POSITION1 ��)
	LayoutDesc[0].SemanticIndex = 1;*/


	LayoutDesc[1].SemanticName = "COLOR";
	LayoutDesc[1].SemanticIndex = 0;
	LayoutDesc[1].AlignedByteOffset = 12; // ���� ����ü�� �ι�°�� ��ġ�� COLOR �� ���� �������� ���� �ּҷ� ���� ���� ����ü�� ù��°�� POSITION �� ũ�⸸ŭ�� OFFset�� ���´�.
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
	UINT iStride = sizeof(Vtx); //  ���ۿ����� ���޾� �ִ� �޸𸮻󿡼� ���ۺ��� ���� �־�� �ϴ� �� �� �� �˾ƾ��� �� ������ ũ��;
	UINT iOffset = 0; // �ϳ��� ���� ���� �ȿ��� ������ �� �����ϰ� ���� ������ �ּҸ� ���Ѵ�. ����� ���� �� ������ �� �������� 0;
	CONTEXT->IASetVertexBuffers(0, 1, g_VB.GetAddressOf(), &iStride, &iOffset);
	// ����� ������ ������ ������ ���۵Ǹ� ���� ���� ������ ����(g_VB)�� IA �ܰ谡 ���� �ɶ� ���޵ȴ�.
	// �׷��ٰ� IASetVertexBufferȣ���� ������ ������ ������ ������ IA �� ���ϴ� ���� �ƴϴ�. ���� IA �ܰ迡 ���� VertexBuffer �� G_VB ��� ���� �˸��� ��.
	CONTEXT->IASetInputLayout(g_Layout.Get());
	CONTEXT->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY::D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	// �������� �������� ��� ������ ���Ѵ�. �̶� TRIANGLELIST �̹Ƿ� ������ ������ ��� �ﰢ���� ����ڴٴ� �ǹ�.

	CONTEXT->VSSetShader(g_VS.Get(), nullptr, 0);
	CONTEXT->PSSetShader(g_PS.Get(), nullptr, 0);

	CONTEXT->Draw(3, 0); // ������ ������ ������ ������ ���۽�Ű�� �Լ�
	// ù��° ���ڷ� 
}

void TestRelease()
{
}
