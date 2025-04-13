#include "Model.h"
#include "Mesh.h"
#include "Bone.h"
#include "Shader.h"
#include "Animation.h"
#include "Channel.h"
#include "MeshMaterial.h"
#include "GameInstance.h"


CModel::CModel(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
    : CComponent { pDevice, pContext }
{

}

CModel::CModel(const CModel& Prototype)
    : CComponent{ Prototype }
    , m_pAIScene { Prototype.m_pAIScene }
    , m_iNumMeshes{ Prototype.m_iNumMeshes }
    , m_vecMesh{ Prototype.m_vecMesh }
    , m_eModelType { Prototype.m_eModelType }
    , m_PreTransformMatrix { Prototype.m_PreTransformMatrix }
    , m_iNumMaterials { Prototype.m_iNumMaterials }
    , m_vecMaterial { Prototype.m_vecMaterial }
    //, m_vecBone { Prototype.m_vecBone } 굳이 복사를 할 필요가 없어짐 -> Clone하면서 vector에 담아 깊복했기 때문
    , m_iNumAnimations{ Prototype.m_iNumAnimations }
    , m_Animations{ Prototype.m_Animations }
    , m_iCurrentAnimationIndex{ Prototype.m_iCurrentAnimationIndex }
    , m_vecCurrentTrackPosition{ Prototype.m_vecCurrentTrackPosition }
    , m_vecKeyFrameIndex{ Prototype.m_vecKeyFrameIndex }
{
    for (auto& pAnimation : m_Animations)
        Safe_AddRef(pAnimation);

    for (auto& pBone : Prototype.m_vecBone)
        m_vecBone.push_back(pBone->Clone());

    for (auto& pMaterial : m_vecMaterial)
        Safe_AddRef(pMaterial);

    for (auto& pMesh : m_vecMesh)
        Safe_AddRef(pMesh);
}

const _float4x4* CModel::Get_BoneMatrix(const _char* pBoneName)
{
    auto iter = find_if(m_vecBone.begin(), m_vecBone.end(), [&](CBone* pBone)->_bool
        {
            if (true == pBone->Compare_Name(pBoneName))
                return true;

            return false;
        });

    if (iter == m_vecBone.end())
        return nullptr;

    return (*iter)->Get_CombinedTransformfloat4x4ptr();
}

void CModel::Set_AnimationIndex(_uint iAnimationIndex, _bool isLoop, _bool IsInter)
{

    m_bIsInter = IsInter;
    m_vecBone[2]->Reset_Delta();
    // 현재 재생하고 있는 애니메이션과 인자값이 같다면 함수진행을 막음
    if (m_iCurrentAnimationIndex == iAnimationIndex)    
        return;

    // 보간 안 할시 모든 애니메이션 프레임 초기화
    if(true != m_bIsInter)
    {
        for (auto& pCurrentTrackPosition : m_vecCurrentTrackPosition)
            pCurrentTrackPosition = 0;
    
        // vector의 vector인 점을 까먹으면 안된다
        for (auto& pCurrentKeyFrameIndices : m_vecKeyFrameIndex)
        {
            for (auto& pCurrentKeyFrameIndex : pCurrentKeyFrameIndices)
                pCurrentKeyFrameIndex = 0;
        }
    }
    
    // 보간 할 시 Enter_State 에서 진입할 애니메이션의 값만 초기화
    else
    {
        m_vecCurrentTrackPosition[iAnimationIndex] = 0;
    
        for (auto& pCurrentKeyFrameIndex : m_vecKeyFrameIndex[iAnimationIndex])
            pCurrentKeyFrameIndex = 0;
    }

    //for (auto& pCurrentTrackPosition : m_vecCurrentTrackPosition)
    //    pCurrentTrackPosition = 0;
    //
    //// vector의 vector인 점을 까먹으면 안된다
    //for (auto& pCurrentKeyFrameIndices : m_vecKeyFrameIndex)
    //{
    //    for (auto& pCurrentKeyFrameIndex : pCurrentKeyFrameIndices)
    //        pCurrentKeyFrameIndex = 0;
    //}

 
    // 애니메이션의 교체를 위해서 KeyFrame_Reset
    // 왜 KeyFrame_Reset을 하는가? -> 현재 진행되고 있던 애니메이션의 KeyFrame값에서 애니메이션을 재생하게 되면
    // 깨지기 때문에 0으로 초기화 한 다음, 애니메이션을 시작하기 위해서이다
    // 0으로 초기화 하면서 애니메이션의 변경 시, 보간을 하여 처리를 해야하는가?
    // 몇개는 필요하다고 생각한다. 하지만 모든 애니메이션에 상태 변환시의 보간을 해버리면
    // 반응속도가 느려져 유저들에게 불쾌함을 줄 수도 있다. 따라서 만들면서 결정을 해야할 듯 하다
    m_Animations[iAnimationIndex]->KeyFrame_Reset();

    m_iCurrentAnimationIndex = iAnimationIndex;

    m_fCurTrackPos = m_vecCurrentTrackPosition[m_iCurrentAnimationIndex];
    m_iCurKeyFrameIndex = m_vecKeyFrameIndex[m_iCurrentAnimationIndex][m_fCurTrackPos];

    m_pCurChannel = m_Animations[m_iCurrentAnimationIndex]->Get_Channel();

    m_bIsLoop = isLoop;

}

void CModel::Set_Interpolate(_bool bIsInter)
{
    m_bIsInter = bIsInter;

    m_fCurTrackPos = m_vecCurrentTrackPosition[m_iCurrentAnimationIndex];
    m_iCurKeyFrameIndex = m_vecKeyFrameIndex[m_iCurrentAnimationIndex][m_fCurTrackPos];

    m_pCurChannel = m_Animations[m_iCurrentAnimationIndex]->Get_Channel();
}

void CModel::Set_PreAnimation(_uint iPreAnimationIndex)
{
    m_iPreAnimationIndex = iPreAnimationIndex;

    m_fPreTrackPos = m_vecCurrentTrackPosition[m_iPreAnimationIndex];
    
    m_iPreKeyFrameIndex = /*m_vecKeyFrameIndex[m_iPreAnimationIndex]*/m_fPreTrackPos;

    m_pPreChannel = m_Animations[m_iPreAnimationIndex]->Get_Channel();

    for (size_t i = 0; i < m_pPreChannel.size(); i++)
    {
        m_vecBone[i]->Get_CombinedTransformationMatrix();
    }

}

void CModel::Interpolate_Animation(_float fRatio)
{
    // 0과 1사이 값만 허용
    if (1 < fRatio)
    {
        return;
    }
    
    m_fRatio += fRatio;
    if (1.f <= m_fRatio)
        m_fRatio = 1.f;
    
    // 이전 애니메이션에서 끝난 지점과 키프레임을 빼서 비율을 구한다
    //_float          fPreRatio = m_fPreTrackPos - m_iPreKeyFrameIndex;
    //
    //KEYFRAME        tPreDesc, tPreDesc1,tCurDesc;
    //
    ////tPreDesc = m_pPreChannel[m_iPreKeyFrameIndex]->Get_KeyFrame();
    ////tPreDesc1 = m_pPreChannel[m_iPreAnimationIndex]->Get_KeyFrame();
    //
    //// 이전 애니메이션의 끝난 시점에서의 채널과 다음 채널과 보간
    ////tPreDesc    =   m_pPreChannel[m_iPreKeyFrameIndex]->Get_KeyFrame()[0];
    ////tPreDesc1   =   m_pPreChannel[m_iPreKeyFrameIndex + 1]->Get_KeyFrame()[0];
    //_vector         vScale, vRotation, vTranslation;
    //
    //_vector         vSourScale, vSourRotation, vSourTranslation;
    //_vector         vDestScale, vDestRotation, vDestTranslation;
    //
    //_vector         vCurScale, vCurRotation, vCurTranslation;
    //_vector         vCurScale1, vCurRotation1, vCurTranslation1;
    //_vector         vNextScale, vNextRotation, vNextTranslation;

    //tPreDesc = m_pPreChannel[m_iPreKeyFrameIndex]->Get_KeyFrame().front();
    //tPreDesc1 = m_pPreChannel[m_iPreKeyFrameIndex]->Get_KeyFrame().back();
    //
    //vCurScale = XMLoadFloat3(&tPreDesc.vScale);
    //vCurScale1 = XMLoadFloat3(&tPreDesc1.vScale);
    //
    //vCurRotation = XMLoadFloat4(&tPreDesc.vRotation);
    //vCurRotation1 = XMLoadFloat4(&tPreDesc1.vRotation);
    //
    //vCurTranslation = XMVectorSetW(XMLoadFloat3(&tPreDesc.vTranslation), 1.f);
    //vCurTranslation1 = XMVectorSetW(XMLoadFloat3(&tPreDesc1.vTranslation), 1.f);
    //
    //// 보간 비율로 
    //vSourScale = XMVectorLerp(vCurScale, vCurScale1, fPreRatio);
    //vSourRotation = XMQuaternionSlerp(vCurRotation, vCurRotation1, fPreRatio);
    //vSourTranslation = XMVectorLerp(vCurTranslation, vCurTranslation1, fPreRatio);
    //
    //for (size_t i = 0; i < m_pCurChannel.size(); i++)
    //{
    //    /*tPreDesc = m_pPreChannel[i]->Get_KeyFrame().back();
    //    tPreDesc1 = m_pPreChannel[i]->Get_KeyFrame().front();
    //
    //    vCurScale = XMLoadFloat3(&tPreDesc.vScale);
    //    vCurScale1 = XMLoadFloat3(&tPreDesc1.vScale);
    //
    //    vCurRotation = XMLoadFloat4(&tPreDesc.vRotation);
    //    vCurRotation1 = XMLoadFloat4(&tPreDesc1.vRotation);
    //
    //    vCurTranslation = XMVectorSetW(XMLoadFloat3(&tPreDesc.vTranslation), 1.f);
    //    vCurTranslation1 = XMVectorSetW(XMLoadFloat3(&tPreDesc1.vTranslation), 1.f);
    //
    //    vSourScale = XMVectorLerp(vCurScale, vCurScale1, fPreRatio);
    //    vSourRotation = XMQuaternionSlerp(vCurRotation, vCurRotation1, fPreRatio);
    //    vSourTranslation = XMVectorLerp(vCurTranslation, vCurTranslation1, fPreRatio);*/
    //
    //    //tPreDesc = m_pPreChannel[m_iPreKeyFrameIndex]->Get_KeyFrame().back();
    //    //tPreDesc1 = m_pPreChannel[m_iPreKeyFrameIndex + 1]->Get_KeyFrame().front();
    //
    //    tCurDesc = m_pCurChannel[i]->Get_KeyFrame()[0];
    //
    //    vNextScale = XMLoadFloat3(&tCurDesc.vScale);
    //
    //    vNextRotation = XMLoadFloat4(&tCurDesc.vRotation);
    //
    //    vNextTranslation = XMVectorSetW(XMLoadFloat3(&tCurDesc.vTranslation), 1.f);
    //
    //    vDestScale = XMVectorLerp(vSourScale, vNextScale, m_fRatio);
    //    vDestRotation = XMQuaternionSlerp(vSourRotation, vNextRotation, m_fRatio);
    //    vDestTranslation = XMVectorLerp(vSourTranslation, vNextTranslation, m_fRatio);
    //
    //    //vSourScale = vNextScale;
    //    //vSourRotation = vNextRotation;
    //    //vSourTranslation = vNextTranslation;
    //
    //    m_vecBone[tCurDesc.iBoneIndex]->Set_TransformationMatrix(
    //        XMMatrixAffineTransformation(vDestScale, XMVectorSet(0.f, 0.f, 0.f, 1.f), vDestRotation, vDestTranslation));
    //
    //}

    

    // m_fRatio 값이 1을 넘어가지 않게 해야함


    //vCurScale = XMLoadFloat3(&tPreDesc.vScale);
    //vNextScale = XMLoadFloat3(&tCurDesc.vScale);
    //
    //vCurRotation = XMLoadFloat4(&tPreDesc.vRotation);
    //vNextRotation = XMLoadFloat4(&tCurDesc.vRotation);
    //
    //vCurTranslation = XMVectorSetW(XMLoadFloat3(&tPreDesc.vTranslation), 1.f);
    //vNextTranslation = XMVectorSetW(XMLoadFloat3(&tCurDesc.vTranslation), 1.f);
    //
    //vScale = XMVectorLerp(vCurScale, vNextScale, fPreRatio);
    //vRotation = XMQuaternionSlerp(vCurRotation, vNextRotation, fPreRatio);
    //vTranslation = XMVectorLerp(vCurTranslation, vNextTranslation, fPreRatio);

    for (size_t i = 0; i < m_pCurChannel.size(); i++)
    {

        _float          fPreRatio = m_fPreTrackPos - m_iPreKeyFrameIndex;

        KEYFRAME        tPreDesc, /*tPreDesc1, */tCurDesc;

        _vector         vScale, vRotation, vTranslation;

        //_vector         vSourScale, vSourRotation, vSourTranslation;
        //_vector         vDestScale, vDestRotation, vDestTranslation;

        _vector         vCurScale, vCurRotation, vCurTranslation;
        //_vector         vCurScale1, vCurRotation1, vCurTranslation1;
        _vector         vNextScale, vNextRotation, vNextTranslation;

        tPreDesc = m_pPreChannel[i]->Get_KeyFrame().back();
        tCurDesc = m_pCurChannel[i]->Get_KeyFrame()[0];
    
        //_vector         vScale, vRotation, vTranslation;
        //
        //_vector vCurScale, vCurRotation, vCurTranslation;
        //_vector vNextScale, vNextRotation, vNextTranslation;
        //
        vCurScale = XMLoadFloat3(&tPreDesc.vScale);
        vNextScale = XMLoadFloat3(&tCurDesc.vScale);
        //
        vCurRotation = XMLoadFloat4(&tPreDesc.vRotation);
        vNextRotation = XMLoadFloat4(&tCurDesc.vRotation);
        //
        vCurTranslation = XMVectorSetW(XMLoadFloat3(&tPreDesc.vTranslation), 1.f);
        vNextTranslation = XMVectorSetW(XMLoadFloat3(&tCurDesc.vTranslation), 1.f);
        //
        vScale = XMVectorLerp(vCurScale, vNextScale, m_fRatio);
        vRotation = XMQuaternionSlerp(vCurRotation, vNextRotation, m_fRatio);
        vTranslation = XMVectorLerp(vCurTranslation, vNextTranslation, m_fRatio);
    
        m_vecBone[tPreDesc.iBoneIndex]->Set_TransformationMatrix(
            XMMatrixAffineTransformation(vScale, XMVectorSet(0.f, 0.f, 0.f, 1.f), vRotation, vTranslation));
    
    }

    for (auto& pBone : m_vecBone)
    {
        pBone->Update_CombinedTransformationMatrix(m_vecBone, &m_PreTransformMatrix);
    }

    if (1 <= m_fRatio)
    {
        m_fRatio = 0.f;
        m_bIsInter = false;
        m_iPreAnimationIndex = 0;
    }
 
    //m_bIsInter = false;

}

HRESULT CModel::Initialize_Prototype(MODELTYPE eType, const _char* pModelFilePath, _fmatrix PreTransformMatrix)
{
    m_eModelType = eType;
    
    XMStoreFloat4x4(&m_PreTransformMatrix, PreTransformMatrix);

    //aiProcess_CalcTangentSpace, aiProcessPreset_TargetRealtime_Fast
    _uint        iFlag = aiProcess_ConvertToLeftHanded | aiProcess_CalcTangentSpace;

    if (MODELTYPE::TYPE_NONANIM == eType)
        iFlag |= aiProcess_PreTransformVertices;

    /* fbx파일로부터 읽어야할 모든 정보를 다 메모리할당(aiScene)하여 저장받아온 것이다.  */
    m_pAIScene = m_Importer.ReadFile(pModelFilePath, iFlag);
    if (nullptr == m_pAIScene)
        return E_FAIL;

    /* 모델의 기본정보는 다 로드를 했다. */
    /* aiScene안에 포함되어있기 떄문에. 우리가 사용하기좋은 형태로 변형, 분리, 보관해주는 작업을 수행해야하낟. */
    
    if (FAILED(Ready_Bones(m_pAIScene->mRootNode, -1)))
        return E_FAIL;

    /* 1.모델하나에 포함되어있는 메시들을 로드하는 작업을 수행할께 .*/
    if (FAILED(Ready_Meshes()))
        return E_FAIL;

    if (FAILED(Ready_Materials(pModelFilePath)))
        return E_FAIL;

    if (FAILED(Ready_Animations()))
        return E_FAIL;

    

    return S_OK;
}

HRESULT CModel::Initialize(void* pArg)
{
    return S_OK;
}

HRESULT CModel::Render(_uint iMeshIndex)
{
    m_vecMesh[iMeshIndex]->Bind_Input_Assembler();
    m_vecMesh[iMeshIndex]->Render();   

    return S_OK;
}

_bool CModel::Play_Animation(_float fTimeDelta, CGameObject* pObject)
{
    /* 특정 애니메이션을 구동한다. */
    /* 애니메이션을 구동한다 == +		[4]	{vScale={x=1.00000060 y=1.00000012 z=1.00000036 } vRotation={x=-0.0756162405 y=-0.0891902819 z=-0.0955794305 ...} ...}	Engine::tagKeyFrame

    이 애니메이션 구동을 위해 움직여야할 뼈(Channel) 들의 현재 재생위치에 맞는 키프레임상태에 따른 TransformationMatrix를 갱신한다.*/
    /* 뼈들의 Transformationmatrix를 갱신했다고 해서 바뀐상태로 그릴 수 있는건 아니다. */
    /* 뼈들의 CombinedTransformationMatrix가 갱신되어있어야 애니메이션 재생되는 표현을 해줄 수 있다. */

    /* 뼈들의 최종 CombinedTransformationMatrix를 갱신한다. */
    _bool bIsEnd;
    

    // 모든 애니메이션에 루트애님을 적용 시켜야하는지 필요성을 못느껴서 분리해서 적용함
    // 루트 애님이 많은 모델이라면 그것이 맞지만 그게 아니라서
    // 기존 애니메이션
    if(nullptr == pObject)
    {
        bIsEnd = m_Animations[m_iCurrentAnimationIndex]->Update_TransformationMatrix(m_vecBone, fTimeDelta, m_bIsLoop, &m_vecCurrentTrackPosition[m_iCurrentAnimationIndex], m_vecKeyFrameIndex[m_iCurrentAnimationIndex]);
        for (auto& pBone : m_vecBone)
        {
            pBone->Update_CombinedTransformationMatrix(m_vecBone, &m_PreTransformMatrix);
        }

        //_float4 f42;
        //memcpy(&f42, &m_vecBone[2]->Get_CombinedTransformfloat4x4ptr()->m[3][0], sizeof(_float4));
        //
        //int a = 10;
    }

    // 루트 애니메이션의 이동량 제거
    else
    {
        bIsEnd = m_Animations[m_iCurrentAnimationIndex]->Update_TransformationMatrix(m_vecBone, fTimeDelta, m_bIsLoop, &m_vecCurrentTrackPosition[m_iCurrentAnimationIndex], m_vecKeyFrameIndex[m_iCurrentAnimationIndex], pObject);
        for (auto& pBone : m_vecBone)
        {
            pBone->Update_Combine_RootMatrix(m_vecBone, &m_PreTransformMatrix, pObject);
        }
    }

    return bIsEnd;
}

void CModel::Reset_PreAnimation()
{
    m_vecCurrentTrackPosition[m_iPreAnimationIndex] = 0;

    for (auto& pCurrentKeyFrameIndex : m_vecKeyFrameIndex[m_iPreAnimationIndex])
        pCurrentKeyFrameIndex = 0;
}

HRESULT CModel::Bind_Material(CShader* pShader, const _char* pConstantName, aiTextureType eMaterialType, _uint iMeshIndex, _uint iTextureIndex)
{
    _uint       iMaterialIndex = m_vecMesh[iMeshIndex]->Get_MaterialIndex();

    return m_vecMaterial[iMaterialIndex]->Bind_SR(pShader, pConstantName, eMaterialType, iTextureIndex);
}

HRESULT CModel::Bind_BoneMatrix(CShader* pShader, const _char* pConstantName, _uint iMeshIndex)
{
    m_vecMesh[iMeshIndex]->Bind_BoneMatrix(pShader, pConstantName, m_vecBone);

    return S_OK;
}

_bool CModel::CheckRayColl_Mesh(_vector vPos, _vector vDir, _float* _fDistance, _float4* _fCoord, _vector vScale, _vector vRotation, _vector vTranslation)
{
    _bool bResult = false;
    
    for (size_t i = 0; i < m_vecMesh.size(); i++)
    {
        bResult = m_vecMesh[i]->Check_Coll_Meshes(vPos, vDir, vScale, vRotation, vTranslation);
    }

    return bResult;
}

_bool CModel::DotPoint_InMesh(_vector vPos, _vector vDir, _float* _fDistance, _float4* _fCoord, _vector vScale, _vector vRotation, _vector vTranslation)
{
    _bool bResult = false;

    for (size_t i = 0; i < m_vecMesh.size(); i++)
    {
        bResult = m_vecMesh[i]->Search_Picked_Face(vPos, vDir, _fDistance, _fCoord, vScale, vRotation, vTranslation);
    }

    return bResult;
}

HRESULT CModel::Ready_Bones(const aiNode* pAINode, _int iParentBoneIndex)
{
    CBone* pBone = CBone::Create(pAINode, iParentBoneIndex);
    if (nullptr == pBone)
        return E_FAIL;

    m_vecBone.push_back(pBone);

    _uint       iNumBones = m_vecBone.size();

    /* 이 뼈의 자식뼈의 갯수 */
    for (size_t i = 0; i < pAINode->mNumChildren; i++)
    {
        Ready_Bones(pAINode->mChildren[i], iNumBones - 1);
    };

    return S_OK;
}

HRESULT CModel::Ready_Meshes()
{
    m_iNumMeshes = m_pAIScene->mNumMeshes;

    for (size_t i = 0; i < m_iNumMeshes; i++)
    {
        const aiMesh*      pAIMesh = m_pAIScene->mMeshes[i];

        CMesh* pMesh = CMesh::Create(m_pDevice, m_pContext, pAIMesh, m_eModelType, m_vecBone, XMLoadFloat4x4(&m_PreTransformMatrix));
        if (nullptr == pMesh)
            return E_FAIL;

        m_vecMesh.push_back(pMesh);
    }
    

    return S_OK;
}

HRESULT CModel::Ready_Materials(const _char* pModelFilePath)
{
    m_iNumMaterials = m_pAIScene->mNumMaterials;

    for (size_t i = 0; i < m_iNumMaterials; i++)
    {
        CMeshMaterial* pMeshMaterial = CMeshMaterial::Create(m_pDevice, m_pContext,
            m_pAIScene->mMaterials[i], pModelFilePath);

        m_vecMaterial.push_back(pMeshMaterial);        
    } 

    return S_OK;
}

HRESULT CModel::Ready_Animations()
{
    m_iNumAnimations = m_pAIScene->mNumAnimations;

    // 왜 애니메이션의 개수만큼 resize 하는지?
    // 애니메이션의 개수만큼 공간을 할당하고 저장할 공간을 마련한 후, 기본값으로 초기화하기 위해서
    // 흐름은 노션에 정리해뒀음 
    m_vecCurrentTrackPosition.resize(m_iNumAnimations);
    m_vecKeyFrameIndex.resize(m_iNumAnimations);


    for (size_t i = 0; i < m_iNumAnimations; i++)
    {
        //m_vecKeyFrameIndex[i] = i 번째 애니메이션에 해당되는 채널의 인덱스 배열들
        CAnimation* pAnimation = CAnimation::Create(m_pAIScene->mAnimations[i], m_vecBone, m_vecKeyFrameIndex[i]);
        if (nullptr == pAnimation)
            return E_FAIL;

        m_Animations.push_back(pAnimation);
    }

    return S_OK;
}

CModel* CModel::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, MODELTYPE eType, const _char* pModelFilePath, _fmatrix PreTransformMatrix)
{
    CModel* pInstance = new CModel(pDevice, pContext);

    if (FAILED(pInstance->Initialize_Prototype(eType, pModelFilePath, PreTransformMatrix)))
    {
        MSG_BOX("Failed To Created : Model");
        Safe_Release(pInstance);
    }

    return pInstance;
}

