#pragma once

#include "GraphicDev.h"
#include "TimerMgr.h"
#include "FrameMgr.h"

BEGIN(Engine)

// grphicDev;
inline HRESULT		Ready_GraphicDev(HWND hWnd,
	WINMODE eMode,
	const _uint& iSizeX,
	const _uint& iSizeY,
	GraphicDev** ppGraphicDev);

inline void		Render_Begin(D3DXCOLOR Color);
inline void		Render_End();

// TimerMgr
inline _float			Get_TimeDelta(const _tchar* pTimerTag);
inline void				Set_TimeDelta(const _tchar* pTimerTag);
inline HRESULT			Ready_Timer(const _tchar* pTimerTag);

// FrameMgr
inline HRESULT			Ready_Frame(const _tchar* pFrameTag, const _float& fCallLimit);
inline _bool			IsPermit_Call(const _tchar* pFrameTag, const _float& fTimeDelta);


inline void		Release_System();

#include "Export_System.inl"

END