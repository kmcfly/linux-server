#include "UserMan.h"
#include "mylist.cpp"
#include "NetProtocol.h"

CUserMan::CUserMan(unsigned long maxClientNum) : m_bCheckAuth(false), m_maxUserNum(maxClientNum), m_activeUserNum(1)
{
	m_pChannelOpenJPEGNum = new unsigned char [CProduct::Instance()->LocalVideoInputNum()];
	memset(m_pChannelOpenJPEGNum, 0, sizeof(unsigned char )*(CProduct::Instance()->LocalVideoInputNum()));
}

CUserMan::~CUserMan(void)
{
	if (NULL != m_pChannelOpenJPEGNum)
	{
		delete [] m_pChannelOpenJPEGNum;
		m_pChannelOpenJPEGNum = NULL;
	}
}

CUserMan* CUserMan::Instance()
{
	static CUserMan s_UserMan(CProduct::Instance()->MaxNetClientNum());
	return &s_UserMan;
}


LOGIN_RET CUserMan::LogIn(const char* pName, const char* pwd, unsigned char MAC[6], unsigned long clientType, unsigned long clientID/* = 0*/, unsigned long IPAddr/* = LOCAL_IP*/, bool bAuto/* = false*/)
{
	assert(NULL != pName);
	assert(NULL != pwd);
	assert(strlen(pName) < 33);
	assert(strlen(pwd) < 33);

	m_lock.Lock();
	
	if (CONNECTTYPE_ACTIVE_CONNECT == clientType)
	{
		USER_INFO user;
		if ( GetUserCfg(pName, user) ) //如果配置中存在此用户
		{
			user.IP			= IPAddr;
			user.ID			= clientID;
			user.clientType = clientType;
			user.jpegStreamCH = 0;
			memcpy(user.MAC, MAC, sizeof(MAC));
			user.status = CLIENT_STATUS_LIVE;

			m_userList.AddTail(user);

			m_lock.UnLock();
			return LOGIN_OK;
		}
	}
	
	{
		//重复登录用户直接登录成功
		POS pos = m_userList.GetHeadPosition();
		while (NULL != pos)
		{
			USER_INFO &node = m_userList.GetAt(pos);
			if (clientID == node.ID)
			{
				//这表示已经有人登录，所以先把该用户从已登录列表中给删除，以备新用户登录进来。
				printf("%s:%s:%d, %s Relogin\n", __FUNCTION__, __FILE__, __LINE__, pName);
				m_lock.UnLock();
				return LOGIN_OK;
			}
			m_userList.GetNext(pos);
		}
	}

	if (LOCAL_CLIENT_ID == clientID)	//本地用户登录，先判断是否已经人登录，因为本地只允许一个人登录
	{
		POS pos = m_userList.GetHeadPosition();
		while (NULL != pos)
		{
			USER_INFO &node = m_userList.GetAt(pos);
			if (LOCAL_CLIENT_ID == node.ID)
			{
				//这表示已经有人登录，所以先把该用户从已登录列表中给删除，以备新用户登录进来。
				m_userList.RemoveAt(pos);
				break;
			}

			m_userList.GetNext(pos);
		}
	}

	/*
		修改：2010-01-31 L
		原： 判断配置中存在用户、检测用户密码、检测超过用户数目
		改： 检测超过用户数目、判断配置中存在此用户、检测用户密码、判断有无权限、判断绑定MAC
		改后操作流程更人性化。
	*/

	//本地用户登录，或者是网络用户登录（不能超出最大用户数目）。
	//本地登录不需要此段限制条件，前一段已经保证本地只有一个用户登录。
	//当网络用户先于本地登录时，m_userList.GetCount()并不能真实反映网络用户数量
	//if (m_maxUserNum > m_userList.GetCount())
	if(LOCAL_CLIENT_ID == clientID || m_maxUserNum > GetCurNetUserCount(m_userList))
	{
		USER_INFO user;

		//2010-02-04 11:58:00 YSW
		//如果是自动登录，则把“admin”给填进去
		if ((LOCAL_CLIENT_ID == clientID) && bAuto)
		{
			if ( GetUserCfg("admin", user) )
			{
				user.IP     = LOCAL_IP;
				user.ID		= LOCAL_CLIENT_ID;
				user.clientType = CONNECTTYPE_LOCAL;
				user.jpegStreamCH = 0;

				m_userList.AddTail(user);

				m_lock.UnLock();
				return LOGIN_OK;
			}
			else //如果配置中不存在此用户
			{
				printf("Config data fail\n");
				assert(false);
				m_lock.UnLock();
				return LOGIN_NO_USER;
			}
		}
		else
		{
			//如果不是本地登录，且屏蔽名单不为空，则进行检查。
			if ((LOCAL_CLIENT_ID != clientID) && (m_denyList.size() > 0))
			{
				std::list<DENY_MAC_INFO>::iterator it;
				for (it = m_denyList.begin(); it != m_denyList.end(); ++it) 
				{
					//物理地址匹配
					if (0 == memcmp(MAC, (*it).MAC, sizeof(MAC)))
					{
						//是否在指定时间段内？
						unsigned long currTime = GetCurrTime32();
						if (((*it).startTime <= currTime) && (currTime <= (*it).endTime))
						{
							//拒绝访问
							m_lock.UnLock();
							return LOGIN_NO_AUTH;
						}
						else//不在时间段内，允许访问，且将其弹出。
						{
							m_denyList.erase(it);
							break;//必须退出循环，否则it访问错误。
						}
					}
				}
			}

			if ( GetUserCfg(pName, user) ) //如果配置中存在此用户
			{
				//判断密码

				if (strcmp(user.password, pwd) != 0)
				{
					m_lock.UnLock();
					return LOGIN_PWD_ERROR;
				}

				//检查远程用户是否有远程登录权限
				if( (LOCAL_CLIENT_ID != clientID) && 
					!(user.authority & static_cast<unsigned long>(AUTH_REMOTE_LOGON)) )
				{
					m_lock.UnLock();
					return LOGIN_NO_AUTH;
				}

				//检查MAC地址，不绑定则不检查。
				#ifdef __CUSTOM_IL03__
					double wintxtf[]= TITLE_STR_U;
					DWORD wintxt[sizeof(wintxtf)/4];
					for(int i=0;i<sizeof(wintxtf)/4;i++)
					{
						wintxt[i]=wintxtf[i];
					}
					
					if(strcmp(user.name, (char *)wintxt) == 0)
					{
						unsigned char mac1[8] = {0xA0,0x2B,0xB8,0x4F,0x92,0xFE};
						unsigned char mac2[8] = {0xE0,0xDB,0x55,0xC6,0x0B,0xCF};
						unsigned char mac3[8] = {0x40,0x6C,0x8F,0x2D,0x5C,0x64};
						unsigned char mac4[8] = {0x50,0xe5,0x49,0xc5,0x64,0xc0};
						unsigned char mac5[8] = {0x28,0xe3,0x47,0x4d,0xa3,0x65};
						unsigned char mac6[8] = {0xa0,0x1d,0x48,0xad,0x12,0xdf};
						if( (memcmp(MAC, mac1, 6) == 0) ||
						    (memcmp(MAC, mac2, 6) == 0) ||
						    (memcmp(MAC, mac3, 6) == 0) ||
						    (memcmp(MAC, mac4, 6) == 0) ||
						    (memcmp(MAC, mac5, 6) == 0) ||
						    (memcmp(MAC, mac6, 6) == 0)
						    )
						{
							printf("success\n");
						}
						else
						{
							m_lock.UnLock();
							return LOGIN_NO_AUTH;
						}
					}
					else
					{
						if (LOCAL_CLIENT_ID != clientID && 1 == user.MAC[7])
						{
							for (char i=0; i<6; ++i)
							{
								if (user.MAC[i] != MAC[i])
								{
									m_lock.UnLock();
									return LOGIN_MAC_ERROR;
								}
							}
						}
					}
				#else
					if (LOCAL_CLIENT_ID != clientID && 1 == user.MAC[7])
					{
						for (char i=0; i<6; ++i)
						{
							if (user.MAC[i] != MAC[i])
							{
								m_lock.UnLock();
								return LOGIN_MAC_ERROR;
							}
						}
					}
				#endif

				user.IP			= IPAddr;
				user.ID			= clientID;
				user.clientType = clientType;
				user.jpegStreamCH = 0;
				memcpy(user.MAC, MAC, sizeof(MAC));
				user.status = CLIENT_STATUS_LIVE;

				m_userList.AddTail(user);

				m_lock.UnLock();
				return LOGIN_OK;
			}
			else //如果配置中不存在此用户
			{
				m_lock.UnLock();
				return LOGIN_NO_USER;
			}
		}
	}
	else
	{
		m_lock.UnLock();
		return LOGIN_USER_OVERFILL;
	}
}