CComponent* CModel::Clone(void* pArg)
{
    CModel* pInstance = new CModel(*this);

    if (FAILED(pInstance->Initialize(pArg)))
    {
        MSG_BOX("Failed To Cloned : Model");
        Safe_Release(pInstance);
    }

    return pInstance;
}


#pragma region 창홍이형의 바이너리

HRESULT CModel::Initialize_Prototype(MODELTYPE _eType, const _char* _pModelFilePath, const _char* _pBinaryFilePath, _fmatrix _PreTransformMatrix)
{
    m_eModelType = _eType;

    ofstream OutStream(_pBinaryFilePath, ios::binary | ios::out);

    if (OutStream.is_open() == false)
        return E_FAIL;

    OutStream.write(reinterpret_cast<const char*>(&m_eModelType), sizeof(MODELTYPE));

    XMStoreFloat4x4(&m_PreTransformMatrix, _PreTransformMatrix);

    _uint iFlag = aiProcess_ConvertToLeftHanded | aiProcess_CalcTangentSpace/* | aiProcessPreset_TargetRealtime_Fast*/;

    if (_eType == MODELTYPE::TYPE_NONANIM)
        iFlag |= aiProcess_PreTransformVertices;

    m_pAIScene = m_Importer.ReadFile(_pModelFilePath, iFlag);
    if (m_pAIScene == nullptr)
        return E_FAIL;

    if (FAILED(Ready_Bones(m_pAIScene->mRootNode, -1)))
        return E_FAIL;

    _uint iWriteByte = m_vecBone.size();
    OutStream.write(reinterpret_cast<const char*>(&iWriteByte), sizeof(_uint));
    for (auto& pBone : m_vecBone)
        pBone->Save_Bone(OutStream);

    if (FAILED(Ready_Meshes_Save(OutStream)))
        return E_FAIL;

    if (FAILED(Ready_Materials_Save(_pModelFilePath, OutStream)))
        return E_FAIL;

    if (FAILED(Ready_Animations()))
        return E_FAIL;

    OutStream.write(reinterpret_cast<const char*>(&m_iNumAnimations), sizeof(_uint));
    for (auto& pAnimation : m_Animations)
        pAnimation->Save_Animation(OutStream);

    OutStream.close();


    return S_OK;
}

