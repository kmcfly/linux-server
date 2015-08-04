#include "Wnd.h"
using namespace GUI;

GUI::CWnd* GUI::CWnd::MoveFocusUp(unsigned long xFocus, unsigned long yFocus) const
{
	if (NULL == m_pFocusMap)
	{
		return NULL;
	}

	assert((xFocus < m_hCtrlNum) && (yFocus < m_vCtrlNum));

	CWnd* pCtrl= NULL; 

	//���Ϸ�������
	while (0 < yFocus) 
	{
		-- yFocus;

		pCtrl = m_pFocusMap[(yFocus * m_hCtrlNum) + xFocus];

		if ( (NULL != pCtrl) && (pCtrl->CanSetFocus()) )
		{
			return pCtrl;
		}
	}

	return NULL;	
}

GUI::CWnd* GUI::CWnd::MoveFocusDown(unsigned long xFocus, unsigned long yFocus) const
{
	if (NULL == m_pFocusMap)
	{
		return NULL;
	}

	assert((xFocus < m_hCtrlNum) && (yFocus < m_vCtrlNum));

	CWnd* pCtrl= NULL; 

	//���·�������
	while (yFocus < (m_vCtrlNum - 1)) 
	{
		++ yFocus;

		pCtrl = m_pFocusMap[(yFocus * m_hCtrlNum) + xFocus];

		if ( (NULL != pCtrl) && (pCtrl->CanSetFocus()) )
		{
			return pCtrl;
		}
	}

	return NULL;
}

GUI::CWnd* GUI::CWnd::MoveFocusLeft(unsigned long xFocus, unsigned long yFocus) const
{
	if (NULL == m_pFocusMap)
	{
		return NULL;
	}

	assert((xFocus < m_hCtrlNum) && (yFocus < m_vCtrlNum));

	CWnd* pCtrl= NULL; 

	unsigned long num = 0;
	unsigned long count = (m_hCtrlNum * m_vCtrlNum);

	//һ���������������ߡ�֮�֡��͵ķ������������Ҽ��෴
	do
	{
		if (0 < xFocus)
		{
			-- xFocus;
		}
		else
		{
			if (0 < yFocus)
			{
				//��һ�У����ұ�
				-- yFocus;
				xFocus = (m_hCtrlNum - 1);
			}
			else
			{
				xFocus = 0;//(m_hCtrlNum - 1);
				yFocus = 0;//(m_vCtrlNum - 1);
			}
		}

		pCtrl = m_pFocusMap[(yFocus * m_hCtrlNum) + xFocus];

		if ( (NULL != pCtrl) && (pCtrl->CanSetFocus()) )
		{
			return pCtrl;
		}

		++ num;
	} while(num < count);

	return NULL;
}

GUI::CWnd* GUI::CWnd::MoveFocusRight(unsigned long xFocus, unsigned long yFocus) const
{
	if (NULL == m_pFocusMap)
	{
		return NULL;
	}

	assert((xFocus < m_hCtrlNum) && (yFocus < m_vCtrlNum));

	CWnd* pCtrl= NULL; 

	unsigned long num = 0;
	unsigned long count = (m_hCtrlNum * m_vCtrlNum);

	++ xFocus;
	//һ���������������ߡ�֮�֡��͵ķ���������������෴
	do 
	{
		if (xFocus >= m_hCtrlNum)
		{
			xFocus = 0;

			++ yFocus;

			if (yFocus >= m_vCtrlNum)
			{
				yFocus = 0;
			}
		}

		pCtrl = m_pFocusMap[(yFocus * m_hCtrlNum) + xFocus];

		if ( (NULL != pCtrl) && (pCtrl->CanSetFocus()) )
		{
			return pCtrl;
		}

		++ num;
		++ xFocus;
	} while(num < count);

	return NULL;
}

