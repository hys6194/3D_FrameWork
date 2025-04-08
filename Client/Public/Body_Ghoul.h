#pragma once

#include "Client_Defines.h"
#include "PartObject.h"

BEGIN(Engine)
class CShader;
class CModel;
END

//추후 보스 몬스터 혹은 일반 몬스터가 무기를 들 수 있으므로
BEGIN(Client)


class CBody_Ghoul : public CPartObject
{
public:
	typedef struct tagBodyMonsterDesc : public CPartObject::PARTOBJ_DESC
	{
		const _uint* pTargetState = { nullptr };
	}BODY_MONSTER_DESC;

private:
	CBody_Ghoul(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CBody_Ghoul(const CBody_Ghoul& Prototype);
	virtual ~CBody_Ghoul() = default;

public:
	const _float4x4*							Get_f4SocketMatrix(const _wstring& strSocketName);


public:
	class CModel*								Get_Model()				{ return m_pModelCom; }

public:
	virtual HRESULT								Initialize_Prototype() override;
	virtual HRESULT								Initialize(void* pArg) override;
	virtual void								Priority_Update(_float fTimeDelta) override;
	virtual void								Update(_float fTimeDelta) override;
	virtual void								Late_Update(_float fTimeDelta) override;
	virtual HRESULT								Render() override;

private:
	HRESULT										Ready_Component();
	HRESULT										Ready_SocketMatrices();
	HRESULT										Bind_SR();

private:
	_uint										m_pTargetState{};

	class CShader*								m_pShaderCom			= { nullptr };
	class CModel*								m_pModelCom				= { nullptr };

	map<const _wstring, const _float4x4*>		m_mapSocketmat;   // 특정 뼈들의 매트릭스를 가지고 있는 map

public:
	// CPartObject을(를) 통해 상속됨
	static CBody_Ghoul*							Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject*						Clone(void* pArg);
	virtual void								Free() override;
};

END