HRESULT CModel::Initialize_Prototype(const _char* _pBinaryFilePath, _fmatrix _PreTransformMatrix)
{
    ifstream InStream(_pBinaryFilePath, ios::binary);

    if (InStream.is_open() == false)
        return E_FAIL;

    InStream.read(reinterpret_cast<char*>(&m_eModelType), sizeof(MODELTYPE));

    XMStoreFloat4x4(&m_PreTransformMatrix, _PreTransformMatrix);

    _uint iWriteByte = m_vecBone.size();
    InStream.read(reinterpret_cast<char*>(&iWriteByte), sizeof(_uint));
    Ready_Bones_Load(iWriteByte, InStream);

    if (FAILED(Ready_Meshes_Load(InStream)))
        return E_FAIL;

    if (FAILED(Ready_Materials_Load(InStream)))
        return E_FAIL; 

    if (FAILED(Ready_Animation_Load(InStream)))
        return E_FAIL;

    InStream.read(reinterpret_cast<char*>(&m_iNumAnimations), sizeof(_uint));
    Ready_Animation_Load(InStream);

    InStream.close();


    return S_OK;
}

HRESULT CModel::Ready_Bones_Load(_uint _iNumBones, ifstream& _InStream)
{
    for (size_t i = 0; i < _iNumBones; i++)
    {
        CBone* pBone = CBone::Create(_InStream);
        if (nullptr == pBone)
            return E_FAIL;

        m_vecBone.push_back(pBone);

    }

    return S_OK;
}

