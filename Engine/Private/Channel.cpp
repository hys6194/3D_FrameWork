#include "Channel.h"
#include "Bone.h"

Channel::Channel()
{
}

HRESULT Channel::Initialize(const aiNodeAnim* pAIChannel, const vector<class Bone*>& pBone)
{
	// 현재 재생중인 애니메이션의 이름 저장
	strcpy_s(m_szName, pAIChannel->mNodeName.data);

	m_iNumFrameKeys = max(pAIChannel->mNumScalingKeys, pAIChannel->mNumRotationKeys);
	m_iNumFrameKeys = max(m_iNumFrameKeys, pAIChannel->mNumPositionKeys);


	KEYFRAME	Desc = {};

	_float3     vScale = { 0.01f, 0.01f, 0.01f };
	_float4     vRotation{ 0.01f, 0.01f, 0.01f, 1.f };
	_float3     vPosition = { 0.01f, 0.01f, 0.01f };

	//1.f 0.01f

	// 현재 채널과 이름이 같은 뼈를 모델이 저장하고 있는 전체 뼈중에서 몇번째에 해당하는지 찾아낸다.
	auto iter = find_if(pBone.begin(), pBone.end(), [&](Bone* pBone)->_bool
		{
			if (true == pBone->Compare_Name(m_szName))
			{
				return m_bIsmatched = true;
			}

			else
			{

				Desc.vScale = vScale;
				Desc.vRotation = vRotation;
				Desc.vTranslation = vPosition;
				Desc.fTrackPosition = m_iBoneIndex;
				Desc.bIsAnim = false;

				m_vecFrame.push_back(Desc);
				++m_iBoneIndex;
				++m_iAnimBoneIndex;
				// 특정 프레임에 정점에게 영향을 주는 특정 뼈의 인덱스를 후의 연산을 통해 가져온다
				return m_bIsmatched = false;
			}
			
		});




	for (size_t i = 0; i < m_iNumFrameKeys; ++i)
	{
		
		
		//if(pAIChannel[i].mNumScalingKeys < m_iNum	FrameKeys)zx
		if (i < pAIChannel->mNumScalingKeys)
		{
			memcpy(&vScale, &pAIChannel->mScalingKeys[i].mValue, sizeof(_float3));
			Desc.fTrackPosition = pAIChannel->mScalingKeys[i].mTime;
		}
		
		// 아래 코드는 절대 안됨 Assimp는 _float4를 저장할 때 x,y,z,w가 아닌, w,x,y,z로 저장하고 있기 때문
		// memcpy(&vRotation, &pAIChannel->mScalingKeys[i].mValue, sizeof(_float4));
		
		if (i < pAIChannel->mNumRotationKeys)
		{
			vRotation.x = pAIChannel->mRotationKeys[i].mValue.x;
			vRotation.y = pAIChannel->mRotationKeys[i].mValue.y;
			vRotation.z = pAIChannel->mRotationKeys[i].mValue.z;
			vRotation.w = pAIChannel->mRotationKeys[i].mValue.w;
			Desc.fTrackPosition = pAIChannel->mRotationKeys[i].mTime;
		}
		
		if (i < pAIChannel->mNumRotationKeys)
		{
			memcpy(&vPosition, &pAIChannel->mPositionKeys[i].mValue, sizeof(_float3));
			Desc.fTrackPosition = pAIChannel->mPositionKeys[i].mTime;
		}
		
		Desc.vScale = vScale;
		Desc.vRotation = vRotation;
		Desc.vTranslation = vPosition;
		Desc.bIsAnim = true;
		
		++m_iBoneIndex;

		m_vecFrame.push_back(Desc);
		m_vecAnimFrame.push_back(Desc);

	}




	return S_OK;
}

