#pragma once

#include "Base.h"

BEGIN(Engine)

class CMyFont : public CBase
{private:
	CMyFont(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual ~CMyFont() = default;

public:
	HRESULT Initialize(const _tchar* pFontFilePath);
	HRESULT Render(const _wstring& strText, const _float2& vPosition, _fvector vColor, _float fRadian, const _float2& vOrigin, _float fScale);

private:
	ID3D11Device* m_pDevice = { nullptr };
	ID3D11DeviceContext* m_pContext = { nullptr };

private:
	SpriteFont* m_pFont = { nullptr };
	SpriteBatch* m_pBatch = { nullptr };

public:
	static CMyFont* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const _tchar* pFontFilePath);
	virtual void Free() override;
};

END