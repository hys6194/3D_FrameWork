#include "Model.h"
#include "Mesh.h"

Model::Model(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
    :Component{ pDevice , pContext }
{
}

Model::Model(const Model& Prototype)
    : Component{ Prototype }
    , m_pAIScene{ Prototype.m_pAIScene }
    , m_iNumMeshes{ Prototype.m_iNumMeshes }
    , m_vecMesh{ Prototype.m_vecMesh }
{
    for (auto& pMesh : m_vecMesh)
        Safe_AddRef(pMesh);
} 

HRESULT Model::Initialize_Prototype(const _char* pFilePath)
{

    _uint iFlag = aiProcess_PreTransformVertices | aiProcess_ConvertToLeftHanded | aiProcessPreset_TargetRealtime_Fast;


    // FBX 파일로부터 읽어야할 정보들을 받아와 저장함
    m_pAIScene = m_Importer.ReadFile(pFilePath, iFlag);
    NULL_CHECK_RETURN(m_pAIScene, E_FAIL);

    // aiScene안에 담겨있는 모든 정보들을 우리가 사용하기에 좋은 형태로 변환하여 사용
    // Assimp는 행렬의 정보를 가로로 저장하고 있는 것이 아닌, 세로로 받고 있어, 이를 수정해야 함
    if (FAILED(Ready_Meshes()))
        return E_FAIL;

    return S_OK;
}

HRESULT Model::Initialize(void* pArg)
{
    return S_OK;
}

HRESULT Model::Render()
{
    // 렌더할 때 꼭 Bind_Input_Assembler 하는 것을 잊지 말자
    for (auto& iter : m_vecMesh)
    {
        iter->Bind_Input_Assembler();
        iter->Render();
    }

    return S_OK;
}

HRESULT Model::Ready_Meshes()
{
    // Assimp를 통해 읽어온 모델의 메쉬의 개수를 받아온다
    // 정점과 인덱스의 모든 정보를 받아올 수 있음
    m_iNumMeshes = m_pAIScene->mNumMeshes;


    for (size_t i = 0; i < m_iNumMeshes; ++i)
    {
        const aiMesh* pAIMesh = m_pAIScene->mMeshes[i];

        Mesh* pMesh = Mesh::Create(m_pDevice, m_pContext, pAIMesh);

        if (nullptr == pMesh)
            return E_FAIL;

        m_vecMesh.push_back(pMesh);

    }

    return S_OK;
}

Model* Model::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const _char* pFilePath)
{
    Model* pInstance = new Model(pDevice, pContext);

    if (FAILED(pInstance->Initialize_Prototype(pFilePath)))
    {
        MSG_BOX("Failed To Created : Model");
        Safe_Release(pInstance);
    }

    return pInstance;
}

Component* Model::Clone(void* pArg)
{
    Component* pInstance = new Model(m_pDevice, m_pContext);

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

    for (auto& pMesh : m_vecMesh)
        Safe_Release(pMesh);

    m_Importer.FreeScene();
}
