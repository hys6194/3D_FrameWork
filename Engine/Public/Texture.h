#pragma once

#include "Component.h"

BEGIN(Engine)

class ENGINE_DLL Texture final : public Component
{
private:
	Texture(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	Texture(const Texture& Prototype);
	virtual ~Texture() = default;

public:
	HRESULT Initialize_Prototype(const _tchar* pTextureFilePath, _uint iNumTextures);
	HRESULT Initialize(void* pArg);

public:
	HRESULT Apply_SR(class Shader* pShader, const _char* pConstantName, _uint iTextureIndex) const;

private:

	// 텍스쳐가 몇장인지에 대한 정보를 담을 멤버변수
	_uint								m_iNumTextures = {};

	// 여러장의 객체를 가지되, 한가지 텍스쳐를 고르고,
	// 이를 꺼내서 사용하기 위해 vector로 선언
	// 정점과 정점을 잇는 픽셀의 색을 입혀줘야 함
	// 따라서 ID3D11ShaderResourceView* 자료형으로 사용
	vector<ID3D11ShaderResourceView*>	m_vecSRV;

public:
	// default 연산자로 대입 안할 경우, 1장만 읽어들이는 것으로 함
	static Texture* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const _tchar* pTextureFilePath, _uint iNumTextures = 1);
	virtual Component* Clone(void* pArg) override;
	virtual void Free()override;

};

END