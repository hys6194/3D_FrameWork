#pragma once

#include "Base.h"

/* GameObject, Component의 원형 객체들을 레벨 별로 모아둘 것*/
/* 지정한 원형객체를 복사하여(사본으로 만들어서) 리턴할 것*/

class Prototype_Manager final : public Base
{
private:
	Prototype_Manager(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual ~Prototype_Manager();

public:
	HRESULT Initialize();
	HRESULT Add_Prototype();
	HRESULT Find_Prototype();


private:
	ID3D11Device*			m_pDevice = { nullptr };
	ID3D11DeviceContext*	m_pContext = { nullptr };

public:
	static Prototype_Manager* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual void Free() override;

};

