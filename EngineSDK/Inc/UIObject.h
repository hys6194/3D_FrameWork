#pragma once

#include "GameObject.h"

// 직교 투영이 필요한 GameObject를 그리기 위해서 만든 클래스
// ex) UI, BackGround 등등 윈도우 좌표(화면)에 고정되는 텍스쳐

BEGIN(Engine)

class ENGINE_DLL UIObject abstract : public CGameObject
{
public:
	typedef struct tagUIObjectDesc : public CGameObject::GAMEOBJECT_DESC
	{
		// 화면에 띄울 좌표와 텍스쳐의 사이즈 x,y값
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

	//UIOBJECT_DESC에 있는 값을 받아서 사용하기 위해서 선언
	_float					m_fX{}, m_fY{}, m_fSizeX{}, m_fSizeY{};

	// 직교투영을 위한 뷰행렬과 투영행렬을 선언
	_float4x4				m_ViewMatrix{}, m_ProjMatrix{};

public:
	virtual CGameObject* Clone(void* pArg) = 0;
	virtual void Free() override;

};

END