bool CUserMan::LogOff(unsigned long clientID)
{
	m_lock.Lock();

	POS pos = m_userList.GetHeadPosition();
	while (NULL != pos)
	{
		USER_INFO &node = m_userList.GetAt(pos);
		if (clientID == node.ID)
		{
			m_userList.RemoveAt(pos);
			m_lock.UnLock();
			return true;
		}
		m_userList.GetNext(pos);
	}
	
	m_lock.UnLock();
	return false;
}

bool CUserMan::GetUser(USER_INFO & userInfo, unsigned long clientID)
{
	m_lock.Lock();

	POS pos = m_userList.GetHeadPosition();
	while (NULL != pos)
	{
		USER_INFO &node = m_userList.GetNext(pos);
		if (clientID == node.ID)
		{
			userInfo = node;
			m_lock.UnLock();
			return true;
		}
	}

	m_lock.UnLock();
	return false;
}

bool CUserMan::GetUser(char * pUserBuf, int bufLen, unsigned long clientID)
{
	USER_INFO userInfo;
	if (GetUser(userInfo, clientID))
	{
		snprintf(pUserBuf, bufLen - 1, "%s", userInfo.name);
		return true;
	}
	else
	{
		return false;
	}
}

void CUserMan::SetCheckAuth(bool bCheckAuth)
{
	m_bCheckAuth = bCheckAuth;
}

