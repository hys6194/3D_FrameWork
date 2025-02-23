#pragma once

#include "Base.h"
#include "Light.h"


// Render의 후처리를 위해서 만든 클래스

BEGIN(Engine)

class Light_Manager final : public Base
{
private:
	Light_Manager(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual ~Light_Manager() = default;

public:
	const LIGHT_DESC* Get_LightDesc(_uint iLightIndex)const
	{
		auto iter = m_listLights.begin();

		for (size_t i = 0; i < iLightIndex; ++i)
			++iter;

			if (iter == m_listLights.end())
				return nullptr;

			return (*iter)->Get_LightDesc();
		
	}

public:
	HRESULT Initialize();
	HRESULT Add_Light(const LIGHT_DESC& pDesc);

private:
	ID3D11Device* m_pDevice = { nullptr };
	ID3D11DeviceContext* m_pContext = { nullptr };
	list<class Light*>			m_listLights;

public:
	static Light_Manager* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual void Free() override;
};

END