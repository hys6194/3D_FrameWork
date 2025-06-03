#include "Body_Monster.h"
#include "Monster.h"

#include "GameInstance.h"

CBody_Monster::CBody_Monster(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CPartObject{ pDevice, pContext }
{
}

CBody_Monster::CBody_Monster(const CBody_Monster& Prototype)
	: CPartObject{ Prototype }
	, m_pTargetState { Prototype.m_pTargetState }
	//, m_pShaderCom { Prototype.m_pShaderCom }
	//, m_pModelCom{ Prototype.m_pModelCom }
	, m_mapSocketmat{ Prototype.m_mapSocketmat }
	, m_fDeadTime { Prototype.m_fDeadTime }
{
	Safe_AddRef(m_pShaderCom);
	Safe_AddRef(m_pModelCom); 
}

const _float4x4* CBody_Monster::Get_f4SocketMatrix(const _wstring& strSocketName)
{
	auto iter = m_mapSocketmat.find(strSocketName);
	if (iter == m_mapSocketmat.end())
		return nullptr;

	return iter->second;
}

HRESULT CBody_Monster::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CBody_Monster::Initialize(void* pArg)
{
	NULL_CHECK_RETURN(pArg, E_FAIL);

	BODY_MONSTER_DESC* pDesc = static_cast<BODY_MONSTER_DESC*>(pArg);
	m_pTargetState = *pDesc->pTargetState;
	m_pOwner = pDesc->pOwner;
	m_iPassIndex = 0;

	FAILED_CHECK_RETURN(__super::Initialize(pDesc), E_FAIL);

	return S_OK;
}

void CBody_Monster::Priority_Update(_float fTimeDelta)
{

}

void CBody_Monster::Update(_float fTimeDelta)
{
	XMStoreFloat4x4(&m_CombinedWorldMatrix,
		XMLoadFloat4x4(m_pTransformCom->Get_WorldMatrix_Ptr()) * XMLoadFloat4x4(m_pParentMatrix));

	if (m_pOwner->Is_Hit())
	{
		m_bHit = true;
		m_fHitTime = 0.f;
	}

	if (m_bHit)
	{
		m_fHitTime += fTimeDelta * 5.f;

		m_iPassIndex = 2;

		if (m_fHitTime >= 1.f)
		{
			m_fHitTime = 0.f;
			m_iPassIndex = 0;
			m_bHit = false;
			return; 
		}

		FAILED_CHECK_RETURN(m_pShaderCom->Bind_RawValue("fTime", &m_fHitTime, sizeof(_float)), );
	}

	if (m_pOwner->Is_Dead())
	{
		m_iPassIndex = 1;

		m_fDeadTime += fTimeDelta / 3.f;

		if (m_fDeadTime >= 1.f)
		{
		
			m_fDeadTime = 1.f;
		}

		FAILED_CHECK_RETURN(m_pShaderCom->Bind_RawValue("g_fDissolveTime", &m_fDeadTime, sizeof(_float)), );
	}



}

void CBody_Monster::Late_Update(_float fTimeDelta)
{
	m_pGameInstance->Add_RenderObject(CRenderer::RENDER_NONBLEND, this);
}

HRESULT CBody_Monster::Render()
{
	if (m_fDeadTime >= 1.f)
		return E_ABORT;

	_uint		iNumMeshes = m_pModelCom->Get_NumMeshes();

	for (size_t i = 0; i < iNumMeshes; i++)
	{
		if (FAILED(m_pModelCom->Bind_Material(m_pShaderCom, "g_DiffuseTexture",
			aiTextureType_DIFFUSE, i, 0)))
			return E_FAIL;

		m_pModelCom->Bind_BoneMatrix(m_pShaderCom, "g_BoneMatrices", i);

		if (FAILED(m_pShaderCom->Begin(m_iPassIndex)))
			return E_FAIL;

		if (FAILED(m_pModelCom->Render(i)))
			return E_FAIL;
	}

	return S_OK;

}

HRESULT CBody_Monster::Ready_Component()
{
	if (FAILED(__super::Add_Component(LEVEL_GAMEPLAY, PRO_TEX_DISSOLVE,
		reinterpret_cast<CComponent**>(&m_pTextureCom), TEXT("Com_Texture_Mask"))))
		return E_FAIL;

	return S_OK;
}

HRESULT CBody_Monster::Ready_SocketMatrices()
{
	return S_OK;
}

HRESULT CBody_Monster::Bind_SR()
{
	return S_OK;
}

CBody_Monster* CBody_Monster::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CBody_Monster* pInstance = new CBody_Monster(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed To Created : Body_Ghoul");
		Safe_Release(pInstance);
	}
	return pInstance;

}

CGameObject* CBody_Monster::Clone(void* pArg)
{
	CBody_Monster* pInstance = new CBody_Monster(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed To Cloned : Body_Ghoul");
		Safe_Release(pInstance);
	}

	return pInstance;
}


void CBody_Monster::Free()
{
	__super::Free();

	Safe_Release(m_pShaderCom);
	Safe_Release(m_pModelCom);
}
