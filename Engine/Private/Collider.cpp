#include "Collider.h"

#include "GameObject.h"
#include "Bounding_AABB.h"
#include "Bounding_OBB.h"
#include "Bounding_Sphere.h"
#include "GameInstance.h"

CCollider::CCollider(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CComponent  { pDevice, pContext }
{
}

CCollider::CCollider(const CCollider& Prototype)
	: CComponent { Prototype } 
    , m_eColliderType { Prototype.m_eColliderType }
#ifdef _DEBUG
    , m_pBatch { Prototype.m_pBatch }
    , m_pEffect { Prototype.m_pEffect }
    , m_pInputLayout { Prototype.m_pInputLayout }
#endif
{
#ifdef _DEBUG
    Safe_AddRef(m_pInputLayout);
#endif
}

HRESULT CCollider::Initialize_Prototype()
{

#ifdef _DEBUG

    m_pBatch = new PrimitiveBatch<VertexPositionColor>(m_pContext);
    m_pEffect = new BasicEffect(m_pDevice);

    m_pEffect->SetVertexColorEnabled(true);

    const void* pShaderByteCode = { nullptr };
    size_t      iLength = {};

    m_pEffect->GetVertexShaderBytecode(&pShaderByteCode, &iLength);

    m_pDevice->CreateInputLayout(VertexPositionColor::InputElements, VertexPositionColor::InputElementCount, pShaderByteCode, iLength, &m_pInputLayout);

#endif

	return S_OK;
}

HRESULT CCollider::Initialize_Prototype(TYPE eType)
{
    m_eColliderType = eType;

#ifdef _DEBUG

    m_pBatch = new PrimitiveBatch<VertexPositionColor>(m_pContext);
    m_pEffect = new BasicEffect(m_pDevice);

    m_pEffect->SetVertexColorEnabled(true);

    const void* pShaderByteCode = { nullptr };
    size_t      iLength = {};

    m_pEffect->GetVertexShaderBytecode(&pShaderByteCode, &iLength);

    m_pDevice->CreateInputLayout(VertexPositionColor::InputElements, VertexPositionColor::InputElementCount, pShaderByteCode, iLength, &m_pInputLayout);

#endif

    return S_OK;
}

HRESULT CCollider::Initialize(void* pArg)
{

    switch (m_eColliderType)
    {
        case TYPE_AABB:
        {
            const CBounding_AABB::BOUNDING_AABB_DESC* pDesc = static_cast<const CBounding_AABB::BOUNDING_AABB_DESC*>(pArg);
            m_pOwner = pDesc->pOwner;
            m_pBounding = CBounding_AABB::Create(m_pDevice, m_pContext, pDesc, this);
            break;
        }
        case TYPE_OBB:
        {
            const CBounding_OBB::BOUNDING_OBB_DESC* pDesc = static_cast<const CBounding_OBB::BOUNDING_OBB_DESC*>(pArg);
            m_pOwner = pDesc->pOwner;
            m_pBounding = CBounding_OBB::Create(m_pDevice, m_pContext, pDesc, this);
            break;
        }
        case TYPE_SPHERE:
        {
            const CBounding_Sphere::BOUNDING_SPHERE_DESC* pDesc = static_cast<const CBounding_Sphere::BOUNDING_SPHERE_DESC*>(pArg);
            m_pOwner = pDesc->pOwner;

            m_pBounding = CBounding_Sphere::Create(m_pDevice, m_pContext, pDesc, this);
            break;
        }
    }

	return S_OK;
}

void CCollider::Update(_fmatrix WorldMatrix)
{
    m_pBounding->Update(WorldMatrix);

    // 피격 처리
    if (m_pTargetBounding != nullptr)
    {
        switch (m_pTargetBounding->Get_Info()->eType)
        {
            case TYPE_AABB:
            {
                wstring strTest = this->m_pOwner->Get_Name();

                if (Check_IncWord(this->m_pOwner->Get_Name(), TEXT("Monster")))
                {

                }
                break;
            }
            case TYPE_OBB:
            {
                wstring strTest = this->m_pOwner->Get_Name();

                int a = 10;

                break;
            }
            case TYPE_SPHERE:
            {
                wstring strTest = this->m_pOwner->Get_Name();

                int a = 10;

                break;
            }
            default:
                break;
        }
    }
}

_bool CCollider::Intersect(CCollider* pTargetCollider)
{
    m_isColl = m_pBounding->Intersect(pTargetCollider->m_eColliderType, pTargetCollider->m_pBounding);       

    if (false == pTargetCollider->m_isColl && true == m_isColl)
    {
        pTargetCollider->m_isColl = true;
    }

    return m_isColl;

    
}

#ifdef _DEBUG

void CCollider::Check_CollisionHit()
{

}

HRESULT CCollider::Render()
{
    // 지오메트리셰이더를 그릴 때 
    m_pContext->GSSetShader(nullptr, nullptr, 0);

    m_pContext->IASetInputLayout(m_pInputLayout);

    m_pEffect->SetWorld(XMMatrixIdentity());
    m_pEffect->SetView(m_pGameInstance->Get_Transform_Matrix(CPipeLine::D3DTS_VIEW));
    m_pEffect->SetProjection(m_pGameInstance->Get_Transform_Matrix(CPipeLine::D3DTS_PROJ));

    m_pEffect->Apply(m_pContext);

    m_pBatch->Begin();

    m_pBounding->Render(m_pBatch, false == m_isColl ? XMVectorSet(0.f, 1.f, 0.f, 1.f) : XMVectorSet(1.f, 0.f, 0.f, 1.f));

    m_pBatch->End();

    return S_OK;
}

#endif

CCollider* CCollider::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
    CCollider* pInstance = new CCollider(pDevice, pContext);

    if (FAILED(pInstance->Initialize_Prototype()))
    {
        MSG_BOX("Failed To Created : CCollider");
        Safe_Release(pInstance);
    }

    return pInstance;
}

CCollider* CCollider::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, TYPE eType)
{
    CCollider* pInstance = new CCollider(pDevice, pContext);

    if (FAILED(pInstance->Initialize_Prototype(eType)))
    {
        MSG_BOX("Failed To Created : CCollider");
        Safe_Release(pInstance);
    }

    return pInstance;
}

CComponent* CCollider::Clone(void* pArg)
{
    CCollider* pInstance = new CCollider(*this);

    if (FAILED(pInstance->Initialize(pArg)))
    {
        MSG_BOX("Failed To Cloned : CCollider");
        Safe_Release(pInstance);
    }

    return pInstance;
}
void CCollider::Free()
{
	__super::Free();

    Safe_Release(m_pBounding);

#ifdef _DEBUG

    if (false == m_isCloned)
    {
        Safe_Delete(m_pBatch);
        Safe_Delete(m_pEffect);
    }

    Safe_Release(m_pInputLayout);
    
#endif


}
