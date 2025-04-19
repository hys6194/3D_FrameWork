#include "Test_Monster.h"
#include "GameInstance.h"
#include "Moloch.h"

#include "Player.h"
#include "PartObject.h"

CTest_Monster::CTest_Monster(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CGameObject{ pDevice, pContext }
{
}

CTest_Monster::CTest_Monster(const CTest_Monster& Prototype)
	: CGameObject{ Prototype }
{
}

HRESULT CTest_Monster::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CTest_Monster::Initialize(void* pArg)
{
	CGameObject::GAMEOBJECT_DESC	Desc{};

	lstrcpy(Desc.szGameObjectTag, TEXT("GameObject_Terrain"));
	Desc.fSpeedPerSec = 0.f;
	Desc.fRotationPerSec = 0.f;

	if (FAILED(__super::Initialize(&Desc)))
		return E_FAIL;

	if (FAILED(Ready_Components()))
		return E_FAIL;

	m_iAnimIndex = 0;
	//m_pTransformCom->Set_State(CTransform::STATE_POS,
	//	XMVectorSet(m_pGameInstance->Random(0.f, 10.f), 2.f, m_pGameInstance->Random(0.f, 10.f), 1.f));

	m_pModelCom->Set_AnimationIndex(m_iAnimIndex, true);

	return S_OK;
}

void CTest_Monster::Priority_Update(_float fTimeDelta)
{
	int a = 10;

	//if (m_pGameInstance->Key_Down(DIK_1))
	//{
	//	m_iAnimIndex++;
	//	if (16 <= m_iAnimIndex)
	//		m_iAnimIndex = CMoloch::MOLOCH_ATK_180_L;
	//
	//	m_pModelCom->Set_AnimationIndex(m_iAnimIndex, true);
	//}
	//
	//if (m_pGameInstance->Key_Down(DIK_2))
	//{
	//	m_iAnimIndex--;
	//	if (0 > m_iAnimIndex)
	//		m_iAnimIndex = CMoloch::MOLOCH_ATK_SWIPE_02;
	//	
	//	m_pModelCom->Set_AnimationIndex(m_iAnimIndex, true);
	//}

}

void CTest_Monster::Update(_float fTimeDelta)
{
#ifdef _DEBUG
	for (size_t i = 0; i < TYPE_END; i++)
	{
		m_pColliderCom[i]->Update(XMLoadFloat4x4(m_pTransformCom->Get_WorldMatrix_Ptr()));
	}
#endif

	_vector vPos = m_pTransformCom->Get_State(CTransform::STATE_POS);



	if (true == m_pModelCom->Play_Animation(fTimeDelta))
		int a = 10;
}

void CTest_Monster::Late_Update(_float fTimeDelta)
{
	m_pGameInstance->Add_RenderObject(CRenderer::RENDER_NONBLEND, this);

}

HRESULT CTest_Monster::Render()
{
	if (FAILED(Bind_ShaderResources()))
		return E_FAIL;



	_uint		iNumMeshes = m_pModelCom->Get_NumMeshes();

	for (size_t i = 0; i < iNumMeshes; i++)
	{
		if (FAILED(m_pModelCom->Bind_Material(m_pShaderCom, "g_DiffuseTexture",
			aiTextureType_DIFFUSE, i, 0)))
			return E_FAIL;

		m_pModelCom->Bind_BoneMatrix(m_pShaderCom, "g_BoneMatrices", i);

		if (FAILED(m_pShaderCom->Begin(0)))
			return E_FAIL;

		if (FAILED(m_pModelCom->Render(i)))
			return E_FAIL;
	}

	return S_OK;
}

HRESULT CTest_Monster::Ready_Components()
{
	/* Com_Model */
	if (FAILED(__super::Add_Component(LEVEL_GAMEPLAY, PRO_MODEL_DOG_LAVA,
		reinterpret_cast<CComponent**>(&m_pModelCom), TEXT("Com_Model"))))
		return E_FAIL;

	/* Com_Shader */
	if (FAILED(__super::Add_Component(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Shader_VtxAnimMesh"),
		reinterpret_cast<CComponent**>(&m_pShaderCom), TEXT("Com_Shader"))))
		return E_FAIL;

	/* Com_Collider_AABB */
	CBounding_AABB::BOUNDING_AABB_DESC		AABBDesc{};
	AABBDesc.vExtents = _float3(0.3f, 0.6f, 0.3f);
	AABBDesc.vCenter = _float3(0.f, AABBDesc.vExtents.y, 0.f);

	if (FAILED(__super::Add_Component(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Collider_AABB"),
		reinterpret_cast<CComponent**>(&m_pColliderCom[TYPE_AABB]), TEXT("Com_Collider_AABB"), &AABBDesc)))
		return E_FAIL;

	/* Com_Collider_Sphere*/
	CBounding_Sphere::BOUNDING_SPHERE_DESC		SphereDesc{};
	SphereDesc.fRadius = 0.5f;
	SphereDesc.vCenter = _float3(0.f, SphereDesc.fRadius, 0.f);

	if (FAILED(__super::Add_Component(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Collider_Sphere"),
		reinterpret_cast<CComponent**>(&m_pColliderCom[TYPE_SPHERE]), TEXT("Com_Collider_Sphere"), &SphereDesc)))
		return E_FAIL;

	/* Com_Collider_OBB */
	CBounding_OBB::BOUNDING_OBB_DESC		OBBDesc{};
	OBBDesc.vRotation = _float3(0.f, 0.f, 0.f);
	OBBDesc.vExtents = _float3(0.3f, 0.3f, 0.3f);
	OBBDesc.vCenter = _float3(0.f, OBBDesc.vExtents.y, 0.f);

	if (FAILED(__super::Add_Component(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Collider_OBB"),
		reinterpret_cast<CComponent**>(&m_pColliderCom[TYPE_OBB]), TEXT("Com_Collider_OBB"), &OBBDesc)))
		return E_FAIL;

	return S_OK;
}

HRESULT CTest_Monster::Bind_ShaderResources()
{
	if (FAILED(m_pTransformCom->Bind_SR("g_WorldMatrix", m_pShaderCom)))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Bind_VP_Transform_SR("g_ViewMatrix", m_pShaderCom,  CPipeLine::D3DTS_VIEW)))
		return E_FAIL;
	if (FAILED(m_pGameInstance->Bind_VP_Transform_SR("g_ProjMatrix", m_pShaderCom,  CPipeLine::D3DTS_PROJ)))
		return E_FAIL;

	return S_OK;
}

CTest_Monster* CTest_Monster::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CTest_Monster* pInstance = new CTest_Monster(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed To Created : CTest_Monster");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CTest_Monster::Clone(void* pArg)
{
	CTest_Monster* pInstance = new CTest_Monster(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed To Cloned : CTest_Monster");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CTest_Monster::Free()
{
	__super::Free();

#ifdef _DEBUG
	for (size_t i = 0; i < TYPE_END; i++)
	{
		Safe_Release(m_pColliderCom[i]);
	}

#endif

	Safe_Release(m_pShaderCom);
	Safe_Release(m_pModelCom);


}
