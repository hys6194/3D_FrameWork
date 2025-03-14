#pragma once

#include "Component.h"

BEGIN(Engine)

class ENGINE_DLL CNavigation final : public CComponent
{
public:
	typedef struct tagNavigationDesc
	{
		_int				iCellIndex = { -1 };
		const _float4x4* pWorldMatrix = { nullptr };
	}NAVIGATION_DESC;

private:
	CNavigation(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CNavigation(const CNavigation& Prototype);
	virtual ~CNavigation() = default;

public:
	HRESULT Initialize_Prototype(const _tchar* pNavigationDataFilePath);
	HRESULT Initialize(void* pArg);


#ifdef _DEBUG
	HRESULT Render();
#endif

public:
	_bool IsMove(_fvector vPosition);
	_vector Compute_Height(_fvector vPosition);

private:
	_int m_iCurrentCellIndex = { -1 };
	vector<class CCell*>	m_vecCell;


#ifdef _DEBUG
private:
	class CShader* m_pShader = { nullptr };

#endif

	static const _float4x4* m_pWorldMatrix;


private:
	HRESULT Ready_Neighbor();


public:
	static CNavigation* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const _tchar* pNavigationDataFilePath);
	virtual CComponent* Clone(void* pArg) override;
	virtual void Free() override;
};


END