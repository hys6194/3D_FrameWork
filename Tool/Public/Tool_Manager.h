#pragma once

#include "Base.h"
#include "Tool_Defines.h"

BEGIN(Engine)
class CGameInstance;
END

BEGIN(Tool)

class Tool_Manager :public CBase
{

private:
	Tool_Manager(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, CGameInstance* pGameInstance);
	virtual ~Tool_Manager() = default;

public:
	HRESULT						Initialize();
	void						Update(_float fTimeDelta);
	void						Late_Update(_float fTimeDelta);
	HRESULT						Render();

private:
	ID3D11Device*				m_pDevice = { nullptr };
	ID3D11DeviceContext*		m_pContext = { nullptr };
	CGameInstance*				m_pGameInstance = { nullptr };

private:
	class CImGui_UI*			m_pUI = { nullptr };
	class CImGui_Map*			m_pMap = { nullptr };
	class CCell_Guide*			m_pCell = { nullptr };

private:
	_bool						m_bRayShoot;
	_bool						m_bTest = { false };
	_float						m_fDistance{};
	_float						m_fPreDistance{};
	_vector						m_vCellCoord{};


private:
	void						Picking_Objects();
	void						Create_NaviCells();

public:
	static Tool_Manager*		Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, CGameInstance* pGameInstance);
	virtual void				Free() override;
};

END