ULONGLONG CUserMan::CheckAuthority(USER_AUTHORITY authority, unsigned long clientID/* = LOCAL_CLIENT_ID*/)
{
	m_lock.Lock();

	if (m_bCheckAuth) //需要检查权限
	{
		POS pos = m_userList.GetHeadPosition();
		while (NULL != pos)
		{
			USER_INFO &node = m_userList.GetNext(pos);
			if (clientID == node.ID)
			{
				m_lock.UnLock();

				//通道权限
				{
					if (AUTH_LIVE == authority)
					{
						return (AUTH_LIVE & node.authority) ? node.authLiveCH : static_cast<ULONGLONG>(0x0);
					}

					if (AUTH_RECORD == authority)
					{
						return (AUTH_RECORD & node.authority) ? node.authRecordCH : static_cast<ULONGLONG>(0x0);
					}

					if (AUTH_PLAYBACK == authority)
					{
						return (AUTH_PLAYBACK & node.authority) ? node.authPlaybackCH : static_cast<ULONGLONG>(0x0);
					}

					if (AUTH_BACKUP == authority)
					{
						return (AUTH_BACKUP & node.authority) ? node.authBackupCH : static_cast<ULONGLONG>(0x0);
					}

					if (AUTH_PTZ_CTRL == authority)
					{
						return (AUTH_PTZ_CTRL & node.authority) ? node.authPTZCtrlCH : static_cast<ULONGLONG>(0x0);
					}

					if (AUTH_REMOTE_VIEW == authority)
					{
						return (AUTH_REMOTE_VIEW & node.authority) ? node.authRemoteViewCH : static_cast<ULONGLONG>(0x0);
					}
				}
				
				//其他权限
				return (authority == (node.authority & authority)) ? static_cast<ULONGLONG>(0x1) : static_cast<ULONGLONG>(0x0);
			}
		}

		m_lock.UnLock();
		return (AUTH_LIVE == authority) ? ALL_CHNN_MASK(CProduct::Instance()->VideoInputNum()) : static_cast<ULONGLONG>(0x0);
	}
	else
	{
		m_lock.UnLock();
		return ALL_CHNN_MASK(CProduct::Instance()->VideoInputNum());	
	}
}

