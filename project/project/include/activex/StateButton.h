
#ifndef _STATE_BUTTON_H_
#define _STATE_BUTTON_H_

#include "Wnd.h"
#include "Button.h"
#include <vector>

namespace GUI
{
	class CStateButton : public CButton
	{
	public:
		CStateButton();
		virtual ~CStateButton();

		void SetStateImage(int uiState, PAINT_STATUS status, BITMAP_ID id);

		//����״̬Bmp��ɺ�,��Ҫ����һ�µ�ǰ״̬
		void SetCurState(int uiState,bool bRefresh);
		int GetCurState();

	protected:
		virtual void OnDestroy();

		struct BTN_STATE
		{
			int							state;
			PAINT_STATUS	paint_status;
			BITMAP_ID			bmp_id;
			BTN_STATE& operator=(const BTN_STATE& item);
		};
		std::vector<BTN_STATE>	m_stateVec;
		int				m_curState;
	};

}

#endif

