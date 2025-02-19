#pragma once

#include "Client_Defines.h"
#include "GameObject.h"

BEGIN(Engine)
class Shader;
class Texture;
class VIBuffer_Terrain;
END

BEGIN(Client)

class Terrain final : public GameObject
{
private:
	Terrain(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	Terrain(const Terrain& Prototype);
	virtual ~Terrain() = default;

public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;
	virtual void Priority_Update(_float fTimeDelta) override;
	virtual void Update(_float fTimeDelta) override;
	virtual void Late_Update(_float fTimeDelta) override;
	virtual HRESULT Render() override;

private:
	Shader* m_pShaderCom = { nullptr };
	Texture* m_pTextureCom = { nullptr };
	VIBuffer_Terrain* m_pVIBufferCom = { nullptr };

private:
	HRESULT Ready_Components();
	HRESULT Bind_SR();


public:
	static Terrain* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual GameObject* Clone(void* pArg);
	virtual void Free() override;
};

END