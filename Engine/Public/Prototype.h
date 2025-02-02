#pragma once

#include "Base.h"

BEGIN(Engine)

class Prototype final : public Base
{
private:
	Prototype(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual ~Prototype() = default;

public:
	HRESULT Initialize(_uint iNumLevels);

private:
	ID3D11Device*				m_pDevice = { nullptr };
	ID3D11DeviceContext*		m_pContext = { nullptr };
	_uint						m_iNumLevels = {};

private:
	map<const wstring, Base*>* m_pPrototype = { nullptr };
	typedef map<const wstring, Base*> PROTOTYPES;

	// EngineDesc에 VECTOR 타입을 넣지 않았는데 왜?
	// memcpy할 때 컴파일러가 터짐
	// 
	// Clone 함수는 언제?
	// -> Prototype을 가진 Object를 만들 때 만들 예정
	// 


public:
	static Prototype* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, _uint iNumLevels);
	virtual void Free() override;
};

END