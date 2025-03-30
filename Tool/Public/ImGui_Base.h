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
	wstring Get_ObjectTag()
	{
		return m_strObjectName;
	}

	wstring Get_ModelTag()
	{
		return m_strModelName;
	}

protected:
	ImVec2						Set_Button_Offset(const char* _cButtonName, _float fX, _float fY);
	void						Get_PrototypeList(const wstring _strDest, const wstring _strSour);

protected:
	void						Button_Info(_float4 _fValue);

protected:
	ID3D11Device*				m_pDevice					= { nullptr };
	ID3D11DeviceContext*		m_pContext					= { nullptr };
	CGameInstance*				m_pGameInstance				= { nullptr };

	vector<wstring>				m_vecProtoNames;
	vector<wstring>				m_vecProtoTag;

	wstring						m_strModelName;
	wstring						m_strObjectName;

	_bool						m_bEvent1					= { false };
	_bool						m_bEvent2					= { false };
	_bool						m_bEvent3					= { false };

	_float						m_fScale					{};

	_vector						m_vAixs						= AXIS_X;
	_float 						m_fValue;


public:
	virtual void				Free() override;
};

END