GUI::CWnd* GUI::CWnd::MoveFocusRightEx(unsigned long xFocus, unsigned long yFocus) const
{
	if (NULL == m_pFocusMap)
	{
		return NULL;
	}

	assert((xFocus<m_hCtrlNum) && (yFocus<m_vCtrlNum));

	//unsigned long count = m_hCtrlNum * m_vCtrlNum;
	unsigned long ctrlIndex = yFocus * m_hCtrlNum + xFocus;
	unsigned long index = ctrlIndex;
	unsigned long endIndex = yFocus * m_hCtrlNum + m_hCtrlNum - 1;
	CWnd* pCurCtrl = m_pFocusMap[index];
	CWnd* pCtrl= pCurCtrl; 
	//��������
	do 
	{
		++index;	
		if (index> endIndex)
		{
			index = yFocus * m_hCtrlNum;
		} 
		pCtrl = m_pFocusMap[index];
		if ( (NULL != pCtrl) && (pCtrl->CanSetFocus()) )
		{
			if (pCtrl->CanSetFocus())
			{
				return pCtrl;
				break;
			}			
		}
	} while(index != ctrlIndex);

	if ((NULL != pCtrl) && (pCtrl->CanSetFocus()) )
	{
		return pCtrl;
	} 
	else
	{
		return NULL;
	}
}


GUI::CWnd* GUI::CWnd::MoveFocusLeftEx(unsigned long xFocus, unsigned long yFocus) const
{
	if (NULL == m_pFocusMap)
	{
		return NULL;
	}

	assert((xFocus<m_hCtrlNum) && (yFocus<m_vCtrlNum));

	//unsigned long count = m_hCtrlNum * m_vCtrlNum;
	unsigned long ctrlIndex = yFocus * m_hCtrlNum + xFocus;
	unsigned long index = ctrlIndex;
	unsigned long endIndex = yFocus * m_hCtrlNum;
	CWnd* pCurCtrl = m_pFocusMap[index];
	CWnd* pCtrl= pCurCtrl; 
	//�����Ϸ�������
	do 
	{
		if (endIndex == index)
		{
			index = yFocus * m_hCtrlNum + m_hCtrlNum - 1;
		} 
		else
		{
			--index;
		}
		pCtrl = m_pFocusMap[index];
		if ( (NULL != pCtrl) && (pCtrl->CanSetFocus()) )
		{
			return pCtrl;
			break;
		}
	} while(index != ctrlIndex);

	if ((NULL != pCtrl) && (pCtrl->CanSetFocus()))
	{
		return pCtrl;
	} 
	else
	{
		return NULL;
	}
}

GUI::CWnd* GUI::CWnd::GetDefaultFocusCtrl(DEFUALT_FOCUS mode)
{
	if ( (NULL == m_pActiveCtrl) && (NULL != m_pFocusMap) )
	{
		switch (mode)
		{
		case DEFUALT_FOCUS_NONE:
			break;
		case DEFUALT_FOCUS_TOP_LEFT:
			{
				unsigned long count = m_vCtrlNum * m_hCtrlNum;
				for (unsigned long i = 0; i < count; ++i)
				{
					if ( (NULL != m_pFocusMap[i]) && (m_pFocusMap[i]->CanSetFocus()) )
					{
						return m_pFocusMap[i];
					}
				}
			}
			break;
		case DEFUALT_FOCUS_TOP_RIGHT:
			{
				CWnd* pBaseCtrl = NULL;
				for (int y = 0; y < m_vCtrlNum; ++y)
				{
					for (int x = m_hCtrlNum - 1; x >= 0; --x)
					{
						pBaseCtrl = m_pFocusMap[x + y * m_hCtrlNum];
						if ( (NULL != pBaseCtrl) && (pBaseCtrl->CanSetFocus()) )
						{
							return pBaseCtrl;
						}
					}
				}
			}
			break;
		case DEFUALT_FOCUS_BOTTOM_LEFT:
			{
				CWnd* pBaseCtrl = NULL;
				for (int y = m_vCtrlNum - 1; y >= 0 ; --y)
				{
					for (int x = 0; x < m_hCtrlNum; ++x)
					{
						pBaseCtrl = m_pFocusMap[x + y * m_hCtrlNum];
						if ( (NULL != pBaseCtrl) && (pBaseCtrl->CanSetFocus()) )
						{
							return pBaseCtrl;
						}
					}
				}
			}
			break;
		case DEFUALT_FOCUS_BOTTOM_RIGHT:
			{
				unsigned long count = m_vCtrlNum * m_hCtrlNum;
				for (unsigned long i = count - 1; i >= 0; --i)
				{
					if ( (NULL != m_pFocusMap[i]) && (m_pFocusMap[i]->CanSetFocus()) )
					{
						return m_pFocusMap[i];
					}
				}
			}
			break;
		default:
			break;
		}
	}

	return NULL;
}

