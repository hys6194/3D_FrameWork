#pragma once

#include "Component.h"

BEGIN (Engine)

class ENGINE_DLL VIBuffer abstract : public Component
{
protected:
	VIBuffer(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	VIBuffer(const VIBuffer& Prototype);
	virtual ~VIBuffer() = default;

public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;
	virtual HRESULT Render();

protected:
	// 정점들의 메모리 공간을 의미하는 컴객체
	ID3D11Buffer* m_pVB = { nullptr };

	// 정점을 잇는 인덱스들의 메모리 공간을 의미하는 컴객체
	ID3D11Buffer* m_pIB = { nullptr };

protected:
	D3D11_BUFFER_DESC			m_BufferDesc{};
	D3D11_SUBRESOURCE_DATA		m_InitialData{};


	// 정점과 다음 정점 까지의 바이트 단위 간격
	_uint						m_iVertexStride = {};		// 버텍스 버퍼 하나의 크기(한 정점이 차지하는 메모리 크기)
	_uint						m_iNumVertices = {};		// 버텍스 버퍼의 개수

	// 인덱스와 다음 인덱스 까지의 바이트 단위 간격
	_uint						m_iIndexStride = {};		// 인덱스스 버퍼 하나의 크기(한 정점이 차지하는 메모리 크기)
	_uint						m_iNumIndices = {};			// 정점 데이터를 효율적으로 참조하고 도형의 연결 순서를 정의하는 값.

	_uint						m_iNumVertexBuffers = {};	// 버텍스 버퍼의 개수
	DXGI_FORMAT					m_eIndexFormat = {};		// 열거체
	D3D_PRIMITIVE_TOPOLOGY		m_eTopology = {};			// 열거체

protected:
	HRESULT Create_Buffer(ID3D11Buffer** ppOut);

public:
	virtual Component* Clone(void* pArg) = 0;
	virtual void Free() override;

};

END