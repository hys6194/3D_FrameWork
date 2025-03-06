#include "Model.h"
#include "Mesh.h"
#include "Bone.h"
#include "Shader.h"
#include "Animation.h"
#include "MeshMaterial.h"


Model::Model(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
    : Component { pDevice, pContext }
{

}

Model::Model(const Model& Prototype)
    : Component{ Prototype }
    , m_pAIScene { Prototype.m_pAIScene }
    , m_iNumMeshes{ Prototype.m_iNumMeshes }
    , m_vecMesh{ Prototype.m_vecMesh }
    , m_eModelType { Prototype.m_eModelType }
    , m_PreTransformMatrix { Prototype.m_PreTransformMatrix }
    , m_iNumMaterials { Prototype.m_iNumMaterials }
    , m_vecMaterial { Prototype.m_vecMaterial }
    //, m_vecBone { Prototype.m_vecBone } 굳이 복사를 할 필요가 없어짐 -> Clone으로 깊복했기 때문
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

const _float4x4* Model::Get_BoneMatrix(const _char* pBoneName)
{
    auto iter = find_if(m_vecBone.begin(), m_vecBone.end(), [&](Bone* pBone)->_bool
        {
            if (true == pBone->Compare_Name(pBoneName))
                return true;

            return false;
        });

    if (iter == m_vecBone.end())
        return nullptr;

    return (*iter)->Get_CombinedTransformfloat4x4ptr();
}

void Model::Set_AnimationIndex(_uint iAnimationIndex, _bool isLoop, _bool IsInter)
{

    // 현재 재생하고 있는 애니메이션과 인자값이 같다면 함수진행을 막음
    if (m_iCurrentAnimationIndex == iAnimationIndex)    
        return;

    // 보간 안 할시 모든 애니메이션 프레임 초기화
    if(true != IsInter)
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

    // 보간 할 시 현재 애니메이션의 값만 초기화
    else
    {
        m_vecCurrentTrackPosition[iAnimationIndex] = 0;

        for (auto& pCurrentKeyFrameIndex : m_vecKeyFrameIndex[iAnimationIndex])
            pCurrentKeyFrameIndex = 0;
    }

 
    // 애니메이션의 교체를 위해서 KeyFrame_Reset
    // 왜 KeyFrame_Reset을 하는가? -> 현재 진행되고 있던 애니메이션의 KeyFrame값에서 애니메이션을 재생하게 되면
    // 깨지기 때문에 0으로 초기화 한 다음, 애니메이션을 시작하기 위해서이다
    // 0으로 초기화 하면서 애니메이션의 변경 시, 보간을 하여 처리를 해야하는가?
    // 몇개는 필요하다고 생각한다. 하지만 모든 애니메이션에 상태 변환시의 보간을 해버리면
    // 반응속도가 느려져 유저들에게 불쾌함을 줄 수도 있다. 따라서 만들면서 결정을 해야할 듯 하다
    m_Animations[iAnimationIndex]->KeyFrame_Reset();

    m_iCurrentAnimationIndex = iAnimationIndex;
    m_bIsLoop = isLoop;
}

HRESULT Model::Initialize_Prototype(MODELTYPE eType, const _char* pModelFilePath, _fmatrix PreTransformMatrix)
{
    m_eModelType = eType;
    
    XMStoreFloat4x4(&m_PreTransformMatrix, PreTransformMatrix);

    //aiProcess_CalcTangentSpace, aiProcessPreset_TargetRealtime_Fast
    _uint        iFlag = aiProcess_ConvertToLeftHanded | aiProcessPreset_TargetRealtime_Fast;

    if (MODELTYPE::TYPE_NONANIM == eType)
        iFlag |= aiProcess_PreTransformVertices;

    /* fbx파일로부터 읽어야할 모든 정보를 다 메모리할당(aiScene)하여 저장받아온 것이다.  */
    m_pAIScene = m_Importer.ReadFile(pModelFilePath, iFlag);
    if (nullptr == m_pAIScene)
        return E_FAIL;

    // 여기에서 fbx 파일의 정보를 읽어와야 함

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

HRESULT Model::Initialize(void* pArg)
{
    return S_OK;
}

HRESULT Model::Render(_uint iMeshIndex)
{
    m_vecMesh[iMeshIndex]->Bind_Input_Assembler();
    m_vecMesh[iMeshIndex]->Render();   

    return S_OK;
}

_bool Model::Play_Animation(_float fTimeDelta)
{
    /* 특정 애니메이션을 구동한다. */
    /* 애니메이션을 구동한다 == 
    이 애니메이션 구동을 위해 움직여야할 뼈(Channel) 들의 현재 재생위치에 맞는 키프레임상태에 따른 TransformationMatrix를 갱신한다.*/
    /* 뼈들의 Transformationmatrix를 갱신했다고 해서 바뀐상태로 그릴 수 있는건 아니다. */
    /* 뼈들의 CombinedTransformationMatrix가 갱신되어있어야 애니메이션 재생되는 표현을 해줄 수 있다. */

    /* 뼈들의 최종 CombinedTransformationMatrix를 갱신한다. */
    
    m_vecKeyFrameIndex

    _bool bIsEnd = m_Animations[m_iCurrentAnimationIndex]->
                   Update_TransformationMatrix(
                   m_vecBone, 
                   fTimeDelta, 
                   m_bIsLoop, 
                   &m_vecCurrentTrackPosition[m_iCurrentAnimationIndex], 
                   m_vecKeyFrameIndex[m_iCurrentAnimationIndex],
                   &m_vecCurrentTrackPosition[m_iPreAnimationIndex],
                   &m_vecKeyFrameIndex[m_iPreAnimationIndex]);
    
   
    for (auto& pBone : m_vecBone)
    {
        pBone->Update_CombinedTransformationMatrix(m_vecBone, &m_PreTransformMatrix);
    }


    return bIsEnd;
}

HRESULT Model::Bind_Material(Shader* pShader, const _char* pConstantName, aiTextureType eMaterialType, _uint iMeshIndex, _uint iTextureIndex)
{
    _uint       iMaterialIndex = m_vecMesh[iMeshIndex]->Get_MaterialIndex();

    return m_vecMaterial[iMaterialIndex]->Bind_SR(pShader, pConstantName, eMaterialType, iTextureIndex);
}

HRESULT Model::Bind_BoneMatrix(Shader* pShader, const _char* pConstantName, _uint iMeshIndex)
{
    m_vecMesh[iMeshIndex]->Bind_BoneMatrix(pShader, pConstantName, m_vecBone);

    return S_OK;
}

HRESULT Model::Ready_Bones(const aiNode* pAINode, _int iParentBoneIndex)
{
    Bone* pBone = Bone::Create(pAINode, iParentBoneIndex);
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

HRESULT Model::Ready_Meshes()
{
    m_iNumMeshes = m_pAIScene->mNumMeshes;

    for (size_t i = 0; i < m_iNumMeshes; i++)
    {
        const aiMesh*      pAIMesh = m_pAIScene->mMeshes[i];

        Mesh* pMesh = Mesh::Create(m_pDevice, m_pContext, pAIMesh, m_eModelType, m_vecBone, XMLoadFloat4x4(&m_PreTransformMatrix));
        if (nullptr == pMesh)
            return E_FAIL;

        m_vecMesh.push_back(pMesh);
    }
    

    return S_OK;
}

HRESULT Model::Ready_Materials(const _char* pModelFilePath)
{
    m_iNumMaterials = m_pAIScene->mNumMaterials;

    for (size_t i = 0; i < m_iNumMaterials; i++)
    {
        MeshMaterial* pMeshMaterial = MeshMaterial::Create(m_pDevice, m_pContext,
            m_pAIScene->mMaterials[i], pModelFilePath);

        m_vecMaterial.push_back(pMeshMaterial);        
    } 

    return S_OK;
}

HRESULT Model::Ready_Animations()
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
        Animation* pAnimation = Animation::Create(m_pAIScene->mAnimations[i], m_vecBone, m_vecKeyFrameIndex[i]);
        if (nullptr == pAnimation)
            return E_FAIL;

        m_Animations.push_back(pAnimation);
    }

    return S_OK;
}

Model* Model::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, MODELTYPE eType, const _char* pModelFilePath, _fmatrix PreTransformMatrix)
{
    Model* pInstance = new Model(pDevice, pContext);

    if (FAILED(pInstance->Initialize_Prototype(eType, pModelFilePath, PreTransformMatrix)))
    {
        MSG_BOX("Failed To Created : Model");
        Safe_Release(pInstance);
    }

    return pInstance;
}

Component* Model::Clone(void* pArg)
{
    Model* pInstance = new Model(*this);

    if (FAILED(pInstance->Initialize(pArg)))
    {
        MSG_BOX("Failed To Cloned : Model");
        Safe_Release(pInstance);
    }

    return pInstance;
}


void Model::Free()
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
