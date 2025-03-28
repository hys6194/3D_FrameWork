#pragma once

#include "Tool_Defines.h"
#include "GameObject.h"

BEGIN(Engine)
class CShader;
class CModel;
class CCollider;
END

BEGIN(Tool)

class CMap_Object final : public CGameObject
{


public:
	typedef struct tagMapObjectDesc : public GAMEOBJECT_DESC
	{

		_wstring  strModelTag;
		_wstring  strObjectTag;
		_float4x4 matWorld;		// 월드 행렬을 가져오는 것이 좋지 않을까? 그래야 값저장때 편할듯

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
	CModel* Get_ModelCom()
	{
		return m_pModelCom;
	}

	MAPOBJ_DESC* Get_MapObjDesc()
	{
		return m_pDesc;
	}


private:
	HRESULT								Ready_Components(const wstring _strModelTag);
	HRESULT								Bind_SR();

private:
	CShader*							m_pShaderCom	= { nullptr };
	CModel*								m_pModelCom		= { nullptr };
	CCollider*							m_pColliderCom	= { nullptr };

	_float4x4							m_matProj		= {};
	_float4x4							m_matView		= {};
	_float4x4							m_matWorld		= {};

	MAPOBJ_DESC*						m_pDesc;

	D3D11_VIEWPORT						m_pViewPort;

public:
	static CMap_Object*					Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CGameObject*						Clone(void* pArg);
	virtual void						Free() override;
};

END