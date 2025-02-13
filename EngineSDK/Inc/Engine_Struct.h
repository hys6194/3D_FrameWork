#ifndef Engine_Struct_h__
#define Engine_Struct_h__

namespace Engine
{
	typedef struct tagEngineDesc
	{
		HWND			hWnd;
		bool			isWindowed;
		unsigned int	iWidth_VP;
		unsigned int	iHeight_VP;
		unsigned int	iNumLevels;

	}ENGINE_DESC;

	typedef struct tagVertexPositionTexcoord
	{
		XMFLOAT3		vPosition;
		XMFLOAT2		vTexcoord;
	}VTXPOSTEX;

	typedef struct tagVertexNormalTexcoord
	{
		XMFLOAT3		vPosition;
		XMFLOAT3		vNormal;
		XMFLOAT2		vTexcoord;
	}VTXNORTEX;

	typedef struct tagLightDesc
	{
		enum TYPE { TYPE_DIRECTIONAL, TYPE_POINT, TYPE_END };

		TYPE			eType;				// ºûÀÇ 
		XMFLOAT4		vDirection;			// ºûÀÇ 
		XMFLOAT4		vPosition;			// ºûÀÇ 
		float			fRange;				// ºûÀÇ 
		XMFLOAT4		vDiffuse;			// ºûÀÇ 
		XMFLOAT4		vAmbient;			// ºûÀÇ 
		XMFLOAT4		vSpecular;			// ºûÀÇ 
	}LIGHT_DESC;

}


#endif // Engine_Struct_h__