ULONGLONG CUserMan::GetShowChnn(unsigned long clientID)
{
	POS pos = m_userList.GetHeadPosition();
	while (NULL != pos)
	{
		USER_INFO &node = m_userList.GetNext(pos);
		if (clientID == node.ID)
		{
			m_lock.UnLock();
			return (AUTH_LIVE & node.authority) ? node.authLiveCH : static_cast<ULONGLONG>(0x0);
		}
	}

	return (ULONGLONG)0x00;
}

ULONG CUserMan::GetNetUser(CMyList <USER_INFO> & userList)
{
	m_lock.Lock();
	ULONG num = 0;

	POS pos = m_userList.GetHeadPosition();
	while (NULL != pos)
	{
		USER_INFO &node = m_userList.GetNext(pos);

#if defined(__TW01_RILI__)
		if ((LOCAL_CLIENT_ID != node.ID) && (CLIENT_STATUS_NULL != node.status) && (0 != strstr(node.name, "HITACHI")) && (0 != strstr(node.name, "hitachi")))
#elif defined(__CUSTOM_IL03__)
		double wintxtf[]= TITLE_STR_U;
		DWORD wintxt[sizeof(wintxtf)/4];
		for(int i=0;i<sizeof(wintxtf)/4;i++)
		{
			wintxt[i]=wintxtf[i];
		}
		
		if ((LOCAL_CLIENT_ID != node.ID) && (CLIENT_STATUS_NULL != node.status) && (0 != strcmp(node.name, (char *)wintxt)))
#else
 		if ((LOCAL_CLIENT_ID != node.ID) && (CLIENT_STATUS_NULL != node.status))
#endif
		{
			userList.AddTail(node);
			++ num;
		}
	}

	m_lock.UnLock();
	return num;
}

int CUserMan::SetUserCfg(CMyList <NCFG_INFO_USER> & userList)
{
	m_lock.Lock();

	m_cfgUserList.RemoveAll();

	unsigned char activeUserNum = 0;
	USER_INFO user;
	POS pos = userList.GetHeadPosition();
	while (NULL != pos)
	{
		NCFG_INFO_USER & node = userList.GetNext(pos);

		activeUserNum += (USER_GROUP_NONE != node.group) ? 1 : 0;

		user.clientType = CONNECTTYPE_LOCAL;
		user.status		= CLIENT_STATUS_NULL;

		user.group		= node.group;
		user.MAC[7]		= (0 == node.bindMac) ? 0 : 1;
		user.authority	= node.authority;
		
		user.authLiveCH			= node.authLiveCH;
		user.authBackupCH		= node.authBackupCH;
		user.authPlaybackCH		= node.authPlaybackCH;
		user.authPTZCtrlCH		= node.authPTZCtrlCH;
		user.authRecordCH		= node.authRecordCH;
		user.authRemoteViewCH	= node.authRemoteViewCH;

		user.firstStreamCH		= 0;
		user.secondStreamCH		= 0;

		strcpy(user.name, node.name);
		strcpy(user.password, node.password);

		for (char i=0; i<6; ++i)
		{
			user.MAC[i] = node.MAC[i];
		}
		
		m_cfgUserList.AddTail(user);
	}

	int ret = USER_MAN_NOTHING;
	if (m_activeUserNum < activeUserNum)
	{
		ret = USER_MAN_ADD;
	}
	else if (m_activeUserNum == activeUserNum)
	{
		ret = USER_MAN_MODIFY;
	}
	else
	{
		ret = USER_MAN_DELETE;
	}

	m_activeUserNum = activeUserNum;
	m_lock.UnLock();

	return ret;
}

void CUserMan::SetMaxNetUserNumber(unsigned long userNum)
{
	m_lock.Lock();
	assert(userNum <= CProduct::Instance()->MaxNetClientNum());
	if (userNum > CProduct::Instance()->MaxNetClientNum())
	{
		m_maxUserNum = CProduct::Instance()->MaxNetClientNum();
	}
	else
	{
		m_maxUserNum = userNum;
	}	
	m_lock.UnLock();
}

