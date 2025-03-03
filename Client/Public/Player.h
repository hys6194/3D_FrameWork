#pragma once

#include "Client_Defines.h"
#include "ContainerObject.h"

BEGIN(Engine)
class FSM;
END

BEGIN(Client)

class Player final : public ContainerObject
{
public:
	enum PARTOBJ { PART_BODY, PART_WEAPON, PART_EFFECT, PART_END };
	enum STATE {
		STATE_IDLE = 0x00000001,
		STATE_RUN = 0x00000002,
		STATE_JUMP = 0x00000004,
		STATE_DOUBLEJUMP = 0x00000008,
		STATE_ATTACK = 0x00000016,
		STATE_NONE = 0x00000000,
	};

private:
	Player(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	Player(const Player& Prototype);
	virtual ~Player() = default;

public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;
	virtual void Priority_Update(_float fTimeDelta) override;
	virtual void Update(_float fTimeDelta) override;
	virtual void Late_Update(_float fTimeDelta) override;
	virtual HRESULT Render() override;


// 파츠들을 모아서 렌더를 할 것인데, PartObject를 상속받는 클래스에서 생성하고 그 클래스에서 렌더를 할 예정이기 때문에 쓸모가 없어짐
//private:
//	CShader*					m_pShaderCom = { nullptr };	
//	CModel*						m_pModelCom = { nullptr };
private:
	_uint					m_iState = { STATE_NONE };
	FSM*					m_pFSMCom = { nullptr };

private:
	HRESULT Ready_Components();
	HRESULT Ready_PartObjects();
	HRESULT Ready_States();
	HRESULT Bind_SR();

public:
	static Player* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual GameObject* Clone(void* pArg);
	virtual void Free() override;
};

END