HRESULT CModel::Ready_Meshes_Save(ofstream& _OutStream)
{
    m_iNumMeshes = m_pAIScene->mNumMeshes;

    _uint iWriteByte = m_iNumMeshes;
    _OutStream.write(reinterpret_cast<const char*>(&iWriteByte), sizeof(_uint));

    for (size_t i = 0; i < m_iNumMeshes; i++)
    {
        const aiMesh* pAIMesh = m_pAIScene->mMeshes[i];

        CMesh* pMesh = CMesh::Create(m_pDevice, m_pContext, pAIMesh, m_eModelType, m_vecBone, XMLoadFloat4x4(&m_PreTransformMatrix), _OutStream);
        if (pMesh == nullptr)
            return E_FAIL;

        m_vecMesh.push_back(pMesh);
    }

    return S_OK;
}

HRESULT CModel::Ready_Materials_Save(const _char* _pModelFilePath, ofstream& _OutStream)
{
    m_iNumMaterials = m_pAIScene->mNumMaterials;

    _uint iWriteByte = m_iNumMaterials;
    _OutStream.write(reinterpret_cast<const char*>(&iWriteByte), sizeof(_uint));

    for (size_t i = 0; i < m_iNumMaterials; i++)
    {
        CMeshMaterial* pMaterial = CMeshMaterial::Create(m_pDevice, m_pContext, m_pAIScene->mMaterials[i], _pModelFilePath, _OutStream);
        if (pMaterial == nullptr)
            return E_FAIL;

        m_vecMaterial.push_back(pMaterial);
    }

    return S_OK;
}

