#include "Tool_Defines.h"
#include "GameObject.h"

BEGIN(Engine)
class CShader;
class CTexture;
class CVIBuffer_Flat_Terrain;
END

BEGIN(Tool)
class Terrain final : public CGameObject
{ 
public:
	enum TEXTURETYPE { TYPE_DIFFUSE, TYPE_MASK, TYPE_BRUSH, TYPE_END };

private:
	Terrain(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	Terrain(const Terrain& Prototype);
	virtual ~Terrain() = default;

public:
	virtual HRESULT						Initialize_Prototype() override;
	virtual HRESULT						Initialize(void* pArg) override;
	virtual void						Priority_Update(_float fTimeDelta) override;
	virtual void						Update(_float fTimeDelta) override;
	virtual void						Late_Update(_float fTimeDelta) override;
	virtual HRESULT						Render() override;

private:
	CShader*							m_pShaderCom				= { nullptr };
	CTexture*							m_pTextureCom[TYPE_END]		= { nullptr };
	CVIBuffer_Flat_Terrain*				m_pVIBufferCom				= { nullptr };

	_bool								m_bRender = { true };

private:
	HRESULT								Ready_Components();
	HRESULT								Bind_SR();

public:
	static Terrain*						Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject*				Clone(void* pArg);
	virtual void						Free() override;

};

END