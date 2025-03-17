#pragma once

#include "Tool_Defines.h"
#include "Base.h"

BEGIN(Engine)
class CGameInstance;
END

BEGIN(Tool)

class CImGui_Base : public CBase
{
protected:
	CImGui_Base(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual ~CImGui_Base() = default; 

public:
	virtual HRESULT				Initialize()						= 0;
	virtual void				Update(_float fTiemDelta)			= 0;
	virtual HRESULT				Late_Update(_float fTiemDelta)		= 0;

public:
	HRESULT						Read_Objects(const _tchar* pFileName, MODELTYPE eType);
	HRESULT						Save_Objects();
	HRESULT						Load_Objects();
	HRESULT						Mouse_Picking();

protected:
	ID3D11Device*				m_pDevice					= { nullptr };
	ID3D11DeviceContext*		m_pContext					= { nullptr };
	CGameInstance*				m_pGameInstance = { nullptr };

public:
	virtual void				Free() override;
};

END