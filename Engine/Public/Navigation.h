#pragma once

#include "Component.h"

BEGIN(Engine)

class ENGINE_DLL CNavigation final : public CComponent
{
public:
	typedef struct tagNavigationDesc
	{
		_int				iCellIndex = { -1};
		const _float4x4* pWorldMatrix = { nullptr };
	}NAVIGATION_DESC;
private:
	CNavigation(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CNavigation(const CNavigation& Prototype);
	virtual ~CNavigation() = default;

public:
	_int Get_CellIndex() { return m_iCurrentCellIndex; }

public:
	virtual HRESULT Initialize_Prototype(const _tchar* pNavigationDataFilePath);
	virtual HRESULT Initialize(void* pArg);

public:
	_bool isMove(_fvector vPosition);
	_vector Compute_Height(_fvector vPosition);

#ifdef _DEBUG
	HRESULT Render();
#endif

private:
	_int						m_iCurrentCellIndex = { -1 };
	vector<class CCell*>		m_Cells;
	
	static const _float4x4*		m_pWorldMatrix;


#ifdef _DEBUG
private:
	class CShader* m_pShader = { nullptr };

#endif

private:
	HRESULT Ready_Neighbors();

public:
	static CNavigation* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const _tchar* pNavigationDataFilePath);
	virtual CComponent* Clone(void* pArg) override;
	virtual void Free() override;
};

END