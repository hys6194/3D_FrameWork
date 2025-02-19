#ifndef Engine_Struct_h__
#define Engine_Struct_h__

namespace Engine
{
	typedef struct tagEngineDesc
	{
		HINSTANCE		hInstance;
		HWND			hWnd;
		bool			isWindowed;
		unsigned int	iWidth_VP;
		unsigned int	iHeight_VP;
		unsigned int	iNumLevels;

	}ENGINE_DESC;

	typedef struct ENGINE_DLL tagVertexPositionTexcoord
	{
		XMFLOAT3		vPosition;
		XMFLOAT2		vTexcoord;

		const static unsigned int					iNumElements = 2;

		/* Prototype_Component_Shader_VtxPosTex */
		// D3D11_INPUT_ELEMENT_DESC : 내 정점을 구성하는 멤버 변수 하나의 정보를 표현하기위한 구조체
		constexpr const static D3D11_INPUT_ELEMENT_DESC       ElementDesc[iNumElements] =
		{
			{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0},
			{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0},
		};
	}VTXPOSTEX;

	typedef struct ENGINE_DLL tagVertexNormalTexcoord
	{
		XMFLOAT3		vPosition;
		XMFLOAT3		vNormal;
		XMFLOAT2		vTexcoord;

		const static unsigned int					iNumElements = 3;
		constexpr const static D3D11_INPUT_ELEMENT_DESC       ElementDesc[iNumElements] =
		{
			{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0},
			{ "NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0},
			{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0},
		};
	}VTXNORTEX;

	typedef struct ENGINE_DLL tagVerticesMesh
	{
		XMFLOAT3		vPosition;
		XMFLOAT3		vNormal;
		XMFLOAT2		vTexcoord;
		XMFLOAT3		vTangent;

		const static unsigned int					iNumElements = 4;
		constexpr const static D3D11_INPUT_ELEMENT_DESC       ElementDesc[iNumElements] =
		{
			{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0},
			{ "NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0},
			{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0},
			{ "TANGENT", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0},
		};
	}VTXMESH;

	typedef struct ENGINE_DLL tagLightDesc
	{
		enum TYPE { TYPE_DIRECTIONAL, TYPE_POINT, TYPE_END };

		TYPE			eType;				// 광원 타입
		XMFLOAT4		vDirection;			// 광원의 룩 방향
		XMFLOAT4		vPosition;			// 광원 위치
		float			fRange;				// 광원 길이
		XMFLOAT4		vDiffuse;			// 난반사
		XMFLOAT4		vAmbient;			// 엠비언트 (반사광)
		XMFLOAT4		vSpecular;			// 정반사
	}LIGHT_DESC;
}


#endif // Engine_Struct_h__