unsigned long GUI::CWnd::OnFocusProc(unsigned long keyID, unsigned long xPoint, unsigned long yPoint)
{
	if (!IsShow() || !m_bEnable)
	{
		return KEY_VIRTUAL_MSG_CONTINUE;
	}

	if (NULL == m_pFocusMap)
	{	//�޽����ͼ��ֱ�ӷ���
		return KEY_VIRTUAL_MSG_CONTINUE;
	}

	switch (keyID)
	{
	case KEY_VIRTUAL_MOVE:
		{
			if (KEY_VIRTUAL_MSG_STOP == OnFocusProcEx(keyID, xPoint, yPoint))
			{
				return KEY_VIRTUAL_MSG_STOP;
			}

			if ((NULL != m_pActiveCtrl) && m_pActiveCtrl->IsShow() && m_pActiveCtrl->IsEnable() && \
				m_pActiveCtrl->IsInTheRect(xPoint, yPoint))//�������ڻ�Ӵ�����
			{
				return  m_pActiveCtrl->OnFocusProc(keyID, xPoint, yPoint);
			} 
			else//��겻�ڻ�Ӵ�����
			{
				CWnd* pCtrl = InWhichActiceCtrl(xPoint, yPoint);
				if (NULL != pCtrl)//��ǰ�����ĳ���Ӵ�����
				{
					pCtrl->SetDefaultFocusMode(DEFUALT_FOCUS_NONE);
					bool ret = SetCtrlFocus(pCtrl, true);
					assert(ret);//��ɶ���ʧ�ܵ�ԭ��ܿ��������ڣ������ý���ģ��ؼ��Ľ��������غ��ˣ���ʹ�ܵĿؼ�������ʹ�ܵĿؼ���

					return  pCtrl->OnFocusProc(keyID, xPoint, yPoint);
				} 
				else//��ǰ��겻���κ��Ӵ�����
				{
					if (NULL != m_pActiveCtrl)
					{
						SetCtrlFocus(m_pActiveCtrl, false);
					}
				}
			}

			return KEY_VIRTUAL_MSG_CONTINUE;
		}
		break;
	case KEY_VIRTUAL_UP:
	case KEY_VIRTUAL_DOWN:
	case KEY_VIRTUAL_LEFT:
	case KEY_VIRTUAL_RIGHT:
		{
			if (KEY_VIRTUAL_MSG_STOP == OnFocusProcEx(keyID, xPoint, yPoint))
			{
				return KEY_VIRTUAL_MSG_STOP;
			}

			if (NULL != m_pActiveCtrl)
			{
				if (KEY_VIRTUAL_MSG_STOP == m_pActiveCtrl->OnFocusProc(keyID, xPoint, yPoint))
				{
					return KEY_VIRTUAL_MSG_STOP;
				}

				int xFocus = 0, yFocus = 0;		
				m_pActiveCtrl->GetFocusPos(xFocus, yFocus);

				CWnd* pCtrl = NULL;
				switch (keyID)
				{
				case KEY_VIRTUAL_UP:
					pCtrl = MoveFocusUp(xFocus, yFocus);
					if (NULL != pCtrl)
					{
						pCtrl->SetDefaultFocusMode(DEFUALT_FOCUS_BOTTOM_RIGHT);
						int tmp_x_focus = xFocus;
						int tmp_y_focus = yFocus;
						pCtrl->GetFocusPos(xFocus, yFocus);
						if ((NULL != m_pParent) && (yFocus >= tmp_y_focus ) && (xFocus >= tmp_x_focus ))
						{
							return KEY_VIRTUAL_MSG_CONTINUE;
						}
					}				
					break;
				case KEY_VIRTUAL_DOWN:
					pCtrl = MoveFocusDown(xFocus, yFocus);
					if (NULL != pCtrl)
					{
						pCtrl->SetDefaultFocusMode(DEFUALT_FOCUS_TOP_LEFT);
						int tmp_x_focus = xFocus;
						int tmp_y_focus = yFocus;
						pCtrl->GetFocusPos(xFocus, yFocus);
						if ((NULL != m_pParent) && (yFocus <= tmp_y_focus) && (xFocus <= tmp_x_focus))
						{
							return KEY_VIRTUAL_MSG_CONTINUE;
						}
					}
					break;
				case KEY_VIRTUAL_LEFT:
					pCtrl = MoveFocusLeft(xFocus, yFocus);
					if (NULL != pCtrl)
					{
						pCtrl->SetDefaultFocusMode(DEFUALT_FOCUS_BOTTOM_RIGHT);
						int tmp_x_focus = xFocus;
						int tmp_y_focus = yFocus;
						pCtrl->GetFocusPos(xFocus, yFocus);
						if ((NULL != m_pParent) && (xFocus >= tmp_x_focus) && (yFocus == tmp_y_focus))
						{
							return KEY_VIRTUAL_MSG_CONTINUE;
						}
					}				
					break;
				case KEY_VIRTUAL_RIGHT:
					pCtrl = MoveFocusRight(xFocus, yFocus);
					if (NULL != pCtrl)
					{
						pCtrl->SetDefaultFocusMode(DEFUALT_FOCUS_TOP_LEFT);
						int tmp_x_focus = xFocus;
						int tmp_y_focus = yFocus;
						pCtrl->GetFocusPos(xFocus, yFocus);
						if ((NULL != m_pParent) && (xFocus <= tmp_x_focus ) && (yFocus <= tmp_y_focus ))
						{
							return KEY_VIRTUAL_MSG_CONTINUE;
						}
					}				
					break;
				default:
					return KEY_VIRTUAL_MSG_CONTINUE;
					break;
				}

				if (NULL == pCtrl)//���ؿգ������κζ���
				{
					//	bool ret = SetCtrlFocus(m_pActiveCtrl, false);
					//	assert(ret);//��ɶ���ʧ�ܵ�ԭ��ܿ��������ڣ������ý���ģ��ؼ��Ľ��������غ��ˣ���ʹ�ܵĿؼ�������ʹ�ܵĿؼ���
					return KEY_VIRTUAL_MSG_CONTINUE;
				}
				else if ( pCtrl != m_pActiveCtrl )
				{
					bool ret = SetCtrlFocus(pCtrl, true);
					assert(ret);//��ɶ���ʧ�ܵ�ԭ��ܿ��������ڣ������ý���ģ��ؼ��Ľ��������غ��ˣ���ʹ�ܵĿؼ�������ʹ�ܵĿؼ���

					return KEY_VIRTUAL_MSG_STOP;
				}
				else
				{
					//���Ӵ�����ͬ�������κβ���
					return KEY_VIRTUAL_MSG_STOP;
				}
			} 
			else //�޻�Ӵ��ڣ�����Ĭ�Ͻ���
			{
				return SetDefaultFocus();
			}
		}
		break;
	default:
		{
			if ((m_width <= xPoint) && (m_height <= yPoint) && (0 < m_childList.size()))
			{
				if ((NULL == m_pActiveCtrl) || \
					(NULL != m_pActiveCtrl) && (!m_pActiveCtrl->IsShow() || !m_pActiveCtrl->IsEnable()))
				{
					WND_LIST::const_iterator iter;
					for (iter = m_childList.begin(); iter != m_childList.end(); ++iter)
					{
						if ((*iter)->IsShow() && (*iter)->IsEnable() && (*iter)->IsSupportFocus())
						{
							if (KEY_VIRTUAL_MSG_STOP == (*iter)->OnFocusProc(keyID, xPoint, yPoint))
							{
								m_pActiveCtrl = (*iter);
								return KEY_VIRTUAL_MSG_STOP;
							}
							else
							{
								SetCtrlFocus((*iter), true);
								m_pActiveCtrl = (*iter);
								return KEY_VIRTUAL_MSG_STOP;
							}
						}
					}
				}
			}

			return KEY_VIRTUAL_MSG_CONTINUE;
		}
		break;
	}
}

