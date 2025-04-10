#include "Bounding_AABB.h"
#include "Bounding_OBB.h"
#include "Bounding_Sphere.h"
#include "DebugDraw.h"
#include "GameInstance.h"

CBounding_Sphere::CBounding_Sphere(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
    : CBounding { pDevice, pContext }
{
}

HRESULT CBounding_Sphere::Initialize(const CBounding_Sphere::BOUNDING_SPHERE_DESC* pDesc, class CCollider* pOwner)
{
    const BOUNDING_SPHERE_DESC* pBoundDesc = static_cast<const BOUNDING_SPHERE_DESC*>(pDesc);

    m_pOwner = pOwner;

    m_pLocalDesc = new BoundingSphere(pBoundDesc->vCenter, pBoundDesc->fRadius);
    m_pDesc = new BoundingSphere(*m_pLocalDesc);

    m_eType = pBoundDesc->eType;
    m_tInfo.iOption = pBoundDesc->iOption;
    m_tInfo.strCollTag = pBoundDesc->strCollTag;

    m_pGameInstance->Add_Collistionlist(pBoundDesc->iOption, pBoundDesc->strCollTag, this);

    return S_OK;
}

void CBounding_Sphere::Update(_fmatrix WorldMatrix)
{   
    m_pLocalDesc->Transform(*m_pDesc, WorldMatrix);
}

_bool CBounding_Sphere::Intersect(TYPE eType, CBounding* pTargetBound)
{
    _bool     isColl = { false };

    switch (eType)
    {
        case TYPE_AABB:
        {
            BoundingBox* pDesc = static_cast<CBounding_AABB*>(pTargetBound)->Get_Desc();
            isColl = m_pDesc->Intersects(*pDesc);
            break;
        }
        case TYPE_OBB:
        {
            BoundingOrientedBox* pDesc1 = static_cast<CBounding_OBB*>(pTargetBound)->Get_Desc();
            isColl = m_pDesc->Intersects(*pDesc1);
            break;
        }
        case TYPE_SPHERE:
        {
            BoundingSphere* pDesc2 = static_cast<CBounding_Sphere*>(pTargetBound)->Get_Desc();
            isColl = m_pDesc->Intersects(*pDesc2);
            break;
        }
    }

    return isColl;
}

#ifdef _DEBUG
HRESULT CBounding_Sphere::Render(PrimitiveBatch<VertexPositionColor>* pBatch, _fvector vColor)
{

    DX::Draw(pBatch, *m_pDesc, vColor);


    return S_OK;
}
#endif

CBounding_Sphere* CBounding_Sphere::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const CBounding_Sphere::BOUNDING_SPHERE_DESC* pDesc, class CCollider* pOwner)
{
    CBounding_Sphere* pInstance = new CBounding_Sphere(pDevice, pContext);

    if (FAILED(pInstance->Initialize(pDesc, pOwner)))
    {
        MSG_BOX("Failed To Created : CBounding_Sphere");
        Safe_Release(pInstance);
    }

    return pInstance;
}

void CBounding_Sphere::Free()
{
    __super::Free();

    Safe_Delete(m_pLocalDesc);
    Safe_Delete(m_pDesc);
}