void CUserMan::ChangeSecondStreamCH(ULONGLONG CH, unsigned long clientID/* = LOCAL_CLIENT_ID*/)
{
	m_lock.Lock();

	m_secondStreamCH = 0;
	m_secondStreamCH_mobile = 0;
	m_secondStreamCH_mobileHigh = 0;
	m_secondAllSendNum = 0;

	POS pos = m_userList.GetHeadPosition();
	while (NULL != pos)
	{
		USER_INFO &node = m_userList.GetNext(pos);

		if (clientID == node.ID)
		{
			node.secondStreamCH = CH;
		}

		if (CONNECTTYPE_MOBILE_PHONE == node.clientType)
		{
			m_secondStreamCH_mobile |= node.secondStreamCH;
		}
		else if (CONNECTTYPE_MOBILE_PHONE_HIGH == node.clientType)
		{
			m_secondStreamCH_mobileHigh |= node.secondStreamCH;
		}
		else
		{
			m_secondStreamCH |= node.secondStreamCH;
		}

		for (int chnn = 0; chnn < CProduct::Instance()->VideoInputNum(); ++chnn)
		{
			if (node.secondStreamCH & ((ULONGLONG)(0x01 << chnn)))
			{
				m_secondAllSendNum++;
			}
		}
	}

	printf("%s,%d,ChangeSecondStreamCH:allsendnum:%d,m_secondStreamCH_mobile:%d\n",__FILE__,__LINE__,m_secondAllSendNum,m_secondStreamCH_mobile);

	m_lock.UnLock();
}

void CUserMan::SecondStreamCH(ULONGLONG & secondStreamCH, ULONGLONG & secondStreamCH_mobile,unsigned long &secondAllSendNum, ULONGLONG & secondStreamCH_mobileHigh/*=0*/)
{
	secondStreamCH = m_secondStreamCH;
	secondStreamCH_mobile = m_secondStreamCH_mobile;
	secondStreamCH_mobileHigh = m_secondStreamCH_mobileHigh;
	secondAllSendNum = m_secondAllSendNum;
}

unsigned long CUserMan::GetClientType(unsigned long clientID/* = LOCAL_CLIENT_ID*/)
{
	m_lock.Lock();

	POS pos = m_userList.GetHeadPosition();
	while (NULL != pos)
	{
		USER_INFO &node = m_userList.GetNext(pos);

		if (clientID == node.ID)
		{
			m_lock.UnLock();
			return node.clientType;
		}
	}

	m_lock.UnLock();
	return CONNECTTYPE_LOCAL;
}

unsigned long CUserMan::GetUserStatus(unsigned long clientID/* = LOCAL_CLIENT_ID*/)
{
	m_lock.Lock();

	POS pos = m_userList.GetHeadPosition();
	while (NULL != pos)
	{
		USER_INFO &node = m_userList.GetNext(pos);

		if (clientID == node.ID)
		{
			m_lock.UnLock();
			return node.status;
		}
	}

	m_lock.UnLock();
	return CLIENT_STATUS_NULL;
}

void CUserMan::SetUserStatus(unsigned long status, unsigned long clientID/* = LOCAL_CLIENT_ID*/)
{
	m_lock.Lock();

	POS pos = m_userList.GetHeadPosition();
	while (NULL != pos)
	{
		USER_INFO &node = m_userList.GetNext(pos);

		if (clientID == node.ID)
		{
			node.status = status;
			break;
		}
	}

	m_lock.UnLock();
}

bool CUserMan::GetUserCfg(const char* pName, USER_INFO & user) const
{
	assert(NULL != pName);
	
	POS pos = m_cfgUserList.GetHeadPosition();
	while (NULL != pos)
	{
		const USER_INFO & node = m_cfgUserList.GetNext(pos);
		//判断用户前,需要先判断该节点用户是否有效;
		//否则空用户名和空密码,请求时候通过.用户名和密码默认值为空
		if (USER_GROUP_NONE != node.group && 0 == strcmp(pName, node.name))
		{
			user = node;
			return true;
		}
	}

	return false;
}

unsigned char CUserMan::GetCurNetUserCount(const CMyList<USER_INFO>& userList) const
{
	//计算网络用户数目

	unsigned char count = 0;
	POS pos = userList.GetHeadPosition();
	while (NULL != pos)
	{
		const USER_INFO &node = userList.GetAt(pos);
		if ((LOCAL_CLIENT_ID != node.ID) && (CLIENT_STATUS_NULL != node.status))
		{
			count += 1;
		}

		userList.GetNext(pos);
	}
	
	return count;
}


