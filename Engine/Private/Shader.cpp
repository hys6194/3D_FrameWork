#include "Shader.h"

Shader::Shader(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
    : Component{ pDevice, pContext }
{

}

Shader::Shader(const Shader& Prototype)
    : Component{ Prototype }
    , m_pEffect{ Prototype.m_pEffect }
    , m_iNumPasses{ Prototype.m_iNumPasses }
    , m_InputLayouts{ Prototype.m_InputLayouts }
{
    Safe_AddRef(m_pEffect);

    for (auto& pInputLayout : m_InputLayouts)
        Safe_AddRef(pInputLayout);

}

HRESULT Shader::Initialize_Prototype(const _tchar* pShaderFilePath, const D3D11_INPUT_ELEMENT_DESC* pElements, _uint iNumElements)
{
    _uint       iHlslFlag = {};


#ifdef _DEBUG
    iHlslFlag = D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION;
#else
    iHlslFlag = D3DCOMPILE_OPTIMIZATION_LEVEL1;
#endif
    /* 인자로 전달해준 셰이더파일을 빌드하고 객체화한다. */
    if (FAILED(D3DX11CompileEffectFromFile(pShaderFilePath, nullptr, D3D_COMPILE_STANDARD_FILE_INCLUDE, iHlslFlag, 0, m_pDevice
        , &m_pEffect, nullptr)))
        return E_FAIL;

    ID3DX11EffectTechnique* pTechnique = m_pEffect->GetTechniqueByIndex(0);
    if (nullptr == pTechnique)
        return E_FAIL;

    // 테크니커 가져오기
    D3DX11_TECHNIQUE_DESC           TechniqueDesc{};
    pTechnique->GetDesc(&TechniqueDesc);

    // hlsl에서 만든 테크니커의 pass들을 가져오기
    m_iNumPasses = TechniqueDesc.Passes;

    m_InputLayouts.reserve(m_iNumPasses);

    // 패스들을 순회하면서 pass에 맞는 셰이더 기법처리
    for (size_t i = 0; i < m_iNumPasses; i++)
    {
        ID3D11InputLayout* pInputLayout = { nullptr };

        ID3DX11EffectPass* pPass = pTechnique->GetPassByIndex(i);

        if (nullptr == pPass)
            return E_FAIL;

        D3DX11_PASS_DESC        PassDesc{};

        pPass->GetDesc(&PassDesc);

        if (FAILED(m_pDevice->CreateInputLayout(pElements, iNumElements, PassDesc.pIAInputSignature, PassDesc.IAInputSignatureSize, &pInputLayout)))
            return E_FAIL;

        m_InputLayouts.push_back(pInputLayout);
    }

    return S_OK;
}

HRESULT Shader::Initialize(void* pArg)
{
    return S_OK;
}

HRESULT Shader::Begin(_uint iPassIndex)
{

    if (iPassIndex >= m_iNumPasses)
        return E_FAIL;

    m_pContext->IASetInputLayout(m_InputLayouts[iPassIndex]);

    // hlsl에서 만든 테크니커의 몇 번째 테크니커의 무슨 패스를 불러올지 선언
    ID3DX11EffectPass* pPass = m_pEffect->GetTechniqueByIndex(0)->GetPassByIndex(iPassIndex);
    if (nullptr == pPass)
        return E_FAIL;

    pPass->Apply(iPassIndex, m_pContext);

    return S_OK;
}

HRESULT Shader::Bind_RawValue(const _char* pConstantName, const void* pData, _uint iLength)
{
    ID3DX11EffectVariable* pVariable = m_pEffect->GetVariableByName(pConstantName);
    if (nullptr == pVariable)
        return E_FAIL;

    return pVariable->SetRawValue(pData, 0, iLength);
}

HRESULT Shader::Bind_Matrix(const _char* pConstantName, const _float4x4* pMatrix)
{
    ID3DX11EffectVariable* pVariable = m_pEffect->GetVariableByName(pConstantName);
    if (nullptr == pVariable)
        return E_FAIL;

    ID3DX11EffectMatrixVariable* pMatrixVariable = pVariable->AsMatrix();
    if (nullptr == pMatrixVariable)
        return E_FAIL;

    return pMatrixVariable->SetMatrix(reinterpret_cast<const _float*>(pMatrix));
}

HRESULT Shader::Bind_Matrices(const _char* pConstantName, const _float4x4* pMatrix, _uint iNumMatrix)
{
    ID3DX11EffectVariable* pVariable = m_pEffect->GetVariableByName(pConstantName);
    if (nullptr == pVariable)
        return E_FAIL;

    ID3DX11EffectMatrixVariable* pMatrixVariable = pVariable->AsMatrix();
    if (nullptr == pMatrixVariable)
        return E_FAIL;

    return pMatrixVariable->SetMatrixArray(reinterpret_cast<const _float*>(pMatrix), 0, iNumMatrix);
}


HRESULT Shader::Bind_SRV(const _char* pConstantName, ID3D11ShaderResourceView* pSRV)
{
    ID3DX11EffectVariable* pVariable = m_pEffect->GetVariableByName(pConstantName);
    if (nullptr == pVariable)
        return E_FAIL;

    ID3DX11EffectShaderResourceVariable* pSRVariable = pVariable->AsShaderResource();
    if (nullptr == pSRVariable)
        return E_FAIL;

    return pSRVariable->SetResource(pSRV);
}

Shader* Shader::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const _tchar* pShaderFilePath, const D3D11_INPUT_ELEMENT_DESC* pElements, _uint iNumElements)
{
    Shader* pInstance = new Shader(pDevice, pContext);

    if (FAILED(pInstance->Initialize_Prototype(pShaderFilePath, pElements, iNumElements)))
    {
        MSG_BOX("Failed To Created : Shader");
        Safe_Release(pInstance);
    }

    return pInstance;
}

Component* Shader::Clone(void* pArg)
{
    Shader* pInstance = new Shader(*this);

    if (FAILED(pInstance->Initialize(pArg)))
    {
        MSG_BOX("Failed To Cloned : Shader");
        Safe_Release(pInstance);
    }

    return pInstance;
}


void Shader::Free()
{
    __super::Free();

    Safe_Release(m_pEffect);

    for (auto& pInputLayout : m_InputLayouts)
        Safe_Release(pInputLayout);
}
