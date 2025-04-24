#pragma once

#include "Trail.h"

BEGIN(Engine)

class CNormal_Trail final: public CTrail
{
public:
	typedef struct tagNormalTrailDesc : public CTrail::TRAIL_DESC
	{

	}NORMALTRAIL_DESC;

private:
	CNormal_Trail(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CNormal_Trail(const CNormal_Trail& Prototype);
	virtual ~CNormal_Trail() = default;

public:
	virtual HRESULT			Initialize_Prototype() override;
	virtual HRESULT			Initialize(void* pArg) override;
	virtual void			Priority_Update(_float fTimeDelta) override;
	virtual void			Update(_float fTimeDelta) override;
	virtual void			Late_Update(_float fTimeDelta) override;
	virtual HRESULT			Render() override;

	virtual HRESULT			Bind_SR() override ;
	virtual HRESULT			Ready_Component() override ;

private:
	class CVIBuffer_Trail*  m_pVIBufferCom = { nullptr };

public:
	static  CNormal_Trail*	Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject*	Clone(void* pArg) override;
	virtual void			Free() override; 
};

END