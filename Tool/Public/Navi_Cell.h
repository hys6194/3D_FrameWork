#pragma once

// 이 클래스를 어떻게 만들어야 할까?
// 내 의도는 삼각형을 그리고 좌표를 저장하는 클래스로 만들고 싶어
// 일단 이 클래스는 Navi 버튼을 눌렀을 때 Create하고 해제 했을 때 동적할당 해제
// Chill하게 생각해보자
// 우선 0번 정점의 위치를 선정했을 때 Render하는 것으로 목표를 잡아보자
// 그러면 이거 상속을 어떻게 할까? 그리고 이건 Tool로 가야 할 듯한데
//

#include "Tool_Defines.h"
#include "GameObject.h"

BEGIN(Engine)
class CShader;			// 셰이더 하나만 있으면 되려나
class CVIBuffer_Cell;   // 
END

BEGIN(Tool)

class CNavi_Cell : public CBase
{
private:
	CNavi_Cell(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	//CNavi_Cell(const CNavi_Cell& Prototype);
	virtual ~CNavi_Cell() = default;

public:
	//virtual HRESULT					Initialize_Prototype();
	//virtual HRESULT					Initialize(void* pArg);
	HRESULT							Initialize();
	void							Update(_float fTimeDelta);
	HRESULT							Render();

public:
	HRESULT							Bind_SR();
	HRESULT							Ready_Component();
	// 야 이거 상속 게임 오브젝트로 하는게 맞다
	// 그래야 트랜스 폼으로 값을 저장할 수 있겠음 또한 값 수정까지 편할듯 해

private:
	CShader*						m_pShaderCom	 = { nullptr };
	CVIBuffer_Cell*					m_pVIBufferCom	 = { nullptr };

	ID3D11Device*					m_pDevice		= { nullptr };
	ID3D11DeviceContext*			m_pContext		= { nullptr };
	CGameInstance*					m_pGameInstance = { nullptr };

public:
	static CNavi_Cell*				Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	//virtual CGameObject*			Clone(void* pArg);
	virtual void					Free() override;
};

END