#pragma once

#include "Base.h"

BEGIN(Engine)

class CRenderer final : public CBase
{
public:
	enum RENDERERGROUP { RENDER_PRIORITY, RENDER_NONLIGHT, RENDER_NONBLEND, RENDER_BLEND, RENDER_UI, RENDER_END };

private:
	CRenderer(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual ~CRenderer() = default;

public:
	HRESULT						Initialize();
	HRESULT						Add_RenderObject(RENDERERGROUP eRenderGroup, class CGameObject* pRenderObject);
	void						Draw();
	void						Clear();


private:
	ID3D11Device*				m_pDevice = { nullptr };
	ID3D11DeviceContext*		m_pContext = { nullptr };
	class CGameInstance*		m_pGameInstance = { nullptr };

	//렌더하려고 하는 오브젝트들은 GameObject를 상속받아서 사용하고 있음
	// 따라서 CGameObject* 를 담아서 관리하는 STL을 사용하는 것이 유리하다
	// 왜 리스트로? -> 추가 및 삭제가 용이하기 때문
	// 왜 추가 및 삭제를 하나? 레벨의 변경(스테이지) 및 컬링을 통한 삭제를 위해서
	list<class CGameObject*>	m_listRenderer[RENDER_END];

	_float4x4					m_matWorld{}, m_matView{}, m_matProj{};
	class CVIBuffer_Rect*		m_pVIBuffer = { nullptr };
	class CShader*				m_pShader = { nullptr };

private:
	HRESULT						Render_Priority();
	HRESULT						Render_NonLight();
	HRESULT						Render_NonBlend();
	HRESULT						Render_Blend();
	HRESULT						Render_UI();

private:
	HRESULT						Render_Lights();
	HRESULT						Render_Deferred();


#ifdef _DEBUG
private:
	HRESULT						Render_Debug();

#endif


public:
	static CRenderer* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual void Free() override;


};

END