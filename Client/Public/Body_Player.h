#pragma once

#include "Client_Defines.h"
#include "PartObject.h"

BEGIN(Engine)
class CShader;
class CModel;
END


BEGIN(Client)

class Body_Player final : public CPartObject
{
public:
	typedef struct tagBodyPlayerDesc : public CPartObject::PARTOBJ_DESC
	{
		const _uint* pTargetState = { nullptr };
	}BODY_PLAYER_DESC;

private:
	Body_Player(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	Body_Player(const Body_Player& Prototype);
	virtual ~Body_Player() = default;

public:
	const _float4x4*	Get_f4SocketMatrix(const _wstring& strSocketName);

public:
	CModel* Get_Model() { return m_pModelCom; };
   
public:
	virtual HRESULT		Initialize_Prototype() override;
	virtual HRESULT		Initialize(void* pArg) override;
	virtual void		Priority_Update(_float fTimeDelta) override;
	virtual void		Update(_float fTimeDelta) override;
	virtual void		Late_Update(_float fTimeDelta) override;
	virtual HRESULT		Render() override;

private:
	CShader*									m_pShaderCom = { nullptr };
	CModel*										m_pModelCom = { nullptr };
	CGameObject*								m_pOwner = { nullptr };

private:
	const _uint*								m_pTargetState = { nullptr };				// 플레이어의 상태를 가져와서 애니메이션의 상태를 변경하려고 함
	map<const _wstring, const _float4x4*>		m_mapSocketmat;   // 특정 뼈들의 매트릭스를 가지고 있는 map

private:
	HRESULT				Ready_Components();
	HRESULT				Ready_SocketMatrices();
	HRESULT				Bind_SR();

public:
	static Body_Player* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg);
	virtual void		Free() override;
};

END