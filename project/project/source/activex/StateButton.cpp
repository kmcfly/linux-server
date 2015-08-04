
#include "StateButton.h"

using namespace GUI;

GUI::CStateButton::BTN_STATE& CStateButton::BTN_STATE::operator =(const GUI::CStateButton::BTN_STATE& item)
{
	state = item.state;
	paint_status = item.paint_status;
	bmp_id = item.bmp_id;
	return *this;
}

CStateButton::CStateButton()
{
	m_curState = -1;
}

CStateButton::~CStateButton()
{

}

void CStateButton::OnDestroy()
{
	GUI::CButton::OnDestroy();

	m_stateVec.clear();

	m_curState = -1;
}

void CStateButton::SetStateImage(int uiState, PAINT_STATUS status, BITMAP_ID id)
{
	BTN_STATE btnState;
	btnState.state = uiState;
	btnState.paint_status = status;
	btnState.bmp_id = id;

	std::vector<BTN_STATE>::iterator iter;
	//先查找是否有此状态添加过
	for ( iter=m_stateVec.begin();iter!=m_stateVec.end();iter++)
	{
		if( iter->state == btnState.state && iter->paint_status == btnState.paint_status )
		{
			iter->bmp_id = btnState.bmp_id;
			return;
		}
	}

	//如果是一种新状态,直接添加
	m_stateVec.push_back(btnState);
}

void CStateButton::SetCurState(int uiState,bool bRefresh)
{
	assert(uiState>=0);

	//排除重复设置情况
	if( uiState == m_curState ) return;

	m_curState = uiState;

	std::vector<BTN_STATE>::iterator iter;
	//先查找是否有此状态添加过
	for ( iter=m_stateVec.begin();iter!=m_stateVec.end();iter++)
	{
		if( iter->state == uiState )
		{
			SetImage(iter->paint_status,iter->bmp_id,false);
		}
	}

	if( bRefresh )
	{
		Repaint();
	}
}

int CStateButton::GetCurState()
{
	return m_curState;
}
