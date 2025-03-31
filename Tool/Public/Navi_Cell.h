#pragma once

#include "Tool_Defines.h"
#include "VIBuffer.h"

// 이 클래스는 삼각형 버퍼를 동적으로 그리기 위해서 만든 클래스
// 파티클에서 배운 버퍼를 동적에서 다루는 거 복습하면서 감을 익혀보자
// 우선 이것도 행렬을 가지는 버퍼를 가지고 있는게 맞아보인다

BEGIN(Engine)

END

BEGIN(Tool)

class CNavi_Cell final : public CVIBuffer
{
private:
	CNavi_Cell(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CNavi_Cell(const CNavi_Cell& Prototype);
	virtual ~CNavi_Cell() = default;

public:
	virtual HRESULT				Initialize_Prototype();
	virtual HRESULT				Initialize(void* pArg) override;

public:
	virtual HRESULT				Bind_Input_Assembler();
	virtual HRESULT				Render();

public:
	void						Modify_VertexPoint(_uint iVertexIndex, _vector vCoord);

private:
	VTXPOS						m_pVertices0;
	VTXPOS						m_pVertices1;

	_bool						m_bSwap = {};

public:
	static  CNavi_Cell*			Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CComponent*			Clone(void* pArg) override;
	virtual void				Free() override;
};

END