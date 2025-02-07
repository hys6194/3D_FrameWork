#pragma once

// 클라이언트에서 레벨을 생성할 때, 상속받아 사용하기 위해 만든 클래스
// 클라이언트에서 레벨에서 수행할 기능을 직접 만들 것

#include "Base.h"

BEGIN(Engine)

class ENGINE_DLL Level abstract : public Base
{
protected:
	Level(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual ~Level() = default;

public:
	virtual HRESULT Initialize();
	virtual void Update(_float fTimeDelta);
	virtual HRESULT Render();

protected:
	ID3D11Device*				m_pDevice = { nullptr };
	ID3D11DeviceContext*		m_pContext = { nullptr };
	class GameInstance*			m_pGameInstance = { nullptr };

public:
	virtual void Free() override;
};

END