HRESULT CModel::Ready_Meshes_Load(ifstream& _InStream)
{
    _InStream.read(reinterpret_cast<char*>(&m_iNumMeshes), sizeof(_uint));

    for (size_t j = 0; j < m_iNumMeshes; j++)
    {
        CMesh* pMesh = CMesh::Create(m_pDevice, m_pContext, m_eModelType, m_vecBone, XMLoadFloat4x4(&m_PreTransformMatrix), _InStream);
        if (pMesh == nullptr)
            return E_FAIL;

        m_vecMesh.push_back(pMesh);
    }

    return S_OK;
}

HRESULT CModel::Ready_Materials_Load(ifstream& _InStream)
{
    _InStream.read(reinterpret_cast<char*>(&m_iNumMaterials), sizeof(_uint));

    for (size_t j = 0; j < m_iNumMaterials; j++)
    {
        CMeshMaterial* pMaterial = CMeshMaterial::Create(m_pDevice, m_pContext, _InStream);
        if (pMaterial == nullptr)
            return E_FAIL;

        m_vecMaterial.push_back(pMaterial);
    }

    return S_OK;
}

HRESULT CModel::Ready_Animation_Load(ifstream& _InStream)
{
    m_vecCurrentTrackPosition.resize(m_iNumAnimations);
    m_vecKeyFrameIndex.resize(m_iNumAnimations);

    for (size_t i = 0; i < m_iNumAnimations; i++)
    {
        CAnimation* pAnimation = CAnimation::Create(m_vecBone, _InStream, m_vecKeyFrameIndex[i]);
        if (pAnimation == nullptr)
            return E_FAIL;

        m_Animations.push_back(pAnimation);
    }

    return S_OK;
}

