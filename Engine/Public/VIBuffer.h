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
	D3D11_BUFFER_DESC				m_BufferDesc{};
	D3D11_SUBRESOURCE_DATA			m_InitialData{};

	// 정점과 다음 정점 까지의 바이트 단위 간격
	_uint							m_iVertexStride = {};
	_uint							m_iNumVertices = {};

	// 인덱스와 다음 인덱스 까지의 바이트 단위 간격
	_uint							m_iIndexStride = {};
	_uint							m_iNumIndices = {};

	_uint							m_iNumVertexBuffers = {};
	DXGI_FORMAT						m_eIndexFormat = {};
	D3D_PRIMITIVE_TOPOLOGY			m_eTopology = {};

protected:
	HRESULT Create_Buffer(ID3D11Buffer** ppOut);

public:
	virtual Component* Clone(void* pArg) = 0;
	virtual void Free() override;

};

END