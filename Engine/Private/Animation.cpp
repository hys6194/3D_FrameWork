#include "Animation.h"
#include "Channel.h"

Animation::Animation()
{
}

HRESULT Animation::Initialize(const aiAnimation* pAIAnimation, const vector<class Bone*>& pBone, vector<_uint>& iCurrentKeyFrameIndices)
{
    strcpy_s(m_szName, pAIAnimation->mName.data);

    m_iNumChannel = pAIAnimation->mNumChannels;

    m_fTrickPerSecond = pAIAnimation->mTicksPerSecond;
    m_fDuration = pAIAnimation->mDuration;

    iCurrentKeyFrameIndices.resize(m_iNumChannel);

    for (size_t i = 0; i < m_iNumChannel; ++i)
    {
        Channel* pChannel = Channel::Create(pAIAnimation->mChannels[i], pBone);
        if(nullptr == pChannel)
            return E_FAIL;

        m_vecChannel.push_back(pChannel);
    }

    return S_OK;
}

_bool Animation::Update_TransformationMatrix(const vector<class Bone*>& pBone, _float fTimeDelta, _bool bIsLoop, _float* pCurrentTrackPosition, vector<_uint>& vecKeyFrameIndex)
{
    // 현재 재생위치를 구한다
    *pCurrentTrackPosition += m_fTrickPerSecond * fTimeDelta;

    // KeyFrame이 끝에 도달한다면
    if(*pCurrentTrackPosition >= m_fDuration)
    {
        // 애니메이션의 연속재생의 여부 확인
        if (true == bIsLoop)
            *pCurrentTrackPosition = 0.f;
        else
            m_bIsAnimEnd = true;
    }

    for (size_t i = 0; i < m_iNumChannel; i++)
    {
        // 뼈의 정보와 현재 KeyFrame의 위치를 인자로 전달
        m_vecChannel[i]->Update_TransformationMatrix(pBone, *pCurrentTrackPosition, &vecKeyFrameIndex[i]);
    }

    return m_bIsAnimEnd;
}

void Animation::KeyFrame_Reset()
{

    m_bIsAnimEnd = false;
}

Animation* Animation::Create(const aiAnimation* pAIAnimation, const vector<class Bone*>& pBone, vector<_uint>& iCurrentKeyFrameIndices)
{
    Animation* pInstance = new Animation();

    if (FAILED(pInstance->Initialize(pAIAnimation, pBone, iCurrentKeyFrameIndices)))
    {
        MSG_BOX("Failed To Created : Animation");
        Safe_Release(pInstance);
    }

    return pInstance;
}


void Animation::Free()
{
    __super::Free();

    for (auto& pChannel : m_vecChannel)
        Safe_Release(pChannel);

    m_vecChannel.clear();

}
