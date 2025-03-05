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

	typedef struct tagKeyFrame
	{
		XMFLOAT3		vScale;
		XMFLOAT4		vRotation;
		XMFLOAT3		vTranslation;
		float			fTrackPosition;
	}KEYFRAME;

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
			{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0},
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
			{ "NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0},
			{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 24, D3D11_INPUT_PER_VERTEX_DATA, 0},
		};
	}VTXNORTEX;

	typedef struct ENGINE_DLL tagVertexCube
	{
		XMFLOAT3		vPosition;
		XMFLOAT3		vNormal;
		XMFLOAT3		vTexcoord;					// 육면체에서의 Texcoord 정점은 3면이 접하고 있음

		const static unsigned int					iNumElements = 3;
		constexpr const static D3D11_INPUT_ELEMENT_DESC       ElementDesc[iNumElements] =
		{
			{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0},
			{ "NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0},
			{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 24, D3D11_INPUT_PER_VERTEX_DATA, 0},
		};
	}VTXCUBE;

	typedef struct ENGINE_DLL tagVerticesMesh
	{
		XMFLOAT3		vPosition;			// 위치 벡터
		XMFLOAT3		vNormal;			// 법선 벡터
		XMFLOAT2		vTexcoord;			// UV 벡터
		XMFLOAT3		vTangent;			// 접선 벡터

		const static unsigned int					iNumElements = 4;
		constexpr const static D3D11_INPUT_ELEMENT_DESC       ElementDesc[iNumElements] =
		{
			{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0},
			{ "NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0},
			{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 24, D3D11_INPUT_PER_VERTEX_DATA, 0},
			{ "TANGENT", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 32, D3D11_INPUT_PER_VERTEX_DATA, 0},
		};
	}VTXMESH;

	typedef struct ENGINE_DLL tagVerticesAnimMesh
	{
		XMFLOAT3		vPosition;			// 위치 벡터
		XMFLOAT3		vNormal;			// 법선 벡터
		XMFLOAT2		vTexcoord;			// UV 벡터
		XMFLOAT3		vTangent;			// 접선 벡터

		XMUINT4			vBlendIndex;		// 뼈의 인덱스 개수
		XMFLOAT4		vBlendWeight;		// 뼈의 가중치, 0 ~ 1 사이의 값으로 사용


		const static unsigned int					iNumElements = 6;
		constexpr const static D3D11_INPUT_ELEMENT_DESC       ElementDesc[iNumElements] =
		{
			{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0},
			{ "NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0},
			{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 24, D3D11_INPUT_PER_VERTEX_DATA, 0},
			{ "TANGENT", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 32, D3D11_INPUT_PER_VERTEX_DATA, 0},
			{ "BLENDINDEX", 0, DXGI_FORMAT_R32G32B32A32_UINT, 0, 44, D3D11_INPUT_PER_VERTEX_DATA, 0},
			{ "BLENDWEIGHT", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 60, D3D11_INPUT_PER_VERTEX_DATA, 0},
		};
	}VTXANIMESH;

	typedef struct tagLightDesc
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
