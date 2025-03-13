#include "PipeLine.h"
#include "Shader.h"

CPipeLine::CPipeLine()
{
}

void CPipeLine::Update()
{
	for (size_t i = 0; i < D3DTS_END; i++)
	{
		XMStoreFloat4x4(&m_TransformInverseMatrices[i],
			XMMatrixInverse(nullptr, XMLoadFloat4x4(&m_TransformMatrices[i])));
	}

	memcpy(&m_vCamPosition, &m_TransformInverseMatrices[D3DTS_VIEW].m[3][0], sizeof(_float4));
}

const _float4* CPipeLine::Get_MouseWindowPosition() const
{
	POINT pt;
	GetCursorPos(&pt);
	ScreenToClient(m_hWnd, &pt);

	_float4 fTest { 0.f,0.f,0.f,0.f };

	fTest.x = pt.x;
	fTest.y = pt.y;

	return &fTest;
}

const _float4* CPipeLine::Get_MouseWorldPosition() const
{
	POINT pt;
	GetCursorPos(&pt);
	ScreenToClient(m_hWnd, &pt);

	_float4 fTest{ 0.f,0.f,0.f,0.f };

	fTest.x = pt.x;
	fTest.y = pt.y;

	//g_


	return nullptr;
}

void CPipeLine::Set_Transform(TRANSFORMSTATE eState, _fmatrix Matrix)
{
    XMStoreFloat4x4(&m_TransformMatrices[eState], Matrix);
}

void CPipeLine::Set_Transform(TRANSFORMSTATE eState, const _float4x4* pMatrix)
{
    m_TransformMatrices[eState] = *pMatrix;
}

HRESULT Engine::CPipeLine::Initialize(HWND _hWnd)
{
	m_hWnd = _hWnd;

	return S_OK;
}

HRESULT CPipeLine::Bind_SR(CShader* pShader, const _char* pConstantName, TRANSFORMSTATE eState)
{
    return pShader->Bind_Matrix(pConstantName, &m_TransformMatrices[eState]);
}

CPipeLine* Engine::CPipeLine::Create(HWND _hWnd)
{
	CPipeLine* pInstance = new CPipeLine;
	if(FAILED(pInstance->Initialize(_hWnd)))
	{
		MSG_BOX("Failed To Created : CPipeLine");
		Safe_Release(pInstance);
	}


    return pInstance;
}

void CPipeLine::Free()
{
	//Safe_Delete(m_hWnd);
    __super::Free();
}