void Channel::Update_TransformationMatrix(const vector<class Bone*>& pBone, _float fCurrentTrackPosition, _uint* pKeyFrameIndex)
{
	// 마지막 키프레임을 기준으로 애니메이션이 작동되는 것을 막기위해 KeyFrame의 위치를 초기화
	if (0.f == fCurrentTrackPosition)
		(*pKeyFrameIndex) = 0;

	// 이 함수에서는 특정 프레임에서의 애니메이션 보간 작업을 수행해야 한다
	// 현재 특정 프레임에 대한 정보를 이 클래스에서 꺼내서 사용하고 있다
	// Initialize에서 저장한 Scale, Rotation, Translation에 대한 정보를 통해 Matrix를 만들고,
	// 이를 뼈에 적용시켜 줄 것이다.

	// 제일 마지막에 담긴 키프레임을 가져온다
	KEYFRAME		tLastKeyFrame = m_vecAnimFrame.back();

	// 현재 재생위치에 맞는 키프레임 상태
	_vector         vScale, vRotation, vTranslation;


	// 마지막 키프레임을 넘어갔는데 애니메이션이 안끝났을 때
	// 즉, 키 프레임은 이미 마지막이지만 애니메이션이 진행되고 있을 때
	// 선형 보간 없이 마지막 키프레임의 상태를 취한다
	// 간혹가다 키프레임은 넘어갔는데 애니메이션이 계속 진행되는 모델이 있으므로 다음과 같이 처리한다
	if (fCurrentTrackPosition >= tLastKeyFrame.fTrackPosition)
	{
		vScale = XMLoadFloat3(&tLastKeyFrame.vScale);
		vRotation = XMLoadFloat4(&tLastKeyFrame.vRotation);

		// 위치벡터이므로 w값을 1로 채워 위치 벡터임을 확실히 한다
		vTranslation = XMVectorSetW(XMLoadFloat3(&tLastKeyFrame.vTranslation), 1.f);
	}
	// 선형 보간
	else
	{
		// 첫 프레임은 0번째 인덱스에서부터 시작한다
		// 인자값으로 처음 받아온 인덱스는 0에서부터 시작하고, 만약 다음 키프레임의 값보다 크거나 같다면
		// 1을 증가시켜 다음 키프레임 인덱스의 키프레임의 위치를 가져온다
		// 이를 보고도 이해가 가지 않는다면, 노션의 노트 정리를 통해 이해하는 것이 좋다
		// 
		// 나중에 프레임 드랍이 생겼을 때, fTimeDelta값이 큰 값으로 들어가게 되면서 뼈가 뒤틀리는 현상이 일어난다
		// 즉, KeyFrame값이 가르키는 값이 그 다음 프레임보다 더 큰 값으로 가르키게 되면서 일어나는 오류이다
		// 그래서 while문으로 루프를 돌려 이를 해결한다
		//if (fCurrentTrackPosition >= m_vecFrame[(*pKeyFrameIndex) + 1].fTrackPosition)
		//	++(*pKeyFrameIndex);
		while (fCurrentTrackPosition >= m_vecAnimFrame[(*pKeyFrameIndex) + 1].fTrackPosition)
			++(*pKeyFrameIndex);


		// 이전 키 프레임과 현재 프레임의 비율을 구할 것
		// 인자로 받아온 프레임 위치 - 현재 프레임위치 / 다음 프레임의 위치 - 현재 프레임 위치로
		// 인자로 받아온 프레임과 현재 프레임의 위치, 다음 프레임의 위치 비율을 구한다
		_float fRatio = (fCurrentTrackPosition - m_vecAnimFrame[(*pKeyFrameIndex)].fTrackPosition) /
			(m_vecAnimFrame[(*pKeyFrameIndex) + 1].fTrackPosition - m_vecAnimFrame[(*pKeyFrameIndex)].fTrackPosition);


		_vector vCurScale, vCurRotation, vCurTranslation;
		_vector vNextScale, vNextRotation, vNextTranslation;

		vCurScale = XMLoadFloat3(&m_vecAnimFrame[(*pKeyFrameIndex)].vScale);
		vNextScale = XMLoadFloat3(&m_vecAnimFrame[(*pKeyFrameIndex) + 1].vScale);

		vCurRotation = XMLoadFloat4(&m_vecAnimFrame[(*pKeyFrameIndex)].vRotation);
		vNextRotation = XMLoadFloat4(&m_vecAnimFrame[(*pKeyFrameIndex) + 1].vRotation);

		vCurTranslation = XMVectorSetW(XMLoadFloat3(&m_vecAnimFrame[(*pKeyFrameIndex)].vTranslation), 1.f);
		vNextTranslation = XMVectorSetW(XMLoadFloat3(&m_vecAnimFrame[(*pKeyFrameIndex) + 1].vTranslation), 1.f);

		vScale = XMVectorLerp(vCurScale, vNextScale, fRatio);
		vRotation = XMQuaternionSlerp(vCurRotation, vNextRotation, fRatio);
		vTranslation = XMVectorLerp(vCurTranslation, vNextTranslation, fRatio);

		
	}

	// XMMatrixAffineTransformation : 로컬 스페이스에서의 Scale, Rotation, Translation의 행렬을 곱하여 월드 스페이스에서의 행렬로 만들어주는 행렬 곱셈 함수
	// 인자로 들어가는 값이 무조건 로컬 스페이스에서의 데이터 값이 아님. 개발자의 편의를 위해서 만든 유틸리티 함수임 
	// 다만 Scale, Rotation, Translation을 곱하는 경우가 로컬 스페이스 상에서의 데이터임을 알아야 한다.

	//_matrix     TransformationMatrix =
	//	XMMatrixAffineTransformation(vScale, XMVectorSet(0.f, 0.f, 0.f, 1.f), vRotation, vTranslation);
	//
	//pBone[m_iBoneIndex]->Set_CombinedTransformationMatrix(
	//	TransformationMatrix);


	pBone[m_iAnimBoneIndex]->Set_TransformationMatrix(
		XMMatrixAffineTransformation(vScale, XMVectorSet(0.f, 0.f, 0.f, 1.f), vRotation, vTranslation));

}

Channel* Channel::Create(const aiNodeAnim* pAIChannel, const vector<class Bone*>& pBone)
{
	Channel* pInstance = new Channel();

	if (FAILED(pInstance->Initialize(pAIChannel, pBone)))
	{
		MSG_BOX("Failed To Created Create : Channel");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void Channel::Free()
{
	__super::Free();

	m_vecFrame.clear();
}
