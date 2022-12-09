#ifndef _TEST
#define _TEST

// vs 입력 구조체
struct VS_IN
{
	float3 vPos : POSITION; // semantic , 레이아웃에서 정보를 가져온다.
	float4 vColor : COLOR;
};
// 셰이더에서 요구하는 정확한 정보가 정점 안에 어디에 있는지를 알려주는 것이 레이아웃 구조체이다.



struct VS_OUT
{
	float4 vPosition : SV_Position;
};

//vertax shader
VS_OUT VS_Test(VS_IN _in)
{
	VS_OUT output = (VS_OUT)0.f; // 초기화 값
	
    output.vPosition = float4(_in.vPos, 1.f);
	 
	return output;
}

//pixel shader
// Vertax shader의 반환값이  PS_TEST 의 입력값이 된다.
float4 PS_Test(VS_OUT _in) : SV_Target // 반환 타입인 float4 를 수식하는 말 SV_Target
// 여기에서 SV_Target는 현재의 랜더타겟을 의미. PS_Test 에서 반환하는 값이 SV_TARGET(RenderTarget)에 찍힐 색상임을 나타낸다.
{
	float4 vColor = (float4)0.f;

	vColor = float4(1.f, 0.f, 0.f, 1.f);

	return vColor;
}

// 위와 같은 효과 를 구조체로 표현하면
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