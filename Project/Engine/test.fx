#ifndef _TEST
#define _TEST

// vs �Է� ����ü
struct VS_IN
{
	float3 vPos : POSITION; // semantic , ���̾ƿ����� ������ �����´�.
	float4 vColor : COLOR;
};
// ���̴����� �䱸�ϴ� ��Ȯ�� ������ ���� �ȿ� ��� �ִ����� �˷��ִ� ���� ���̾ƿ� ����ü�̴�.



struct VS_OUT
{
	float4 vPosition : SV_Position;
};

//vertax shader
VS_OUT VS_Test(VS_IN _in)
{
	VS_OUT output = (VS_OUT)0.f; // �ʱ�ȭ ��
	
    output.vPosition = float4(_in.vPos, 1.f);
	 
	return output;
}

//pixel shader
// Vertax shader�� ��ȯ����  PS_TEST �� �Է°��� �ȴ�.
float4 PS_Test(VS_OUT _in) : SV_Target // ��ȯ Ÿ���� float4 �� �����ϴ� �� SV_Target
// ���⿡�� SV_Target�� ������ ����Ÿ���� �ǹ�. PS_Test ���� ��ȯ�ϴ� ���� SV_TARGET(RenderTarget)�� ���� �������� ��Ÿ����.
{
	float4 vColor = (float4)0.f;

	vColor = float4(1.f, 0.f, 0.f, 1.f);

	return vColor;
}

// ���� ���� ȿ�� �� ����ü�� ǥ���ϸ�
//struct PS_OUT
//{
//	float4 vColor : SV_Target;
//};
//
//PS_OUT PS_Test(VS_OUT _in)
//{
// PS_OUT output = (PS_OUT)0.f;
// output.vColor = float4(1.f, 0.f, 0.f, 1.f);
// return output;
//}

#endif