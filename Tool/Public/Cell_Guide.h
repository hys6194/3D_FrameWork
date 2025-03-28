#pragma once

// 이 클래스를 어떻게 만들어야 할까?
// 내 의도는 동적인 삼각형을 그리고 피킹 좌표를 저장하는 클래스로 만들고 싶어
// 일단 이 클래스는 Navi 버튼을 눌렀을 때
// 버퍼를 생성하고 정점 3개를 선언하고 나면 point를 Cell 클래스에 전달해야해
// 우선 피킹했을 때 Render하는 것으로 목표를 잡아보자
// 그러면 이거 상속을 어떻게 할까?
// 
// 이거 차라리 Tool에서만 쓰이는 Cell클래스로 만들고 이를 버퍼를 동적할당하여
// 사용해보자 선생님이 파티클 했던 대로 해보면 될 듯 한데
//

#include "Tool_Defines.h"
#include "GameObject.h"

BEGIN(Engine)
class CShader;			// 셰이더 하나만 있으면 되려나
class CVIBuffer_Cell;	// 
END

BEGIN(Tool)

class CCell_Guide : public CGameObject
{

public:
	typedef struct tagCellPoint {

		_vector v0;
		_vector v1;
		_vector v2;
	}CELL_POS;
private:
	CCell_Guide(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CCell_Guide(const CCell_Guide& Prototype);
	virtual ~CCell_Guide() = default;

public:

	_bool Get_Modify()
	{
		return m_bIsModify;
	}

	_bool Is_Empty()
	{
		return (m_vecBufferComs.empty() ? true : false);
	}


public:
	//virtual HRESULT					Initialize_Prototype();
	//virtual HRESULT					Initialize(void* pArg);
	//virtual void					Late_Update(_float fTimeDelta);
	//virtual HRESULT					Render();


	virtual HRESULT					Initialize_Prototype();
	virtual HRESULT					Initialize(void* pArg);
	virtual void					Priority_Update(_float fTimeDelta);
	virtual void					Update(_float fTimeDelta, _vector vCoord);
	virtual void					Late_Update(_float fTimeDelta);
	virtual HRESULT					Render();
public:
	HRESULT							Bind_SR();
	HRESULT							Ready_Component();

	HRESULT							Clone_VIBuffer();
	// 야 이거 상속 게임 오브젝트로 하는게 맞다
	// 그래야 트랜스 폼으로 Add_Renderer랑 Bind시킨다

private:
	CShader*						m_pShaderCom	 = { nullptr };
	class CNavi_Cell*				m_pVIBufferCom	 = { nullptr };

	vector<class CNavi_Cell*>		m_vecBufferComs;
	

	_bool							m_bIsModify = { false };
	_uint							m_iIndex = { 0 };

	_vector							m_vPoint[3];

	//어떻게 해야 0,1,2의 정보가 담겨있는 배열을 저장할 수 있을까
	vector<CELL_POS>				m_vecCellPos;
private:
	void							Check_Cell_Translation();
	void							Correct_CellPoint(_vector vCoord);
	void							Calculate_CellNorvec();

public:
	void							Save_Data();
	void							Load_Data();

public:
	static CCell_Guide*				Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject*			Clone(void* pArg);
	virtual void					Free() override;
};

END