unsigned long GUI::CWnd::SetDefaultFocus(bool bRefresh/* = true*/)
{
	CWnd* pWnd = GetDefaultFocusCtrl(m_defaultFocus);
	if (NULL != pWnd)
	{
		SetCtrlFocus(pWnd, true, bRefresh);
		return KEY_VIRTUAL_MSG_STOP;
	}
	else
	{
		if (bRefresh)
		{
			if (0 == m_childList.size())
			{
				Repaint();
			}
		}

		return KEY_VIRTUAL_MSG_CONTINUE;
	}
}


/***********************************************************
��������  SetCtrlFocus
�������������ÿؼ����㣻
���������pCtrl���ؼ�ָ�룻 bFocus���Ƿ��н��㣻
����������ޣ�
����ֵ��  �ޣ�
˵����    ��1�����öԻ����пؼ��Ľ��㣬������ֻ�ܵ��ô˺�����
��2���Ի����������һ�����н���Ŀؼ���
��3����bFocusΪtrue:  �ı�Ի���ǰ�ؼ��Ľ��� �� ��ǰ�ؼ���λ�ã�
��bFocusΪfalse: �����ؼ��Ľ���ȥ������������������

������/�޸���                   ����/�޸�����                  ��ע
��չ                            28/08/07                       ����
***********************************************************/
bool GUI::CWnd::SetCtrlFocus(CWnd* pCtrl, bool bFocus, bool bRefresh/* = true*/)
{
	assert(NULL != pCtrl);

	if (bFocus) //�����н�������
	{	
		if ( !pCtrl->CanSetFocus() ) //����ؼ��������ý��㣬����ʧ��
		{
			return false;
		}
		else //��ʼ���ÿؼ����㣬 ���سɹ�
		{
			unsigned long newCtrlID = pCtrl->GetID();
			unsigned long oldCtrlID = INVALID_ID;

			if (NULL != m_pActiveCtrl) //ȥ����ǰ���н���ؼ��Ľ���
			{
				if (m_pActiveCtrl->IsFocus())
				{
					if (!m_pActiveCtrl->SetFocus(false, bRefresh))
					{
						//��ǰ�ؼ���Ȼ�������ý��� ? ʧ��ԭ�����������m_pActive���ɼ����߲�ʹ��
						m_pActiveCtrl->m_bFocus = false;
//						assert(false);
					}

					oldCtrlID = m_pActiveCtrl->GetID();
				}
			}

			pCtrl->SetFocus(true, bRefresh); //���øĿؼ��Ľ���
			m_pActiveCtrl = pCtrl;//���õ�ǰ�ؼ�λ��
			OnChangeFocus(oldCtrlID, newCtrlID);

			return true;
		}
	}
	else //�����޽�������
	{
		//ע�⣬��ʱ��ǰ�ؼ���λ��û�иı�
		if (pCtrl->IsFocus())
		{
			assert (m_pActiveCtrl == pCtrl);
			m_pActiveCtrl = NULL;
			
			if (pCtrl->SetFocus(false, bRefresh))
			{
				OnChangeFocus(pCtrl->GetID(), INVALID_ID);
				return true;
			} 
			else
			{
				//ʧ��ԭ������Ǹ��Ի��򲻿���ʾ��������ı�״̬
				return false;
			}

		}
		else
		{
			return true;
		}
	}	
}


