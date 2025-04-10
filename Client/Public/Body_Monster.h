#pragma once

#include "Client_Defines.h"
#include "PartObject.h"

BEGIN(Engine)
class CShader;
class CModel;
END

//추후 보스 몬스터 혹은 일반 몬스터가 무기를 들 수 있으므로
BEGIN(Client)

class CBody_Monster : public CPartObject
{
public:
	typedef struct tagBodyMonsterDesc : public CPartObject::PARTOBJ_DESC
	{
		const _uint* pTargetState = { nullptr };
	}BODY_MONSTER_DESC;

protected:
	CBody_Monster(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CBody_Monster(const CBody_Monster& Prototype);
	virtual ~CBody_Monster() = default;

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

protected:
	virtual HRESULT								Ready_Component() ;
	virtual HRESULT								Ready_SocketMatrices() ;
	virtual HRESULT								Bind_SR() ;

protected:
	_uint										m_pTargetState;

	class CShader*								m_pShaderCom			= { nullptr };
	class CModel*								m_pModelCom				= { nullptr };

	map<const _wstring, const _float4x4*>		m_mapSocketmat;   // 특정 뼈들의 매트릭스를 가지고 있는 map

public:
	// CPartObject을(를) 통해 상속됨
	static CBody_Monster*						Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject*						Clone(void* pArg);
	virtual void								Free() override;
};

END
