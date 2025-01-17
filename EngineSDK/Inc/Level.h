#pragma once

/* 클라이언트 개발자가 만드는 모든 레벨클래스들의 부모가 되는 클래스다. */
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
	ID3D11Device*		 m_pDevice = { nullptr };
	ID3D11DeviceContext* m_pContext = { nullptr };

	class GameInstance* m_pGameInstance = { nullptr };

public:
	virtual void Free() override;


};

END