bool GUI::CWnd::CanSetFocus() const
{
	if (IsSupportFocus())
	{
		return (IsShow() && m_bEnable);
	} 
	else
	{
		return false;
	}
}

bool GUI::CWnd::IsSupportFocus() const
{
	if (m_style & WND_CHILD)
	{
		if (m_style & WND_DISABLE_FOCUS)
		{
			return false;
		}
		else
		{
			return true;
		}
	}
	else
	{
		return false;
	}
}

void GUI::CWnd::CalculateFocusCount(int& x, int & y) const
{
	int xFocus ,yFocus;
	const CWnd* pCtrl = NULL;
	WND_LIST::const_iterator iter;
	for (iter = m_childList.begin(); iter != m_childList.end(); ++iter)
	{
		pCtrl = *iter;

		if ( pCtrl->IsSupportFocus() )
		{
			pCtrl->GetFocusPos(xFocus, yFocus);

			if ( xFocus >= x )
			{
				x = xFocus + 1;
			}

			if ( yFocus >= y )
			{
				y = yFocus + 1;
			}
		}
	}
}
/***********************************************************
��������  CreateFocusMap
�������������������ͼ��
����������ޣ�
����������ޣ�
����ֵ��  �ޣ�
˵����    ��1�����������ͼ��Ŀ����Ϊ�˴������ƶ����⣻
��2�����Ի����в�����֧�ֽ���Ŀؼ��� �����ͼ��m_pFocusMap��Ϊ��(NULL)��
�Һ����곤�ȣ�m_hCtrlNum���������곤�ȣ�m_vCtrlNum����Ϊ0��
��3������ɡ��ؼ�����Ի��򡱵Ĳ����󣬵��ô˺������������ͼ��

������/�޸���                   ����/�޸�����                  ��ע
��չ                            28/08/07                       ����
***********************************************************/
bool GUI::CWnd::CreateFocusMap()
{
	if (NULL != m_pFocusMap)
	{
		delete [] m_pFocusMap;
		m_pFocusMap = NULL;
	}

	m_hCtrlNum = 0;
	m_vCtrlNum = 0;

	//���㽹���ͼ�ĳ�����֧�ֽ���Ŀؼ��������뽹���ͼ��
	CalculateFocusCount(m_hCtrlNum, m_vCtrlNum);

	//�����֧�ֽ���Ŀؼ�, ���������ͼ
	if ( (m_vCtrlNum>0) && (m_hCtrlNum>0) ) 
	{	
		unsigned long size = m_vCtrlNum*m_hCtrlNum;
		m_pFocusMap = new CWnd* [size];
		memset(m_pFocusMap, 0, size*sizeof(CWnd*));//����ĳ�����ϵĿؼ�ָ���Ƿ�Ϊ�գ����жϴ��������Ƿ��пؼ�

		CWnd* pCtrl = NULL;
		int xFocus, yFocus;

		WND_LIST::iterator iter;
		for (iter = m_childList.begin(); iter != m_childList.end(); ++iter)
		{
			pCtrl = *iter;
			if ( pCtrl->IsSupportFocus() )
			{		
				pCtrl->GetFocusPos(xFocus, yFocus);
				assert( (xFocus<m_hCtrlNum) && (yFocus<m_vCtrlNum) );

				m_pFocusMap[ (yFocus * m_hCtrlNum) + xFocus ] = pCtrl;
			}
		}

		//�Ľ�һ��ռ��񣬽����޷��ƶ������ 20080613 hz
		/*for (int y = 0; y < m_vCtrlNum; ++y)
		{
		for (int x = 1; x < m_hCtrlNum; ++x)
		{
		if (NULL == m_pFocusMap[ (y * m_hCtrlNum) + x ])
		{
		m_pFocusMap[ (y * m_hCtrlNum) + x ] = m_pFocusMap[ (y * m_hCtrlNum) + x - 1];
		}
		}
		}*/
		return true;
	}
	else
	{
		return false;
	}
}

