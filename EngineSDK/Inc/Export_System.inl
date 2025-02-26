HRESULT		Ready_GraphicDev(HWND hWnd,
	WINMODE eMode,
	const _uint& iSizeX,
	const _uint& iSizeY,
	GraphicDev** ppGraphicDev)
{
	return GraphicDev::GetInstance()->Ready_GraphicDev(hWnd, eMode, iSizeX, iSizeY, ppGraphicDev);
}

void		Render_Begin(D3DXCOLOR Color)
{
	GraphicDev::GetInstance()->Render_Begin(Color);
}

void		Render_End()
{
	GraphicDev::GetInstance()->Render_End();
}

_float			Get_TimeDelta(const _tchar* pTimerTag)
{
	return TimerMgr::GetInstance()->Get_TimeDelta(pTimerTag);
}
void				Set_TimeDelta(const _tchar* pTimerTag)
{
	TimerMgr::GetInstance()->Set_TimeDelta(pTimerTag);
}
HRESULT			Ready_Timer(const _tchar* pTimerTag)
{
	return TimerMgr::GetInstance()->Ready_Timer(pTimerTag);
}

HRESULT			Ready_Frame(const _tchar* pFrameTag, const _float& fCallLimit)
{
	return FrameMgr::GetInstance()->Ready_Frame(pFrameTag, fCallLimit);
}
_bool			IsPermit_Call(const _tchar* pFrameTag, const _float& fTimeDelta)
{
	return FrameMgr::GetInstance()->IsPermit_Call(pFrameTag, fTimeDelta);
}

 void		Release_System()
{
	FrameMgr::GetInstance()->DestroyInstance();
	TimerMgr::GetInstance()->DestroyInstance();
	GraphicDev::GetInstance()->DestroyInstance();
}
