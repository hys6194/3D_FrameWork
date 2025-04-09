#include "Bounding_Sphere.h"
#include "DebugDraw.h"

CBounding_Sphere::CBounding_Sphere(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
    : CBounding{ pDevice, pContext }
{
}

HRESULT CBounding_Sphere::Initialize(const CBounding::BOUNDING_DESC* pDesc)
{
    const BOUNDING_SPHERE_DESC* pBoundDesc = static_cast<const BOUNDING_SPHERE_DESC*>(pDesc);

    m_pLocalDesc = new BoundingSphere(pBoundDesc->vCenter, pBoundDesc->fRadius);
    m_pDesc = new BoundingSphere(*m_pLocalDesc);

    return S_OK;
}

void CBounding_Sphere::Update(_fmatrix WorldMatrix)
{
    m_pLocalDesc->Transform(*m_pDesc, WorldMatrix);
}

_bool CBounding_Sphere::Intersect(CCollider::TYPE eType, CBounding* pTargetBound)
{
    void* pDesc = pTargetBound->Get_Desc();

    _bool     isColl = { false };
    switch (eType)
    {
    case CCollider::TYPE_AABB:
        isColl = m_pDesc->Intersects(*static_cast<BoundingBox*>(pDesc));
        break;
    case CCollider::TYPE_OBB:
        isColl = m_pDesc->Intersects(*static_cast<BoundingOrientedBox*>(pDesc));
        break;
    case CCollider::TYPE_SPHERE:
        isColl = m_pDesc->Intersects(*static_cast<BoundingSphere*>(pDesc));
        break;
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

CBounding_Sphere* CBounding_Sphere::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const CBounding::BOUNDING_DESC* pDesc)
{
    CBounding_Sphere* pInstance = new CBounding_Sphere(pDevice, pContext);

    if (FAILED(pInstance->Initialize(pDesc)))
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
