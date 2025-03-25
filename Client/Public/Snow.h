#pragma once

#include "Client_Defines.h"
#include "GameObject.h"

BEGIN(Engine)
class CShader;
class CTexture;
class CVIBuffer_Rect_Instancing;
END

BEGIN(Client)

class CSnow final : public CGameObject
{
private:
	CSnow(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CSnow(const CSnow& Prototype);
	virtual ~CSnow() = default;

public:
	virtual HRESULT					Initialize_Prototype() override;
	virtual HRESULT					Initialize(void* pArg) override;
	virtual void					Priority_Update(_float fTimeDelta) override;
	virtual void					Update(_float fTimeDelta) override;
	virtual void					Late_Update(_float fTimeDelta) override;
	virtual HRESULT					Render() override;

private:
	CTexture*						m_pTextureCom = { nullptr };
	CShader*						m_pShaderCom = { nullptr };
	CVIBuffer_Rect_Instancing*		m_pVIBufferCom = { nullptr };

private:
	HRESULT							Ready_Component();
	HRESULT							Bind_SR();

public:
	static CSnow*					Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject*			Clone(void* pArg);
	virtual void					Free() override;
};

END