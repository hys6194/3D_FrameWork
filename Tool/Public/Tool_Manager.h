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
	ID3D11Device*				m_pDevice			= { nullptr };
	ID3D11DeviceContext*		m_pContext			= { nullptr };

private:
	class CMap_Object*			m_pMapObject = { nullptr };
	class CImGui_UI*			m_pUI				= { nullptr };
	class CImGui_Map*			m_pMap				= { nullptr };
	class CCell_Guide*			m_pCell				= { nullptr };
	class CGameInstance*		m_pGameInstance		= { nullptr };

private:
	_bool						m_bRayShoot			= { false };
	_bool						m_bColl				= { false };

	_float						m_fDistance			  {};
	_float						m_fPreDistance		  {};
	_vector						m_vCellCoord		  {};

private:
	void						Picking_Objects();
	void						Create_NaviCells();
	void						Move_CamPos(class CMap_Object* pObject);

public:
	static Tool_Manager*		Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, class CGameInstance* pGameInstance);
	virtual void				Free() override;
};

END