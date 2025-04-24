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

class CHP_Frame final : public CUIObject
{
public:
	typedef struct tagHPFrameDesc : public CUIObject::UIOBJECT_DESC
	{
		_wstring strFrameTag;
		_uint	 iPass;
	}HPFRAME_DESC;

private:
	CHP_Frame(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CHP_Frame(const CHP_Frame& Prototype);
	virtual ~CHP_Frame() = default;

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

private:
	HRESULT Ready_Component(const _wstring& strTag);
	HRESULT Bind_SR();

public:
	static CHP_Frame* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg);
	virtual void Free() override;

};

END