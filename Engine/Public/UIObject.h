#pragma once

#include "GameObject.h"

BEGIN(Engine)

class ENGINE_DLL UIObject abstract : public GameObject
{
public:
	typedef struct tagUIObjectDesc : public GameObject::GAMEOBJECT_DESC
	{
		_float		fX, fY, fSizeX, fSizeY;
	}UIOBJECT_DESC;

protected:
	UIObject(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	UIObject(const UIObject& Prototype);
	virtual ~UIObject() = default;

public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;
	virtual void Priority_Update(_float fTimeDelta) override;
	virtual void Update(_float fTimeDelta) override;
	virtual void Late_Update(_float fTimeDelta) override;
	virtual HRESULT Render() override;

protected:
	_float					m_fX{}, m_fY{}, m_fSizeX{}, m_fSizeY{};
	_float4x4				m_ViewMatrix{}, m_ProjMatrix{};

public:
	virtual GameObject* Clone(void* pArg) = 0;
	virtual void Free() override;

};

END