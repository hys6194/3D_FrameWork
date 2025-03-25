#include "Animation.h"
#include "Channel.h"
#include "GameObject.h"

CAnimation::CAnimation()
{
}

HRESULT CAnimation::Initialize(const aiAnimation* pAIAnimation, const vector<class CBone*>& pBone, vector<_uint>& iCurrentKeyFrameIndices)
{

    // 모든 뼈들을 저장하지 않고 메시에 영향을 주는 뼈들만 
    // Channel에 저장하게 된다. 이는 pAIAnimationd이 
    strcpy_s(m_szName, pAIAnimation->mName.data);

    m_iNumChannel = pAIAnimation->mNumChannels;

    m_fTrickPerSecond = pAIAnimation->mTicksPerSecond;
    m_fDuration = pAIAnimation->mDuration;

    iCurrentKeyFrameIndices.resize(m_iNumChannel);

    for (size_t i = 0; i < m_iNumChannel; ++i)
    {
        CChannel* pChannel = CChannel::Create(pAIAnimation->mChannels[i], pBone);
        if(nullptr == pChannel)
            return E_FAIL;

        m_vecChannel.push_back(pChannel);
    }

    return S_OK;
}

HRESULT CAnimation::Initialize(const vector<class CBone*>& _Bones, ifstream& _InStream, vector<_uint>& _CurrentKeyFrameIndices)
{
    _uint iReadByte = { };
    _char szAnimationName[MAX_PATH] = { };

    _InStream.read(reinterpret_cast<char*>(&iReadByte), sizeof(_uint));
    _InStream.read(reinterpret_cast<char*>(szAnimationName), sizeof(_char) * iReadByte);
    strcpy_s(m_szName, szAnimationName);

    _InStream.read(reinterpret_cast<char*>(&m_fTrickPerSecond), sizeof(_float));

    _InStream.read(reinterpret_cast<char*>(&m_fDuration), sizeof(_float));

    _InStream.read(reinterpret_cast<char*>(&m_iNumChannel), sizeof(_float));

    _CurrentKeyFrameIndices.resize(m_iNumChannel);

    for (size_t i = 0; i < m_iNumChannel; i++)
    {
        CChannel* pChannel = CChannel::Create(_Bones, _InStream);
        if (pChannel == nullptr)
            return E_FAIL;

        m_vecChannel.push_back(pChannel);
    }

    return S_OK;
}

_bool CAnimation::Save_Animation(ofstream& _OpenStream)
{
    if (!_OpenStream)
        return false;

    _uint iWriteByte = sizeof(m_szName);
    _OpenStream.write(reinterpret_cast<const char*>(&iWriteByte), sizeof(_uint));
    _OpenStream.write(m_szName, sizeof(_char) * iWriteByte);

    _OpenStream.write(reinterpret_cast<const char*>(&m_fTrickPerSecond), sizeof(_float));

    _OpenStream.write(reinterpret_cast<const char*>(&m_fDuration), sizeof(_float));

    _OpenStream.write(reinterpret_cast<const char*>(&m_iNumChannel), sizeof(_float));

    for (size_t i = 0; i < m_iNumChannel; i++)
        m_vecChannel[i]->Save_Channel(_OpenStream);

    return true;
}

_bool CAnimation::Update_TransformationMatrix(const vector<class CBone*>& pBone, _float fTimeDelta, _bool bIsLoop, _float* pCurrentTrackPosition, vector<_uint>& vecKeyFrameIndex)
{
    *pCurrentTrackPosition += m_fTrickPerSecond * fTimeDelta;
    
    // KeyFrame이 끝에 도달한다면
    if (*pCurrentTrackPosition >= m_fDuration)
    {
        // 애니메이션의 연속재생의 여부 확인
        if (true == bIsLoop)
            *pCurrentTrackPosition = 0.f;
        else
            m_bIsAnimEnd = true;
    }
    
    for (size_t i = 0; i < m_iNumChannel; i++)
    {
        // 애니메이션을 순회할때는 다른 vector로 순회
        m_vecChannel[i]->Update_TransformationMatrix(pBone, *pCurrentTrackPosition, &vecKeyFrameIndex[i]);
    }

    return m_bIsAnimEnd;
}

_bool CAnimation::Update_TransformationMatrix(const vector<class CBone*>& pBone, _float fTimeDelta, _bool bIsLoop, _float* pCurrentTrackPosition, vector<_uint>& vecKeyFrameIndex, CGameObject* pObject)
{

    //_vector v3 = pObject->Get_Transform()->Get_State(CTransform::STATE_POS);

    *pCurrentTrackPosition += m_fTrickPerSecond * fTimeDelta;

    // KeyFrame이 끝에 도달한다면
    if (*pCurrentTrackPosition >= m_fDuration)
    {
        // 애니메이션의 연속재생의 여부 확인
        if (true == bIsLoop)
            *pCurrentTrackPosition = 0.f;
        else
            m_bIsAnimEnd = true;
    }

    for (size_t i = 0; i < m_iNumChannel; i++)
    {
        // 애니메이션을 순회할때는 다른 vector로 순회
        m_vecChannel[i]->Update_TransformationMatrix(pBone, *pCurrentTrackPosition, &vecKeyFrameIndex[i]);
    }

    return m_bIsAnimEnd;
}

void CAnimation::KeyFrame_Reset()
{
    m_bIsAnimEnd = false;
}

CAnimation* CAnimation::Create(const vector<class CBone*>& _Bones, ifstream& _InStream, vector<_uint>& _CurrentKeyFrameIndices)
{
    CAnimation* pInstance = new CAnimation();

    if (FAILED(pInstance->Initialize(_Bones, _InStream, _CurrentKeyFrameIndices)))
    {
        MSG_BOX("Failed To Created : CAnimation");
        Safe_Release(pInstance);
    }

    return pInstance;
}

CAnimation* CAnimation::Create(const aiAnimation* pAIAnimation, const vector<class CBone*>& pBone, vector<_uint>& iCurrentKeyFrameIndices)
{
    CAnimation* pInstance = new CAnimation();

    if (FAILED(pInstance->Initialize(pAIAnimation, pBone, iCurrentKeyFrameIndices)))
    {
        MSG_BOX("Failed To Created : Animation");
        Safe_Release(pInstance);
    }

    return pInstance;
}


void CAnimation::Free()
{
    __super::Free();

    for (auto& pChannel : m_vecChannel)
        Safe_Release(pChannel);

    m_vecChannel.clear();

}
