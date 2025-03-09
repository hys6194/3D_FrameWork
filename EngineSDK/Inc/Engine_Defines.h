#pragma once
//#define  NOMINMAX  

#pragma warning(disable : 4251)
#pragma warning(disable : 4267)
#include <d3d11.h>
#include <DirectXMath.h>
#include <DirectXCollision.h>
#include <d3dcompiler.h>

#include "DirectXTK/VertexTypes.h"
#include "DirectXTK/DDSTextureLoader.h"
#include "DirectXTK/WICTextureLoader.h"
#include "Fx11/d3dx11effect.h"
#include "assimp_Old/scene.h"
#include "assimp_Old/Importer.hpp"
#include "assimp_Old/postprocess.h"

using namespace DirectX;

#include <vector>
#include <list>
#include <map>
#include <algorithm>
#include <functional>
#include <string>
#include <unordered_map>
#include <ctime>
using namespace std;

#include "Engine_Enum.h"
#include "Engine_Macro.h"
#include "Engine_Struct.h"
#include "Engine_Typedef.h"
#include "Engine_Function.h"

#define DIRECTINPUT_VERSION	0x0800
#include <dinput.h>


namespace Engine
{
	// Transform을 어디에서든 끌고오기 위해서 전역으로 선언
	const wstring g_strTransformTag = TEXT("Com_Transform");
}

#define TIME60									TEXT("Timer_60")
#define AXIS_X									XMVectorSet(1.f, 0.f, 0.f, 0.f)
#define AXIS_Y									XMVectorSet(0.f, 1.f, 0.f, 0.f)
#define AXIS_Z									XMVectorSet(0.f, 0.f, 1.f, 0.f)
#define AXIS_W									XMVectorSet(0.f, 0.f, 0.f, 1.f)

#ifdef _DEBUG

#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>

#ifndef DBG_NEW 

#define DBG_NEW new ( _NORMAL_BLOCK , __FILE__ , __LINE__ ) 
#define new DBG_NEW 

#endif
#endif


using namespace Engine;