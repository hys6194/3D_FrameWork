#include "PipeLine.h"
#include "Shader.h"

PipeLine::PipeLine()
{
}

void PipeLine::Set_Transform(TRANSFORMSTATE eState, _fmatrix Matrix)
{
    XMStoreFloat4x4(&m_TransformMatrices[eState], Matrix);
}

void PipeLine::Set_Transform(TRANSFORMSTATE eState, const _float4x4* pMatrix)
{
    m_TransformMatrices[eState] = *pMatrix;
}

HRESULT PipeLine::Bind_SR(Shader* pShader, const _char* pConstantName, TRANSFORMSTATE eState)
{
    return pShader->Bind_Matrix(&m_TransformMatrices[eState], pConstantName);
}

PipeLine* PipeLine::Create()
{
    return new PipeLine;
}

void PipeLine::Free()
{
    __super::Free();
}