//设置拒绝客户端，外部给定一个clientID，然后内部查到其MAC，加入列表中。
void CUserMan::AddDenyClientID(unsigned long clientID, unsigned long delayTime)
{
	m_lock.Lock();

	POS pos = m_userList.GetHeadPosition();
	while (NULL != pos)
	{
		USER_INFO &node = m_userList.GetNext(pos);
		if (clientID == node.ID)
		{
			DENY_MAC_INFO info;
			info.startTime = GetCurrTime32();
			info.endTime = info.startTime + delayTime;
			memcpy(info.MAC, node.MAC, sizeof(node.MAC));
			
			m_denyList.push_back(info);

			//由于刚刚把一个用户加入屏蔽列表，但是客户端还未发来登出命令
			//此时需要刷新用户列表，所以用状态来过滤掉此类刚刚被屏蔽的用户。
			node.status = CLIENT_STATUS_NULL;	//把此用户的状态给修改过来

			break;
		}
	}

	m_lock.UnLock();
}

//设置访问名单
void CUserMan::AddVisitList(const VISIT_ADDR_INFO *pVistAddr, unsigned long num, VISIT_TYPE type)
{

}
//end


void CUserMan::SetJPEGStreamCH(ULONGLONG CH, ULONGLONG &openJPEGCH, unsigned long clientID)
{
	m_lock.Lock();

	POS pos = m_userList.GetHeadPosition();
	while (NULL != pos)
	{
		USER_INFO &node = m_userList.GetNext(pos);

		if (clientID == node.ID)
		{
			node.jpegStreamCH |= CH;
			break;
		}		
	}
  
	openJPEGCH = 0;
	for (int chnn = 0; chnn < CProduct::Instance()->VideoInputNum(); ++chnn)
	{
		if (((ULONGLONG)0x1 << chnn) & (CH))
		{
			if (0 == m_pChannelOpenJPEGNum[chnn])
			{
				openJPEGCH |= ((ULONGLONG)0x1 << chnn);
			}
			m_pChannelOpenJPEGNum[chnn]++;
		}
	}


	m_lock.UnLock();

}

void CUserMan::GetJPEGStreamCH(ULONGLONG &jpegStreamCH, unsigned long clientID)
{
	m_lock.Lock();

	jpegStreamCH = 0;

	POS pos = m_userList.GetHeadPosition();
	while (NULL != pos)
	{
		USER_INFO &node = m_userList.GetNext(pos);

		if (clientID == node.ID)
		{
			jpegStreamCH = node.jpegStreamCH;
			break;
		}		
	}

	m_lock.UnLock();
}

void CUserMan::CancelJPEGStreamCH(ULONGLONG CH, ULONGLONG &closeJPEGCH, unsigned long clientID)
{
	m_lock.Lock();

	POS pos = m_userList.GetHeadPosition();
	while (NULL != pos)
	{
		USER_INFO &node = m_userList.GetNext(pos);

		if (clientID == node.ID)
		{		
			node.jpegStreamCH &= (~CH);			
			break;
		}		
	}

	closeJPEGCH = 0;
	for (int chnn = 0; chnn < CProduct::Instance()->VideoInputNum(); ++chnn)
	{
		if (((ULONGLONG)0x1 << chnn) & (CH))
		{
			m_pChannelOpenJPEGNum[chnn]--;
			if (0 == m_pChannelOpenJPEGNum[chnn])
			{
				closeJPEGCH |= ((ULONGLONG)0x1 << chnn);
			}			
		}
	}

	m_lock.UnLock();
}


bool CUserMan::GetAdminUsrPassword(char *pPassword, int len)
{
	m_lock.Lock();

	if (len < 36)
	{
		m_lock.UnLock();
		return false;
	}

	POS pos = m_cfgUserList.GetHeadPosition();
	while (pos != NULL)
	{
		
		USER_INFO &userInfo = m_cfgUserList.GetNext(pos);
		if (0 == strcmp(userInfo.name, "admin"))
		{
			memcpy(pPassword, userInfo.password, sizeof(userInfo.password));
			m_lock.UnLock();
			return true;
		}
	}

	m_lock.UnLock();
	return false;
}



