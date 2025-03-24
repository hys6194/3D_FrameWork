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

_vector* CPipeLine::Get_MouseWindowPosition()
{
	POINT pt;
	GetCursorPos(&pt);
	ScreenToClient(m_hWnd, &pt);

	_float4 fTest{ 0.f,0.f,0.f,0.f };
	_vector vTest{ 0.f,0.f,0.f,1.f };


	_uint i = 1;
	D3D11_VIEWPORT ViewPort;
	m_pContext->RSGetViewports(&i, &ViewPort);

	fTest.x = pt.x / (ViewPort.Width * 0.5f) - 1.f;

	// 원래 0.5가 뷰 포트 상 제일 중앙이 되는 좌표임 
	// 플레이어의 팔 위치로 중점을 옮긴거 밖에 안됨
	fTest.y = pt.y / -(ViewPort.Height * 0.5f) + 1.f;

	vTest = XMVectorSet(fTest.x, fTest.y, fTest.z, 1.f);

	return &vTest;
}

_vector* CPipeLine::Get_PlayerViewPortPos()
{
	POINT pt;
	GetCursorPos(&pt);
	ScreenToClient(m_hWnd, &pt);

	_float4 fTest{ 0.f,0.f,0.f,0.f };
	_vector vTest{ 0.f,0.f,0.f,1.f };


	_uint i = 1;
	D3D11_VIEWPORT ViewPort;
	m_pContext->RSGetViewports(&i, &ViewPort);

	fTest.x = pt.x / (ViewPort.Width * 0.5f) - 1.f;

	// 원래 0.5가 뷰 포트 상 제일 중앙이 되는 좌표임 
	// 플레이어의 팔 위치로 중점을 옮긴거 밖에 안됨
	fTest.y = pt.y / -(ViewPort.Height * 0.33f) + 1.f;

	vTest = XMVectorSet(fTest.x, fTest.y, fTest.z, 1.f);

	return &vTest;
}
_float4* CPipeLine::Get_RayDirCoords()
{
	POINT pt;
	GetCursorPos(&pt);
	ScreenToClient(m_hWnd, &pt);

	_float3 fFar{ 0.f, 0.f, 0.f };
	_float4 fTest{ (_float)pt.x , (_float)pt.y ,0.f, 0.f };

	_uint i = 1;
	D3D11_VIEWPORT ViewPort;
	m_pContext->RSGetViewports(&i, &ViewPort);
	
	fFar.x = pt.x / (ViewPort.Width * 0.5f) - 1.f;
	fFar.y = pt.y / -(ViewPort.Height * 0.5f) + 1.f;
	
	_float4x4 fProj = m_TransformInverseMatrices[D3DTS_PROJ];
	
	// 투영의 역행렬
	_vector vRayPos = { 0.f,0.f,0.f,0.f };
	vRayPos = XMVector3TransformCoord(XMLoadFloat3(&fFar), XMLoadFloat4x4(&fProj));
	
	_vector vPos = { 0.f,0.f,0.f,1.f };
	_vector vDir = { 0.f,0.f,0.f,0.f };
	
	_vector vRayDir;
	vDir = XMVector4Normalize(vRayPos - vPos);
	vRayDir = XMVector4Normalize(vDir);
	
	// 뷰의 역행렬
	_float4x4 fView = m_TransformInverseMatrices[D3DTS_VIEW];
	vRayPos = XMVector3TransformCoord(vPos, XMLoadFloat4x4(&fView));
	
	
	_float4 fRayPos, fRayDir;
	//vRayDir = vDir;
	
	XMStoreFloat4(&fRayPos, vRayPos);
	XMStoreFloat4(&fRayDir, vDir);
	
	//XMVectorSetW(XMLoadFloat4(&fRayDir), 0.f);	

	return &fRayDir;
}

vector<_float4>* CPipeLine::Get_RayCoords()
{
	m_vecRays.clear();

	POINT pt;
	GetCursorPos(&pt);
	ScreenToClient(m_hWnd, &pt);

	_float3 fFar{ 0.f, 0.f, 0.f };
	_float4 fTest{ (_float)pt.x , (_float)pt.y ,0.f, 0.f };

	_uint i = 1;
	D3D11_VIEWPORT ViewPort;
	m_pContext->RSGetViewports(&i, &ViewPort);

	fFar.x = pt.x / (ViewPort.Width * 0.5f) - 1.f;
	fFar.y = pt.y / -(ViewPort.Height * 0.5f) + 1.f;

	_float4x4 fProj = m_TransformInverseMatrices[D3DTS_PROJ];

	// 투영의 역행렬
	_vector vRayPos = { 0.f,0.f,0.f,0.f };
	vRayPos = XMVector3TransformCoord(XMLoadFloat3(&fFar), XMLoadFloat4x4(&fProj));

	_vector vPos = { 0.f,0.f,0.f,1.f };
	_vector vDir = { 0.f,0.f,0.f,0.f };

	_vector vRayDir;
	vDir = XMVector4Normalize(vRayPos - vPos);
	vRayDir = XMVector4Normalize(vDir);

	// 뷰의 역행렬
	_float4x4 fView = m_TransformInverseMatrices[D3DTS_VIEW];
	vRayPos = XMVector3TransformCoord(vPos, XMLoadFloat4x4(&fView));


	_float4 fRayPos, fRayDir;
	//vRayDir = vDir;

	XMStoreFloat4(&fRayPos, vRayPos);
	XMStoreFloat4(&fRayDir, vDir);

	//XMVectorSetW(XMLoadFloat4(&fRayDir), 0.f);	
	m_vecRays.push_back(fRayPos);
	m_vecRays.push_back(fRayDir);

	return &m_vecRays;
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

    __super::Free();

	Safe_Release(m_pDevice);
	Safe_Release(m_pContext);
}
