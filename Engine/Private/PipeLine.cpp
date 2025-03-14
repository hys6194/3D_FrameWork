#include "PipeLine.h"
#include "Shader.h"

CPipeLine::CPipeLine(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: m_pDevice{ pDevice }
	, m_pContext{ pContext }
{
	Safe_AddRef(m_pDevice);
	Safe_AddRef(m_pContext);

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

 _vector* CPipeLine::Get_MouseWorldPosition(const _float4x4* TargetmatWorld)
{
	POINT pt;
	GetCursorPos(&pt);
	ScreenToClient(m_hWnd, &pt);

	_float4 fTest{ 0.f,0.f,0.f,0.f };
	_vector vTest{ 0.f,0.f,0.f,1.f };
	//_vector vRayPos, vRayDir;


	_uint i = 1;
	D3D11_VIEWPORT ViewPort;
	m_pContext->RSGetViewports(&i, &ViewPort); 

	fTest.x = pt.x / (ViewPort.Width * 0.5f) - 1.f;
	fTest.y = pt.y / -(ViewPort.Height * 0.5f) + 1.f;

	vTest = XMVectorSet(fTest.x, fTest.y, fTest.z, fTest.w);

	XMVector3Unproject(vTest, 
		ViewPort.TopLeftX, ViewPort.TopLeftY, 
		ViewPort.Width, ViewPort.Height,
		ViewPort.MinDepth, ViewPort.MaxDepth, 
		XMLoadFloat4x4(&m_TransformMatrices[D3DTS_PROJ]),
		XMLoadFloat4x4(&m_TransformMatrices[D3DTS_VIEW]),
		XMMatrixIdentity());

	return &vTest;
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

CPipeLine* Engine::CPipeLine::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, HWND _hWnd)
{
	CPipeLine* pInstance = new CPipeLine(pDevice, pContext);
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

	Safe_Release(m_pDevice);
	Safe_Release(m_pContext);
}
