#pragma once
#include "Base.h"

// 뷰, 투영행렬을 저장하고 있는 클래스이다
// 특정행렬을 셰이더에 던지는 기능을 담당하고 있다

// 왜 월드행렬을 가지고 있는 Transform에서 수행하지 않고 PipeLine 클래스를 만들어서 수행하였는가?
// Transform 에서 수행하고 있는 행렬 연산 기능들이 무거운 기능이므로, 월드 행렬만 계산하는 것으로 하였다

// DX9에서는 장치에 행렬을 저장한다는 기능이 있었지만 11에서는 없어졌다
// 따라서 직접 만들어야 한다.

// 그러면 뷰 투영행렬을 사용하는 카메라에 만드는 것도 방법이지 않나?
// 그렇다면 뷰 투영행렬을 사용할 때 마다 카메라를 검색해서 카메라를 통해 뷰, 투영행렬을 가져와아 하는 일이 발생한다
// 그래서 PipeLine 클래스를 만들어 사용하였다


BEGIN(Engine)

class CPipeLine final : public CBase
{
public:
	enum TRANSFORMSTATE					{ D3DTS_VIEW, D3DTS_PROJ, D3DTS_END };
private:
										CPipeLine(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual								~CPipeLine() = default;

public:
	HRESULT								Initialize(HWND _hWnd);
	void								Update();

public:
	const _float4x4*					Get_Transform_Float4x4(TRANSFORMSTATE eState)
	{
		return &m_TransformMatrices[eState];
	}

	_matrix								Get_Transform_Matrix(TRANSFORMSTATE eState)
	{
		return XMLoadFloat4x4(&m_TransformMatrices[eState]);
	}

	const _float4x4*					Get_Transform_Inverse_Float4x4(TRANSFORMSTATE eState) const
	{
		return &m_TransformInverseMatrices[eState];
	}

	_matrix								Get_Transform_Inverse_Matrix(TRANSFORMSTATE eState) const 
	{
		return XMLoadFloat4x4(&m_TransformInverseMatrices[eState]);
	}

	const _float4*						Get_CamPosition() const 
	{
		return &m_vCamPosition;
	}

	//GameInstance의 상호참조로 문제가 생길까봐 PipeLine클래스에 제작
	_vector*							Get_MouseWindowPosition();
	_vector*							Shoot_RayLazer();
public:

	void								Set_Transform(TRANSFORMSTATE eState, _fmatrix Matrix);				// fmatrix 형 
	void								Set_Transform(TRANSFORMSTATE eState, const _float4x4* pMatrix);	// flaot4x4 형 

public:
	HRESULT								Bind_SR(class CShader* pShader, const _char* pConstantName, TRANSFORMSTATE eState);

private:
	_float4x4							m_TransformMatrices[D3DTS_END] = {};	
	_float4x4							m_TransformInverseMatrices[D3DTS_END] = {};
	_float4								m_vCamPosition = {};

private:
	HWND								m_hWnd = { nullptr };
	ID3D11Device*						m_pDevice = { nullptr };
	ID3D11DeviceContext*				m_pContext = { nullptr };



public:
	static CPipeLine*					Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, HWND _hWnd);
	virtual void						Free() override;
};

END