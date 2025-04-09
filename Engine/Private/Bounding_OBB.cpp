#include "Bounding_OBB.h"
#include "DebugDraw.h"

CBounding_OBB::CBounding_OBB(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
    : CBounding { pDevice, pContext }
{

}

HRESULT CBounding_OBB::Initialize(const CBounding::BOUNDING_DESC* pDesc)
{
    const BOUNDING_OBB_DESC* pBoundDesc = static_cast<const BOUNDING_OBB_DESC*>(pDesc);

    _float4 vQuaternion = {};
    
    XMStoreFloat4(&vQuaternion, XMQuaternionRotationRollPitchYaw(pBoundDesc->vRotation.x, pBoundDesc->vRotation.y, pBoundDesc->vRotation.z));

    m_pLocalDesc = new BoundingOrientedBox(pBoundDesc->vCenter, pBoundDesc->vExtents, vQuaternion);
    m_pDesc = new BoundingOrientedBox(*m_pLocalDesc);

    m_tInfo.strCollTag = pBoundDesc->strCollTag;
    m_tInfo.eType = pBoundDesc->eType;
    m_tInfo.iOption = pBoundDesc->iOption;
    m_tInfo.bColls = pBoundDesc->bColls;



    return S_OK;
}

void CBounding_OBB::Update(_fmatrix WorldMatrix)
{
    m_pLocalDesc->Transform(*m_pDesc, WorldMatrix);
}

_bool CBounding_OBB::Intersect(COLL_TYPE eType, CBounding* pTargetBound)
{
    void* pDesc = pTargetBound->Get_Desc();

    _bool     isColl = { false };

    switch (eType)
    {
    case TYPE_AABB:
        isColl = m_pDesc->Intersects(*static_cast<BoundingBox*>(pDesc));
        break;
    case TYPE_OBB:
        //isColl = Intersect_OBB(static_cast<CBounding_OBB*>(pTargetBound));
        isColl = m_pDesc->Intersects(*static_cast<BoundingOrientedBox*>(pDesc));
        break;
    case TYPE_SPHERE:
        isColl = m_pDesc->Intersects(*static_cast<BoundingSphere*>(pDesc));
        break;
    }

    return isColl;
}

#ifdef _DEBUG
HRESULT CBounding_OBB::Render(PrimitiveBatch<VertexPositionColor>* pBatch, _fvector vColor)
{
    DX::Draw(pBatch, *m_pDesc, vColor);

    return S_OK;
}
#endif

_bool CBounding_OBB::Intersect_OBB(CBounding_OBB* pTargetBound)
{
    OBB_DESC        OBBDesc[2];

    OBBDesc[0] = Compute_OBBDesc();
    OBBDesc[1] = pTargetBound->Compute_OBBDesc();

    for (size_t i = 0; i < 2; i++)
    {
        for (size_t j = 0; j < 3; j++)
        {
            _float          fLength[3] = {};

            fLength[0] = fabs(XMVector3Dot(XMLoadFloat3(&OBBDesc[1].vCenter) - XMLoadFloat3(&OBBDesc[0].vCenter),
                XMLoadFloat3(&OBBDesc[i].vAlignDir[j])).m128_f32[0]);

            fLength[1] = fabs(XMVector3Dot(XMLoadFloat3(&OBBDesc[0].vCenterDir[0]), XMLoadFloat3(&OBBDesc[i].vAlignDir[j])).m128_f32[0]) +
                fabs(XMVector3Dot(XMLoadFloat3(&OBBDesc[0].vCenterDir[1]), XMLoadFloat3(&OBBDesc[i].vAlignDir[j])).m128_f32[0]) +
                fabs(XMVector3Dot(XMLoadFloat3(&OBBDesc[0].vCenterDir[2]), XMLoadFloat3(&OBBDesc[i].vAlignDir[j])).m128_f32[0]);

            fLength[2] = fabs(XMVector3Dot(XMLoadFloat3(&OBBDesc[1].vCenterDir[0]), XMLoadFloat3(&OBBDesc[i].vAlignDir[j])).m128_f32[0]) +
                fabs(XMVector3Dot(XMLoadFloat3(&OBBDesc[1].vCenterDir[1]), XMLoadFloat3(&OBBDesc[i].vAlignDir[j])).m128_f32[0]) +
                fabs(XMVector3Dot(XMLoadFloat3(&OBBDesc[1].vCenterDir[2]), XMLoadFloat3(&OBBDesc[i].vAlignDir[j])).m128_f32[0]);

            if (fLength[0] > fLength[1] + fLength[2])
                return false;
        }

    }

    return true;
}

CBounding_OBB::OBB_DESC CBounding_OBB::Compute_OBBDesc()
{
    OBB_DESC            OBBDesc{};

    _float3     vPoints[8];

    m_pDesc->GetCorners(vPoints);

    OBBDesc.vCenter = m_pDesc->Center;

    XMStoreFloat3(&OBBDesc.vCenterDir[0], (XMLoadFloat3(&vPoints[5]) - XMLoadFloat3(&vPoints[4])) * 0.5f);
    XMStoreFloat3(&OBBDesc.vCenterDir[1], (XMLoadFloat3(&vPoints[7]) - XMLoadFloat3(&vPoints[4])) * 0.5f);
    XMStoreFloat3(&OBBDesc.vCenterDir[2], (XMLoadFloat3(&vPoints[0]) - XMLoadFloat3(&vPoints[4])) * 0.5f);

    for (size_t i = 0; i < 3; i++)
        XMStoreFloat3(&OBBDesc.vAlignDir[i], XMVector3Normalize(XMLoadFloat3(&OBBDesc.vCenterDir[i])));

    return OBBDesc;
}

CBounding_OBB* CBounding_OBB::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const CBounding::BOUNDING_DESC* pDesc)
{
    CBounding_OBB* pInstance = new CBounding_OBB(pDevice, pContext);

    if (FAILED(pInstance->Initialize(pDesc)))
    {
        MSG_BOX("Failed To Created : CBounding_OBB");
        Safe_Release(pInstance);
    }

    return pInstance;
}

void CBounding_OBB::Free()
{
    __super::Free();

    Safe_Delete(m_pLocalDesc);
    Safe_Delete(m_pDesc);
}
