#pragma once

#include <wrl.h>
using namespace Microsoft::WRL;

#include <d3d11.h> // ���̷�Ʈx 11 ������� �����´�
#include <d3dcompiler.h> // shader ������
#include <DirectXMath.h> // Dx Math
#include <DirectXPackedVector.h> // ���̷�Ʈ x ���� ���� ���Ǵ� ����

using namespace DirectX;
using namespace DirectX::PackedVector;

#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "d3dcompiler")
// Math�� PackedVector �� ����ȿ� ����ü�� �ζ������� �̹� ����Ǿ� �־� ���� �������� �ʿ� ����

#include <SimpleMath.h>
using namespace DirectX::SimpleMath;

typedef Vector2 Vec2;
typedef Vector3 Vec3;
typedef Vector4 Vec4;

#include "define.h"
#include "struct.h"
#include "func.h"
#include "CSingleton.h"