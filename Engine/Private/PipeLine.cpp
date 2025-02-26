#include "PipeLine.h"
#include "Shader.h"

PipeLine::PipeLine()
{
}

void PipeLine::Update()
{
	for (size_t i = 0; i < D3DTS_END; i++)
	{
		XMStoreFloat4x4(&m_TransformInverseMatrices[i],
			XMMatrixInverse(nullptr, XMLoadFloat4x4(&m_TransformMatrices[i])));
	}

	memcpy(&m_vCamPosition, &m_TransformInverseMatrices[D3DTS_VIEW].m[3][0], sizeof(_float4));
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
    return pShader->Bind_Matrix(pConstantName, &m_TransformMatrices[eState]);
}

PipeLine* PipeLine::Create()
{
    return new PipeLine;
}

void PipeLine::Free()
{
    __super::Free();
}
