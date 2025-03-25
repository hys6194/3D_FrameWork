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

		unsigned int	iBoneIndex;
	}KEYFRAME;

	typedef struct ENGINE_DLL tagVertexPosition
	{
		XMFLOAT3		vPosition;

		const static unsigned int					iNumElements = 1;

		constexpr const static D3D11_INPUT_ELEMENT_DESC       ElementDesc[iNumElements] =
		{
			{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0}
		};
	}VTXPOS;

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
		XMFLOAT3		vTexcoord;					// 육면체에서의 Texcoord 정점은 3면이 접하고 있음

		const static unsigned int					iNumElements = 2;
		constexpr const static D3D11_INPUT_ELEMENT_DESC       ElementDesc[iNumElements] =
		{
			{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0},
			{ "TEXCOORD", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0},
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

		// 4-Bone
		XMUINT4			vBlendIndex;		// 뼈의 인덱스 개수
		XMFLOAT4		vBlendWeight;		// 뼈의 가중치, 0 ~ 1 사이의 값으로 사용

		// 8-Bone
		//XMUINT4			vBlendIndex0;		// 뼈의 인덱스 개수
		//XMFLOAT4		vBlendWeight0;
		//
		//XMUINT4			vBlendIndex1;		// 뼈의 인덱스 개수
		//XMFLOAT4		vBlendWeight1;

		// 4-Bone
		const static unsigned int					iNumElements = 6;

		// 8-Bone
		//const static unsigned int					iNumElements = 8;

		constexpr const static D3D11_INPUT_ELEMENT_DESC       ElementDesc[iNumElements] =
		{
			{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0},
			{ "NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0},
			{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 24, D3D11_INPUT_PER_VERTEX_DATA, 0},
			{ "TANGENT", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 32, D3D11_INPUT_PER_VERTEX_DATA, 0},
			{ "BLENDINDEX", 0, DXGI_FORMAT_R32G32B32A32_UINT, 0, 44, D3D11_INPUT_PER_VERTEX_DATA, 0},
			{ "BLENDWEIGHT", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 60, D3D11_INPUT_PER_VERTEX_DATA, 0},

			// 8-Bone
			//{ "BLENDINDEX1", 0, DXGI_FORMAT_R32G32B32A32_UINT, 0, 76, D3D11_INPUT_PER_VERTEX_DATA, 0},
			//{ "BLENDWEIGHT1", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 92, D3D11_INPUT_PER_VERTEX_DATA, 0},
		};
	}VTXANIMESH;

	typedef struct ENGINE_DLL tagVertexPosTexInstance
	{
		const	  static unsigned int						  iNumElements = 6;
		constexpr const static D3D11_INPUT_ELEMENT_DESC       ElementDesc[iNumElements] =
		{
			{ "POSITION", 0,	 DXGI_FORMAT_R32G32B32_FLOAT,	 0, 0,  D3D11_INPUT_PER_VERTEX_DATA,   0},
			{ "TEXCOORD", 0,	 DXGI_FORMAT_R32G32_FLOAT,		 0, 12, D3D11_INPUT_PER_VERTEX_DATA,   0},

			// "TEXCOORD", 1 :   셰이더의 시멘틱 넘버 구별을 위해서 수를 넣은 것
			//  1 :		         인스턴싱하면서 버퍼를 두 개 만들었는데 몇 번째 버퍼인지 알려주기 위해서 선언한 것
			{ "TEXCOORD", 1,	 DXGI_FORMAT_R32G32B32A32_FLOAT, 1, 0,  D3D11_INPUT_PER_INSTANCE_DATA, 1},
			{ "TEXCOORD", 2,	 DXGI_FORMAT_R32G32B32A32_FLOAT, 1, 16, D3D11_INPUT_PER_INSTANCE_DATA, 1},
			{ "TEXCOORD", 3,	 DXGI_FORMAT_R32G32B32A32_FLOAT, 1, 32, D3D11_INPUT_PER_INSTANCE_DATA, 1},
			{ "TEXCOORD", 4,	 DXGI_FORMAT_R32G32B32A32_FLOAT, 1, 48, D3D11_INPUT_PER_INSTANCE_DATA, 1},

		};
	}INST_VTXPOSTEX;
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

	typedef struct ENGINE_DLL tagInstancingVertex
	{
		XMFLOAT4	vRight;
		XMFLOAT4	vUp;
		XMFLOAT4	vLook;
		XMFLOAT4	vTranslation;
	}INSTVTX;
}


#endif // Engine_Struct_h__
