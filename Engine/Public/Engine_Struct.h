#ifndef Engine_Struct_h__
#define Engine_Struct_h__

namespace Engine
{
	typedef struct tagEngineDesc
	{
		HWND			hWnd;				// 핸들
		bool			isWindowed;			// 최대화면인지 아닌지
		unsigned int	iWidth_VP;			// 가로 픽셀 수
		unsigned int	iHeight_VP;			// 세로 픽셀 수

	}ENGINE_DESC;

}


#endif // Engine_Struct_h__
