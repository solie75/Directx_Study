#pragma once

#include <d3d11.h> // 다이렉트x 11 헤더들을 가져온다
#include <d3dcompiler.h> // shader 컴파일
#include <DirectXMath.h> // Dx Math
#include <DirectXPackedVector.h> // 다이렉트 x 에서 자주 사용되는 벡터

using namespace DirectX;
using namespace DirectX::PackedVector;

#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "d3dcompiler")
// Math와 PackedVector 는 헤더안에 구조체와 인라인으로 이미 선언되어 있어 따로 연결해줄 필요 없다


#include "CSingleton.h"