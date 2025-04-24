#pragma once

#include "Client_Defines.h"
#include "UIObject.h"

BEGIN(Engine)
class CShader;
class CTexture;
class CVIBuffer_Rect;
END

BEGIN(Client)

// 직교 투영으로 만들 거라서 UIObject라는 클래스를 만들어서 상속받을 예정

class CHP_Bar final : public CUIObject
{
public:
	typedef struct tagHPBarDesc : public UIOBJECT_DESC
	{
		_uint iPass;
	}HPBAR_DESC;
private:
	CHP_Bar(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CHP_Bar(const CHP_Bar& Prototype);
	virtual ~CHP_Bar() = default;

public:
	virtual HRESULT Initialize_Prototype();
	virtual HRESULT Initialize(void* pArg) override;
	virtual void	Priority_Update(_float fTimeDelta) override;
	virtual void	Update(_float fTimeDelta) override;
	virtual void	Late_Update(_float fTimeDelta) override;
	virtual HRESULT Render() override;

private:
	CTexture*				m_pTextureCom = { nullptr };	
	CShader*				m_pShaderCom = { nullptr };
	CVIBuffer_Rect*			m_pVIBufferCom = { nullptr };

private:
	_uint					m_iPassIndex;

	_float					m_fOriginfX;
	_float					m_fOriginfSizeX;

private:
	HRESULT Ready_Component();
	HRESULT Bind_SR();

public:
	static CHP_Bar* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg);
	virtual void Free() override;

};

END