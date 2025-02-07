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

}


#endif // Engine_Struct_h__
