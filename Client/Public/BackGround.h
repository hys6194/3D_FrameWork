#pragma once

#include "Client_Defines.h"
#include "UIObject.h"

BEGIN(Engine)
class Shader;
class Texture;
class VIBuffer_Rect;
END

BEGIN(Client)

// 직교 투영으로 만들 거라서 UIObject라는 클래스를 만들어서 상속받을 예정

class BackGround final : public UIObject
{
private:
	BackGround(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	BackGround(const BackGround& Prototype);
	virtual ~BackGround() = default;

public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;
	virtual void Priority_Update(_float fTimeDelta) override;
	virtual void Update(_float fTimeDelta) override;
	virtual void Late_Update(_float fTimeDelta) override;
	virtual HRESULT Render() override;

private:
	Texture*				m_pTextureCom = { nullptr };	
	Shader*					m_pShaderCom = { nullptr };
	VIBuffer_Rect*			m_pVIBufferCom = { nullptr };

private:
	HRESULT Ready_Component();

public:
	static BackGround* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual GameObject* Clone(void* pArg);
	virtual void Free() override;

};

END