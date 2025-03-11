#include "Bone.h"
#include "GameObject.h"

Bone::Bone()
{
}

HRESULT Bone::Initialize(const aiNode* pAINode, _int iParentBoneIndex)
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

void Bone::Update_CombinedTransformationMatrix(const vector<class Bone*>& Bones, const _float4x4* pPreTransformMatrix)
{
    if (-1 == m_iParentBoneIndex)
        XMStoreFloat4x4(&m_matCombinedTransform,
            XMLoadFloat4x4(&m_matTransform) *
            XMLoadFloat4x4(pPreTransformMatrix));

    else
       XMStoreFloat4x4(&m_matCombinedTransform,
           XMLoadFloat4x4(&m_matTransform) * XMLoadFloat4x4(&Bones[m_iParentBoneIndex]->m_matCombinedTransform));

}

void Bone::Update_Combine_RootMatrix(const vector<class Bone*>& Bones, const _float4x4* pPreTransformMatrix, GameObject* pObject)
{
    if (-1 == m_iParentBoneIndex)
        XMStoreFloat4x4(&m_matCombinedTransform,
            XMLoadFloat4x4(&m_matTransform) *
            XMLoadFloat4x4(pPreTransformMatrix));

    else if (2 == m_iParentBoneIndex)
    {
        //_float f42;
        //f42 = Bones[2]->Get_CombinedTransformfloat4x4ptr()->m[3][2];
        //
        //_vector vecSum, vec1, vec2;
        //vec1 = XMVectorSetW(Bones[2]->Get_CombinedTransformationMatrix().r[3], 0.f);
        //vec2 = pObject->Get_Transform()->Get_State(Transform::STATE_POS);
        //
        //vecSum = vec1 + vec2;
        //// 계속 더하게 되면 문제가 됨
        //
        //pObject->Get_Transform()->Set_State(Transform::STATE_POS, vec2);

        //XMStoreFloat4x4(&m_matCombinedTransform,
        //    XMLoadFloat4x4(&m_matTransform) * XMLoadFloat4x4(&Bones[m_iParentBoneIndex]->m_matCombinedTransform));


        _float4x4 matPreTransform;
        _float4 vPreTrans, vDelta, vNonTrans {0.f,0.f,0.f,1.f};

        XMStoreFloat4x4(&matPreTransform,
            XMLoadFloat4x4(&m_matTransform) * XMLoadFloat4x4(&Bones[m_iParentBoneIndex]->m_matCombinedTransform));

        memcpy(&vPreTrans, &matPreTransform.m[3][0], sizeof(_float4));
        memcpy(&matPreTransform.m[3][0], &vNonTrans, sizeof(_float4));

        XMStoreFloat4(&vDelta, XMLoadFloat4(&vPreTrans) - XMLoadFloat4(&m_vDelta));

        m_vDelta = vDelta;

        pObject->Get_Transform()->Set_State(Transform::STATE_POS,
            pObject->Get_Transform()->Get_State(Transform::STATE_POS) +
            XMVectorSetW(XMLoadFloat4(&m_vDelta), 0.f));

        XMStoreFloat4x4(&m_matCombinedTransform,
            XMLoadFloat4x4(&matPreTransform));

        int a = 10;

    }


    else
        XMStoreFloat4x4(&m_matCombinedTransform,
            XMLoadFloat4x4(&m_matTransform) * XMLoadFloat4x4(&Bones[m_iParentBoneIndex]->m_matCombinedTransform));

}

Bone* Bone::Create(const aiNode* pAINode, _int iParentBoneIndex)
{
    Bone* pInstance = new Bone();

    if (FAILED(pInstance->Initialize(pAINode, iParentBoneIndex)))
    {
        MSG_BOX("Failed To Created : Bone");
        Safe_Release(pInstance);
    }

    return pInstance;
}

Bone* Bone::Clone()
{
    return new Bone(*this);
}


void Bone::Free()
{
}
