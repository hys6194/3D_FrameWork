#pragma once

#include "VIBuffer.h"

BEGIN(Engine)

class ENGINE_DLL CVIBuffer_Instancing : public CVIBuffer
{
public:
	typedef struct tagInstanceDesc
	{
		_uint			iNumInstances = {};	// 인스턴싱하여 생성할 객체의 개수
		_float3			vCenter;
		_float3			vRange;
	}INSTANCE_DESC;

protected:
	CVIBuffer_Instancing(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CVIBuffer_Instancing(const CVIBuffer_Instancing& Prototype);
	virtual ~CVIBuffer_Instancing() = default;

public:
	virtual HRESULT				Initialize_Prototype(CVIBuffer_Instancing::INSTANCE_DESC* pDesc);
	virtual HRESULT				Initialize(void* pArg) override;
	virtual HRESULT				Render() override;

public:
	virtual HRESULT				Bind_Input_Assembler() override;

protected:
	ID3D11Buffer*				m_pVBInstance = { nullptr };
	_uint						m_iInstanceStride = {};
	_uint						m_iNumInstance = {};
	_uint						m_iNumIndexPerInstance = {}; // 인스턴스 하나당 사용할 인덱스의 개수

	INSTVTX*					m_pInstanceVertices = { nullptr };



public:
	virtual CComponent*			Clone(void* pArg) = 0;
	virtual void				Free() override;

};

END