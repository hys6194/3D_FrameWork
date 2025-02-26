#include "Bone.h"

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

void Bone::Update_CombinedTransformationMatrix(const vector<class Bone*>& Bones)
{
    if (-1 == m_iParentBoneIndex)
        m_matCombinedTransform = m_matTransform;

    else
        XMStoreFloat4x4(&m_matCombinedTransform,
            XMLoadFloat4x4(&m_matTransform) * XMLoadFloat4x4(&Bones[m_iParentBoneIndex]->m_matCombinedTransform));
}

Bone* Bone::Create(const aiNode* pAINode, _int iParentBoneIndex)
{
    Bone* pInstance = new Bone();

    if (FAILED(pInstance->Initialize(pAINode, iParentBoneIndex)))
    {
        MSG_BOX("Failed To Created : Mesh");
        Safe_Release(pInstance);
    }

    return pInstance;
}

void Bone::Free()
{
    __super::Free();
}
