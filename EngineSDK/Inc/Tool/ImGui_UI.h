#pragma once

#include "ImGui_Base.h"

BEGIN(Engine)

class CImGui_UI : public CImGui_Base
{
private:
	CImGui_UI(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual ~CImGui_UI() = default;

public:
	virtual HRESULT							Initialize() override;
	virtual void							Update(_float fTimeDelta) override;
	virtual HRESULT							Late_Update(_float fTimeDelta) override;


public:
	static CImGui_UI*						Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual void							Free() override;
};

END