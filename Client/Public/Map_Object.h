#pragma once

#include "Client_Defines.h"
#include "GameObject.h"

BEGIN(Engine)
class CShader;
class CModel;
END

BEGIN(Client)

class CMap_Object final : public CGameObject
{
public:
	typedef struct tagMapObjectDesc : public GAMEOBJECT_DESC
	{
		//해당 맵 오브젝트들의 좌표를 받기위해 만든 구조체 
		//_float4* fScale;
		//_float4* fRotation;
		//_float4* fTraslation;
		//_float4* fPosition;

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
	HRESULT								Ready_Components();
	HRESULT								Bind_SR();

private:
	CShader*							m_pShaderCom = { nullptr };
	CModel*								m_pModelCom = { nullptr };

public:
	static CMap_Object*					Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CGameObject*						Clone(void* pArg);
	virtual void						Free() override;
};

END