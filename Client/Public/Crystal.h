#pragma once

#include "Client_Defines.h"
#include "GameObject.h"

BEGIN(Engine)
class CShader;
class CModel;
class CCollider;
END


BEGIN(Client)

class CCrystal : public CGameObject
{
public:
	typedef struct tagCrystalDesc :public GAMEOBJECT_DESC
	{
		_float fLifeTime = 5.f;
		_vector vCrystalPos;
		_vector vLook;

		_wstring strModelTag;

		class CMoloch* pOwner;
	}CRYSTAL_DESC;

private:
	CCrystal(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CCrystal(const CCrystal& Prototype);
	virtual ~CCrystal() = default;

public:
	virtual HRESULT				Initialize_Prototype();
	virtual HRESULT				Initialize(void* pArg);
	virtual void				Priority_Update(_float fTimeDelta);
	virtual void				Update(_float fTimeDelta);
	virtual void				Late_Update(_float fTimeDelta);
	virtual HRESULT				Render();

public:
	HRESULT						Ready_Component();
	HRESULT						Bind_SR();

private:
	CShader*					m_pShaderCom = { nullptr };
	CModel*						m_pModelCom = { nullptr };
	CCollider*					m_pColliderCom = { nullptr };

	_bool						m_bDisappear = { false };

	_float						m_fLifeTime = {};

	_vector						m_vCrystalPos;
	_vector						m_vLook;
	_float						m_fTotalTime = { 0.f };

	_uint						m_iIndex	= { 0 };

	_wstring					m_strModelTag;

	class CMoloch*				m_pOwner = { nullptr };

public:
	static CCrystal*			Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject*		Clone(void* pArg)override;
	virtual void				Free() override;
};

END