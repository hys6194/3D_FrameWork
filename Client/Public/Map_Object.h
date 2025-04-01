#pragma once

#include "Client_Defines.h"
#include "GameObject.h"

BEGIN(Engine)
class CShader;
class CModel;
class CCollider;
END

BEGIN(Client)

class CMap_Object final : public CGameObject
{


public:
	typedef struct tagMapObjectDesc : public GAMEOBJECT_DESC
	{
		_wstring	strModelTag;
		_wstring	strObjectTag;
		_float3		fRotValue;
		_uint		iObjectIndex;
		_float4x4	matWorld;
		MODELTYPE	eType;

	}MAPOBJ_DESC;

private:
	CMap_Object(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual ~CMap_Object() = default;

public:
	virtual HRESULT						Initialize_Prototype()				override;
	virtual HRESULT						Initialize(void* pArg)				override;
	virtual void						Priority_Update(_float fTimeDelta)	override;
	virtual void						Update(_float fTimeDelta)			override;
	virtual void						Late_Update(_float fTimeDelta)		override;
	virtual HRESULT						Render()							override;


public:
	class CModel*						Get_ModelCom()
		{
			return m_pModelCom;
	}

	MAPOBJ_DESC&						Get_MapObjDesc()
	{
		return m_pDesc;
	}

public:
	void								Set_DescValue(_float3 fValue)
	{
		m_pDesc.fRotValue.x = fValue.x;
		m_pDesc.fRotValue.y = fValue.y;
		m_pDesc.fRotValue.z = fValue.z;
	}


private:
	HRESULT								Ready_Components(const wstring _strModelTag);
	HRESULT								Bind_SR();

private:
	class CShader*						m_pShaderCom = { nullptr };
	class CModel*						m_pModelCom = { nullptr };
	class CCollider*					m_pColliderCom = { nullptr };

	_float4x4							m_matProj = {};
	_float4x4							m_matView = {};
	_float4x4							m_matWorld = {};

	MAPOBJ_DESC							m_pDesc;

	D3D11_VIEWPORT						m_pViewPort;

	_bool								m_bRender = { true };

public:
	static CMap_Object*					Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CGameObject*						Clone(void* pArg);
	virtual void						Free() override;
};

END