CModel* CModel::Create(ID3D11Device* _pDevice, ID3D11DeviceContext* _pContext, MODELTYPE _eType, const _char* _pModelFilePath, const _char* _pBinaryFilePath, _fmatrix _PreTransformMatrix)
{
    CModel* pInstance = new CModel(_pDevice, _pContext);

    if (FAILED(pInstance->Initialize_Prototype(_eType, _pModelFilePath, _pBinaryFilePath, _PreTransformMatrix)))
    {
        MSG_BOX("Failed To Created : CModel");
        Safe_Release(pInstance);
    }

    return pInstance;
}

CModel* CModel::Create(ID3D11Device* _pDevice, ID3D11DeviceContext* _pContext, const _char* _pBinaryFilePath, _fmatrix _PreTransformMatrix)
{
    CModel* pInstance = new CModel(_pDevice, _pContext);

    if (FAILED(pInstance->Initialize_Prototype(_pBinaryFilePath, _PreTransformMatrix)))
    {
        MSG_BOX("Failed To Created : CModel");
        Safe_Release(pInstance);
    }

    return pInstance;
}


#pragma endregion


void CModel::Free()
{
    __super::Free();

    for (auto& pAnimation : m_Animations)
        Safe_Release(pAnimation);

    m_Animations.clear();

    for (auto& pBone : m_vecBone)
        Safe_Release(pBone);

    m_vecBone.clear();

    for (auto& pMaterial : m_vecMaterial)
        Safe_Release(pMaterial);

    m_vecMaterial.clear();

    for (auto& pMesh : m_vecMesh)
        Safe_Release(pMesh);

    m_vecMesh.clear();

    m_Importer.FreeScene();
}
