#pragma once
#include "Base.h"
#include "Define.h"
#include "Export_System.h"

class CImGuiManager : public Base
{
	DECLARE_SINGLETON(CImGuiManager)

private :
	explicit			CImGuiManager();
	virtual				~CImGuiManager();

public :
	void				SetUp_ImguiManager(LPDIRECT3DDEVICE9 );
	void				CleanUp_ImguiManager();

public :
	void				Render_Begin();
	void				Render_DemoWindow();
	void				Render_CameraEditor();

private :
	virtual void		Free();
};