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
		// 클라이언트에서 좌표의 위치를 받기 위해서 대충 만듦
		// 행렬을 던지는게 나아보임
		// 해당 맵 오브젝트들의 좌표를 받기위해 만든 구조체 
		//_float4* fScale;
		//_float4* fRotation;
		//_float4* fTraslation;
		//_float4* fPosition;

		wstring strModelTag;
		_uint   iObjectIndex;

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

private:
	HRESULT								Ready_Components(const wstring _strModelTag);
	HRESULT								Bind_SR();

private:
	CShader*							m_pShaderCom	= { nullptr };
	CModel*								m_pModelCom		= { nullptr };
	CCollider*							m_pColliderCom	= { nullptr };

	//_float4x4							m_matProj		= {};
	//_float4x4							m_matView		= {};
	//_float4x4							m_matWorld		= {};

	_uint								m_iID = {};

	D3D11_VIEWPORT						m_pViewPort;

public:
	static CMap_Object*					Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CGameObject*						Clone(void* pArg);
	virtual void						Free() override;
};

END