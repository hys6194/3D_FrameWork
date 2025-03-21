#include "Bone.h"
#include "GameObject.h"

CBone::CBone()
{
}

HRESULT CBone::Initialize(const aiNode* pAINode, _int iParentBoneIndex)
{
    strcpy_s(m_szName, pAINode->mName.data);
    
    m_iParentBoneIndex = iParentBoneIndex;

    memcpy(&m_matTransform, &pAINode->mTransformation, sizeof(_float4x4));

    XMStoreFloat4x4(&m_matTransform,
        XMMatrixTranspose(XMLoadFloat4x4(&m_matTransform)));

    XMStoreFloat4x4(&m_matCombinedTransform,
        XMMatrixIdentity());

    return S_OK;
}

void CBone::Update_CombinedTransformationMatrix(const vector<class CBone*>& Bones, const _float4x4* pPreTransformMatrix)
{
    if (-1 == m_iParentBoneIndex)
        XMStoreFloat4x4(&m_matCombinedTransform,
            XMLoadFloat4x4(&m_matTransform) *
            XMLoadFloat4x4(pPreTransformMatrix));

    else
       XMStoreFloat4x4(&m_matCombinedTransform,
           XMLoadFloat4x4(&m_matTransform) * XMLoadFloat4x4(&Bones[m_iParentBoneIndex]->m_matCombinedTransform));

}

void CBone::Update_Combine_RootMatrix(const vector<class CBone*>& Bones, const _float4x4* pPreTransformMatrix, CGameObject* pObject)
{
    if (-1 == m_iParentBoneIndex)
        XMStoreFloat4x4(&m_matCombinedTransform,
            XMLoadFloat4x4(&m_matTransform) *
            XMLoadFloat4x4(pPreTransformMatrix));

    else if (1 == m_iParentBoneIndex)
    {
        _float4x4 matPreTransform;
        _float4 vPreTrans, vDelta, vNonTrans {0.f,0.f,0.f,1.f};
    
        XMStoreFloat4x4(&matPreTransform,
            XMLoadFloat4x4(&m_matTransform) * XMLoadFloat4x4(&Bones[m_iParentBoneIndex]->m_matCombinedTransform));

        memcpy(&vPreTrans, &matPreTransform.m[3][0], sizeof(_float4));
    
        // 현재 매트릭스 값에 이동량을 초기화
        memcpy(&matPreTransform.m[3][0], &vNonTrans, sizeof(_float4));
    
        // 이동량 계산
        XMStoreFloat4(&vDelta, XMVectorSetW(XMLoadFloat4(&vPreTrans) - XMLoadFloat4(&m_vPreDelta), 1.f));
        
        // 변화량을 담아서 다른 클래스에서 사용할 수 있게 저장
        // 추후 Transform에서 원하는 값만 가져와서 사용
        m_vCurDelta = vDelta;

        // 이전 값을 저장
        m_vPreDelta = vPreTrans;

        // 이동량을 제거한 매트릭스를 컴바인드 행렬에 대입함으로서 애니메이션의 로컬 이동량 제거
        XMStoreFloat4x4(&m_matCombinedTransform,
            XMLoadFloat4x4(&matPreTransform));

        // 플레이어의 위치에 그대로 입력
        // 방향은 고려되지 않았음
        // pObject->Get_Transform()->Set_State(CTransform::STATE_POS,
        //    pObject->Get_Transform()->Get_State(CTransform::STATE_POS) +
        //    XMVectorSetW(XMLoadFloat4(&m_vCurDelta), 0.f));
    
        // pObject->Get_Transform()->Dash(m_vCurDelta);

        //TCHAR debugMessage[256];
        //_stprintf_s(debugMessage, _T("Debug_Value: x = %.6f, y = %.6f, z = %.6f, w = %.6f\n"), vDelta.x, vDelta.y, vDelta.z, vDelta.w);
        //OutputDebugString(debugMessage);


        //TCHAR debugMessage[256];
        //_stprintf_s(debugMessage, _T("Player_Look: x = %.6f, y = %.6f, z = %.6f, w = %.6f\n"), 
        //    vDelta.x, vDelta.y, vDelta.z, vDelta.w);
        //OutputDebugString(debugMessage);
    
    }


    else
    {   
        XMStoreFloat4x4(&m_matCombinedTransform,
            XMLoadFloat4x4(&m_matTransform) * XMLoadFloat4x4(&Bones[m_iParentBoneIndex]->m_matCombinedTransform));
    }

}

CBone* CBone::Create(const aiNode* pAINode, _int iParentBoneIndex)
{
    CBone* pInstance = new CBone();

    if (FAILED(pInstance->Initialize(pAINode, iParentBoneIndex)))
    {
        MSG_BOX("Failed To Created : Bone");
        Safe_Release(pInstance);
    }

    return pInstance;
}

CBone* CBone::Clone()
{
    return new CBone(*this);
}


void CBone::Free()
{
}
