#include "Timer_Manager.h"
#include "Timer.h"

CTimer_Manager::CTimer_Manager()
{

}


_float CTimer_Manager::Get_TimeDelta(const _wstring& strTimerTag)
{
	CTimer*		pTimer = Find_Timer(strTimerTag);
	NULL_CHECK_RETURN(pTimer, 0.f);

	return pTimer->Get_TimeDelta();
}

void CTimer_Manager::Set_TimeDelta(const _wstring& strTimerTag)
{
	CTimer*		pTimer = Find_Timer(strTimerTag);
	NULL_CHECK(pTimer);

	pTimer->Update_Timer();
}

HRESULT CTimer_Manager::Ready_Timer(const _wstring& strTimerTag)
{
	CTimer*		pTimer = Find_Timer(strTimerTag);

	if (nullptr != pTimer)
		return E_FAIL;

	pTimer = CTimer::Create();
	NULL_CHECK_RETURN(pTimer, E_FAIL);

	m_Timers.emplace(strTimerTag, pTimer);	

	return S_OK;
}

CTimer * CTimer_Manager::Find_Timer(const _wstring& strTimerTag)
{
	auto	iter = m_Timers.find(strTimerTag);

	/*auto	iter = find_if(m_mapTimer.begin(), m_mapTimer.end(), 
							CTag_Finder(pTimerTag));*/

	if (iter == m_Timers.end())
		return nullptr;
	
	return iter->second;
}

CTimer_Manager* CTimer_Manager::Create()
{
	return new CTimer_Manager();
}

void CTimer_Manager::Free()
{
	__super::Free();

	for (auto& Pair : m_Timers)	
		Safe_Release(Pair.second);

	m_Timers.clear();	
}
