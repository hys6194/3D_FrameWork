#include "Bounding_AABB.h"
#include "Bounding_OBB.h"
#include "Bounding_Sphere.h"
#include "DebugDraw.h"
#include "GameInstance.h"

CBounding_AABB::CBounding_AABB(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
    : CBounding { pDevice, pContext }
{
}

HRESULT CBounding_AABB::Initialize(const CBounding_AABB::BOUNDING_AABB_DESC* pDesc, class CCollider* pOwner)
{
    const BOUNDING_AABB_DESC* pBoundDesc = static_cast<const BOUNDING_AABB_DESC*>(pDesc);

    m_pOwner = pOwner;

    m_pLocalDesc = new BoundingBox(pBoundDesc->vCenter, pBoundDesc->vExtents);
    m_pDesc = new BoundingBox(*m_pLocalDesc);

    m_tInfo.iOption = pBoundDesc->iOption;
    m_tInfo.strCollTag = pBoundDesc->strCollTag;
    m_eType = pBoundDesc->eType;

    m_pGameInstance->Add_Collistionlist(pBoundDesc->iOption, pBoundDesc->strCollTag, this);

    return S_OK;
}

void CBounding_AABB::Update(_fmatrix WorldMatrix)
{
    _matrix     TransformMatrix = WorldMatrix;

    TransformMatrix.r[0] = XMVectorSet(1.f, 0.f, 0.f, 0.f) * XMVector3Length(WorldMatrix.r[0]);
    TransformMatrix.r[1] = XMVectorSet(0.f, 1.f, 0.f, 0.f) * XMVector3Length(WorldMatrix.r[1]);
    TransformMatrix.r[2] = XMVectorSet(0.f, 0.f, 1.f, 0.f) * XMVector3Length(WorldMatrix.r[2]);

    m_pLocalDesc->Transform(*m_pDesc, TransformMatrix);
}

_bool CBounding_AABB::Intersect(TYPE eType, CBounding* pTargetBound)
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
HRESULT CBounding_AABB::Render(PrimitiveBatch<VertexPositionColor>* pBatch, _fvector vColor)
{
    DX::Draw(pBatch, *m_pDesc, vColor);


    return S_OK;
}
#endif

_bool CBounding_AABB::Intersect_AABB(CBounding_AABB* pTargetBound)
{
    _float3     vSourMin, vSourMax;
    _float3     vDestMin, vDestMax;

    vSourMin = Compute_Min();
    vSourMax = Compute_Max();

    vDestMin = pTargetBound->Compute_Min();
    vDestMax = pTargetBound->Compute_Max();

    /* x축선상에서의 비교. */
    if (max(vSourMin.x, vDestMin.x) > min(vSourMax.x, vDestMax.x))
        return false;
    if (max(vSourMin.y, vDestMin.y) > min(vSourMax.y, vDestMax.y))
        return false;
    if (max(vSourMin.z, vDestMin.z) > min(vSourMax.z, vDestMax.z))
        return false;

    return true;
}

_float3 CBounding_AABB::Compute_Min()
{
    return _float3(m_pDesc->Center.x - m_pDesc->Extents.x, m_pDesc->Center.y - m_pDesc->Extents.y, m_pDesc->Center.z - m_pDesc->Extents.z);
}

_float3 CBounding_AABB::Compute_Max()
{
    return _float3(m_pDesc->Center.x + m_pDesc->Extents.x, m_pDesc->Center.y + m_pDesc->Extents.y, m_pDesc->Center.z + m_pDesc->Extents.z);
}

CBounding_AABB* CBounding_AABB::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const CBounding_AABB::BOUNDING_AABB_DESC* pDesc, class CCollider* pOwner)
{
    CBounding_AABB* pInstance = new CBounding_AABB(pDevice, pContext);

    if (FAILED(pInstance->Initialize(pDesc, pOwner)))
    {
        MSG_BOX("Failed To Created : CBounding_AABB");
        Safe_Release(pInstance);
    }

    return pInstance;
}

void CBounding_AABB::Free()
{
    __super::Free();

    Safe_Delete(m_pLocalDesc);
    Safe_Delete(m_pDesc);
}