bool GUI::CWnd::SetFocus(bool bFocus, bool bRefresh /* = true */)
{
	if (CanSetFocus())
	{
		if (m_bFocus != bFocus)
		{
			if (bFocus)
			{
				//�ȸı�״̬,��ˢ��
				m_bFocus = bFocus;

				CWnd* pWnd = GetDefaultFocusCtrl(m_defaultFocus);
				if (NULL != pWnd)
				{
					SetCtrlFocus(pWnd, true, false);
				}
				else
				{
					//ԭ���޸�����Ϊ�ų̿ؼ������������������˽��㣬���ǲ��ܱ仯������
					//�޸���Ϊ (NULL == m_pActiveCtrl)�����Ƿ������ֻ��ƻᵼ������Ƶ������У�
					//��������ػ棬Ч�ʲ��ߡ����ԸĻ�ȥ�����ǻ���ɴ��Ӵ���ؼ������ý���ʱ
					//����ˢ������������ͬԭ��
					if (0 == m_childList.size())
					//if (NULL == m_pActiveCtrl)
					{
						Repaint();
					}
				}

				//SetDefaultFocus(false);	
			} 
			else
			{	
				m_bFocus = bFocus;

				//��ȥ������ʱ��������ڴ���ӵ�н�����Ӵ��ڣ�ȥ���Ӵ��ڽ���
				if (NULL != m_pActiveCtrl)
				{
					SetCtrlFocus(m_pActiveCtrl, false, false);
				}
				else
				{
					if (0 == m_childList.size())
					//if (NULL == m_pActiveCtrl)
					{
						Repaint();
					}
				}
			}
		}		

		return true;
	} 
	else
	{
		//ʧ��ԭ������Ǹ��Ի��򲻿���ʾ��������ı�״̬
		if (!bFocus)
		{
			m_bFocus = false;
		}
		return false;
	}
}

/***********************************************************
������:OnChangeFocus
˵��:����仯,�������������ڻ�ȡ.�������ϼ�����
������Ϣ�¼�
***********************************************************/
void GUI::CWnd::OnChangeFocus(unsigned long oldID, unsigned long newID)
{
	GUI::CWnd* pWnd = GetParent();
	if( pWnd )
	{
		pWnd->OnChangeFocus(oldID,newID);
	}
}
