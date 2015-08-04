
#include "OnvifDevDiscoverServ.h"

#ifndef WIN32
#include <sys/syscall.h>
#endif

//#define  ONVIF_DISCOVERY_DEBUG

//#define ONVIF_SEARCH_METHO_SECOND 1

char *onvifDiscoverWsaTo	="urn:schemas-xmlsoap-org:ws:2005:04:discovery";
char *onvifDiscoverActionTo	="http://schemas.xmlsoap.org/ws/2005/04/discovery/Probe";

COnvifDevDiscover::COnvifDevDiscover()
{
	//m_totalDevNum = 0;
	m_threadID = PUB_THREAD_ID_NOINIT;
	m_bDiscovering = false;
    
    m_pSoap = NULL;

	m_pTmp=NULL;
	memset(m_uuid,'\0',sizeof(m_uuid));
	m_pDevInfo = NULL;
    memset(&m_devInfoBuffer,0,sizeof(struct OnvifDisDevInfo));

	m_pNewHead =NULL;
	m_pOldHead =NULL;

	m_iStartSearchCount = 0;

	m_bHasMustSearchEth1Dev = true;

}

COnvifDevDiscover::~COnvifDevDiscover()
{
    //m_totalDevNum = 0; 
	if(m_pSoap != NULL)
	{
		onvif_free_soap(m_pSoap);
	}
	m_pSoap = NULL;
	m_pTmp=NULL;
	if(m_pDevInfo != NULL)
	{
		delete m_pDevInfo;
	}
    m_pDevInfo = NULL;

	m_threadID = PUB_THREAD_ID_NOINIT;
	m_bDiscovering = false;
	
	FreeDeviceInfo(m_pOldHead);
	FreeDeviceInfo(m_pNewHead);

	//m_updataEvent.Close();
}

COnvifDevDiscover* COnvifDevDiscover::Instance(void)
{
  static COnvifDevDiscover discover;
  return &discover;
}

//开始搜索
void COnvifDevDiscover::StartSearch(void)
{
    m_threadID = PUB_CreateThread(DeviceDiscovery,this,&m_bDiscovering);
	if(m_threadID == PUB_CREATE_THREAD_FAIL)
	{
		return;
	}

}

//停止搜索
void COnvifDevDiscover::StopSearch(void)
{
   PUB_ExitThread(&m_threadID,&m_bDiscovering);
}

//function
//获取搜索到的设备信息。设备保存在设备列表中
//input:保存的设备列表
//deviceNum:获取的设备数目
//return:保存到设备信息表中的设备个数
int COnvifDevDiscover::GetDeviceInfo(nd_in ND_DEVICE_INFO* p_device_info, nd_in int deviceNum, bool bGetIpcInfo)
{
#ifdef ONVIF_SEARCH_METHO_SECOND

	ND_DEVICE_INFO* tmpInfo = NULL;
	std::list<ONVIF_SECOND_DEV_INFO>::iterator it;
	int getDevNum = 0;
	char *tmp1= NULL;
	char *tmp2=NULL;

	if( NULL == p_device_info || 0 == deviceNum )
	{
		m_iStartSearchCount = 2;
		return 0;
	}

	if( bGetIpcInfo )
	{
		ONVIF_SECOND_DEV_INFO stOnvifInfo;
		p_device_info->manufacturerId = MANU_ONVIF_NORMAL;

		m_lock.Lock();
		if( SecondRegetIpcProdInfo( (const unsigned char*)p_device_info->szAddr,stOnvifInfo ) )
		{
			p_device_info->manufacturerId = stOnvifInfo.manu_id_val;
			strcpy( p_device_info->szID, stOnvifInfo.endPointReference );
			strcpy( p_device_info->productModel, stOnvifInfo.Manufacturer );
		}
		m_lock.UnLock();

		return 0;
	}

	m_lock.Lock();

	tmpInfo = p_device_info;
	for (it=m_listOnvifDevice.begin(); it!=m_listOnvifDevice.end(); ++it)
	{
		//if( it->active )
		{
			if( getDevNum < deviceNum )
			{
				tmp1 = it->Addr;
				if( strstr(it->Addr,"http://") )
				{
					tmp1 += 7;
				}
				tmp2 = strstr(tmp1,"/onvif");
				if(tmp2 != NULL)
				{
					memset(tmpInfo->szAddr,0,sizeof(tmpInfo->szAddr));
					strncpy(tmpInfo->szAddr,tmp1,tmp2-tmp1);
				}

				memset( tmpInfo->szID, 0, sizeof(tmpInfo->szID) );
				memcpy(tmpInfo->szID,it->endPointReference, 16);
				//strcpy(tmpInfo->szID,it->endPointReference);

				strcpy(tmpInfo->szEther, it->szEther);

				tmpInfo->productModel[35] = it->manu_id_val;

				strcpy(tmpInfo->manufacturerName, "ONVIF");
				
				tmpInfo++;
				getDevNum++;

			}
			else
			{
				break;
			}
		}
	}

	m_lock.UnLock();

	return getDevNum;

#else // ONVIF_SEARCH_METHO_SECOND
    int getDevNum = 0;
	int i = 0;
	struct OnvifDisDevInfo *tmpDev=NULL;
    ND_DEVICE_INFO* tmpInfo = NULL;
	char *tmp1= NULL;
	char *tmp2=NULL;

	if( NULL == p_device_info || 0 == deviceNum )
	{
		return 0;
	}

    m_lock.Lock();
    tmpDev= m_pOldHead; 
	tmpInfo = p_device_info;
	while(i < deviceNum)
	{
		if(tmpDev == NULL)
		{
			break;
		}
		else
		{        
			 tmp1 = tmpDev->Addr;
			 tmp2 = strstr(tmp1,"/onvif");
			 if(tmp2 != NULL)
			 {
				 memset(tmpInfo->szAddr,0,sizeof(tmpInfo->szAddr));
				 strncpy(tmpInfo->szAddr,tmp1,tmp2-tmp1);
			 }

			 memset(tmpInfo->szID,0,sizeof(tmpInfo->szID));
			 if( strlen(tmpDev->endPointReference) > 54 )
			 {
				 strncpy( tmpInfo->szID, tmpDev->endPointReference,54 );
			 }
			 else
			 {
				 strcpy(tmpInfo->szID,tmpDev->endPointReference);
			 }

			 char prod_id[16];

			 if( 0 )
			 {
				 memset( prod_id, 0, 16 );
				 snprintf( prod_id, 16, "SZ%04dSZ", tmpDev->manu_id_val );
				 strcat( tmpInfo->szID, prod_id );
			 }

			 strcpy(tmpInfo->szEther, tmpDev->szEther);
		}
		tmpDev= tmpDev->next;
		tmpInfo++;
		i++;
	}
	getDevNum = i;
	m_lock.UnLock();

	tmp1= NULL;
	tmp2=NULL;

	return getDevNum;

#endif //ONVIF_SEARCH_METHO_SECOND

}

int COnvifDevDiscover::GetDeviceNum(void)
{
   int devNum = 0; 
	
#ifdef ONVIF_SEARCH_METHO_SECOND

    ULONGLONG ullCurTime = (ULONGLONG)GetCurrTime();
	SecondDeleteTimerOld(ullCurTime);
	SecondAllEthDeviceDiscover();

	devNum = m_listOnvifDevice.size();


#else
	m_lock.Lock();
	devNum = GetListNodeNum(m_pOldHead);
	m_lock.UnLock();
#endif

	return devNum;
}

int COnvifDevDiscover::DiscoveryStart(const char *soap_endpoint, const char *soap_action, struct d__ProbeType *d__Probe, const char *pEther)
{
	struct __dndl__Probe soap_tmp___dndl__Probe;

	if (soap_action == NULL)
	{
		soap_action = "http://www.onvif.org/ver10/network/wsdl/Probe";
	}
	m_pSoap->encodingStyle = NULL;
	soap_tmp___dndl__Probe.d__Probe = d__Probe;
	soap_begin(m_pSoap);

	soap_serializeheader(m_pSoap);

	soap_serialize___dndl__Probe(m_pSoap, &soap_tmp___dndl__Probe);
	if (soap_begin_count(m_pSoap))
	{
		return m_pSoap->error;
	}

	if (m_pSoap->mode & SOAP_IO_LENGTH)
	{	
		if (soap_envelope_begin_out(m_pSoap)
			|| soap_putheader(m_pSoap)
			|| soap_body_begin_out(m_pSoap)
			|| soap_put___dndl__Probe(m_pSoap, &soap_tmp___dndl__Probe, "-dndl:Probe", NULL)
			|| soap_body_end_out(m_pSoap)
			|| soap_envelope_end_out(m_pSoap))
			return m_pSoap->error;
	}
	if (soap_end_count(m_pSoap))
	{
		return m_pSoap->error;
	}
	if (soap_connect(m_pSoap, soap_endpoint, soap_action, pEther)
		|| soap_envelope_begin_out(m_pSoap)
		|| soap_putheader(m_pSoap)
		|| soap_body_begin_out(m_pSoap)
		|| soap_put___dndl__Probe(m_pSoap, &soap_tmp___dndl__Probe, "-dndl:Probe", NULL)
		|| soap_body_end_out(m_pSoap)
		|| soap_envelope_end_out(m_pSoap)
		|| soap_end_send(m_pSoap))
	{
		return soap_closesock(m_pSoap);
	}
	return SOAP_OK;
}

int COnvifDevDiscover::DiscoveryRecv(struct d__ProbeMatchesType *d__ProbeMatches)
{
	if (d__ProbeMatches == NULL)
	{
		soap_closesock(m_pSoap);
		return -1;
	}

	soap_default_d__ProbeMatchesType(m_pSoap, d__ProbeMatches);

	if (soap_begin_recv(m_pSoap)
		|| soap_envelope_begin_in(m_pSoap)
		|| soap_recv_header(m_pSoap)
		|| soap_body_begin_in(m_pSoap))
	{
		return soap_closesock(m_pSoap);
	}
	soap_get_d__ProbeMatchesType(m_pSoap, d__ProbeMatches, "d:ProbeMatches", "d:ProbeMatchesType");
	if (m_pSoap->error)
	{
		soap_recv_fault(m_pSoap, 0);
		return -1;
	}
	if (soap_body_end_in(m_pSoap)
		|| soap_envelope_end_in(m_pSoap)
		|| soap_end_recv(m_pSoap))
	{
		soap_closesock(m_pSoap);
		return -1;
	}
	return SOAP_OK;

}
int COnvifDevDiscover::DiscoveryStop(void)
{
	return soap_closesock(m_pSoap);
}

PUB_THREAD_RESULT PUB_THREAD_CALL COnvifDevDiscover::DeviceDiscovery(void *param)
{
#ifdef __ENVIRONMENT_LINUX__
	pid_t pid = getpid();
	pid_t tid = syscall(__NR_gettid);
	printf("%s, %d, pid = %d, tid=%d\n", __FILE__, __LINE__, pid, tid);
#endif
     COnvifDevDiscover *pThis = NULL;
	  pThis  =	 (COnvifDevDiscover *)param;
	  if(pThis == NULL)
	  {
		  return 0;
	  }
	  else
	  {
	     return pThis->DeviceDiscoveryProc();
	  }
}

PUB_THREAD_RESULT COnvifDevDiscover::DeviceDiscoveryProc(void)
{
	int result = -1;
	int i = 0;
	int failCount = 0;
	int product_id;

	char szEther[2][8];
	strcpy(szEther[0], MAIN_ETHER);
	strcpy(szEther[1], POE_ETHER);
	char serialNum[64];
	int  serialLen = 0;

	ULONGLONG ullCurTimes;
	int check_times = 0;

	while(m_bDiscovering)
	{
#ifdef ONVIF_SEARCH_METHO_SECOND

		++check_times;
		if( 0 == (check_times % 500))
		{
			check_times = 0;
			ullCurTimes = (ULONGLONG)GetCurrTime();
		}

		PUB_Sleep(2000);

		continue;
#endif
		result = -1;
		i = 0;
		m_pSoap = NULL;

		for(int ether = 0; ether < (m_bHasMustSearchEth1Dev ? 2 : 1); ether++)
		{
			memset(m_uuid,'\0',sizeof(m_uuid));
			onvif_soap_generate_uuid(m_uuid);

			m_pSoap = onvif_init_soap(TDEVICE_DISCOVERY,true);
			if(m_pSoap == NULL)
			{
				continue;
			}
		   memset(&m_header,0,sizeof(struct SOAP_ENV__Header));
		   m_header.wsa__MessageID = m_uuid;	
		   m_header.wsa__RelatesTo = NULL;	
		   m_header.wsa__From = NULL;	
		   m_header.wsa__ReplyTo = NULL;	
		   m_header.wsa__FaultTo =NULL;	
		   m_header.wsa__To = onvifDiscoverWsaTo;	
		   m_header.wsa__Action = onvifDiscoverActionTo;	
		   m_header.bDiscover = _true; 
		   m_pSoap->header = &m_header;

		   memset(&m_discoverReq,0,sizeof(struct d__ProbeType));

			//soap_default_d__ScopesType(m_pSoap, &sScope);
			//sScope.__item = "";

			//soap_default_d__ProbeType(m_pSoap, &req);
		   memset(&m_discoverScope,0,sizeof(struct d__ScopesType));
			m_discoverReq.Scopes = &m_discoverScope;
			m_discoverReq.Types = "dn:NetworkVideoTransmitter";

		   memset(&m_discoverResp,0,sizeof(struct d__ProbeMatchesType));

			result = DiscoveryStart("soap.udp://239.255.255.250:3702", NULL, &m_discoverReq, szEther[ether]);

			failCount = 0;
			while(result == SOAP_OK)
			{
				result = DiscoveryRecv(&m_discoverResp); 
				if(result==SOAP_OK)
				{
					failCount = 0;
					if (m_pSoap->error) 
					{ 
						result = m_pSoap->error; 
						break;
					} 
					else
					{
						if(m_discoverResp.__sizeProbeMatch == 0)
						{
							continue;
						}
						else
						{
							//some cases the reply data is more then 1
							i=0;
							while(i < m_discoverResp.__sizeProbeMatch)
							{
								if(m_discoverResp.ProbeMatch[i].XAddrs != NULL)
								{
									//if( 1 == ether )
									//{
									//	SetONVIFDeviceIPAddr( m_discoverResp.ProbeMatch[i].XAddrs);
									//}

									if(strstr(m_discoverResp.ProbeMatch[i].XAddrs,"onvif") != NULL)
									{
										
										memset(&m_devInfoBuffer,0,sizeof(struct OnvifDisDevInfo));			
										if(strstr(m_discoverResp.ProbeMatch[i].XAddrs,"http://"))
										{

											m_pTmp = m_discoverResp.ProbeMatch[i].XAddrs + 7;

											strcpy(m_devInfoBuffer.Addr,m_pTmp);

											m_pTmp = NULL;
										}
										else
										{
											printf("check not HTTP,copy ADDR!\n");
											strcpy(m_devInfoBuffer.Addr,m_discoverResp.ProbeMatch[i].XAddrs);
										}


										if(m_discoverResp.ProbeMatch[i].wsa__EndpointReference.Address != NULL)
										{
											if( strlen(m_discoverResp.ProbeMatch[i].wsa__EndpointReference.Address) > 62)
											{
												strncpy(m_devInfoBuffer.endPointReference,m_discoverResp.ProbeMatch[i].wsa__EndpointReference.Address, 62);
											}
											else
											{
												strcpy(m_devInfoBuffer.endPointReference,m_discoverResp.ProbeMatch[i].wsa__EndpointReference.Address);
											}
										}
										else
										{
											strcpy( m_devInfoBuffer.endPointReference, m_devInfoBuffer.Addr );
										}
									
										bool bTvtIpc = false;
										if(m_discoverResp.ProbeMatch[i].__size > 0)
										{
											if(m_discoverResp.ProbeMatch[i].__any != NULL)
											{
												if(strstr(m_discoverResp.ProbeMatch[i].__any[0],"TDigitalIPC") != NULL)
												{
													bTvtIpc = true;
													memset(m_devInfoBuffer.endPointReference,0,sizeof(m_devInfoBuffer.endPointReference));
													strcpy(m_devInfoBuffer.endPointReference,"0018aexxxxxx");
													//printf("discover TVT :%s\n",m_discoverResp.ProbeMatch[i].XAddrs);
												}
											}
										}

										//if(!bTvtIpc)
										if( 0 )
										{
											//Get device information
											memset(serialNum,0,sizeof(serialNum));
											serialLen = 0;
											product_id = MANU_ONVIF_NORMAL;
											GetDevSerial(m_discoverResp.ProbeMatch[i].XAddrs,serialNum,serialLen, product_id);
											if(12 == serialLen)
											{
												strlwr(serialNum);
												//匹配TVT的MAC地址段
												if((strncmp(serialNum, "0018ae", 6) == 0) || (strncmp(serialNum, "a81b189a", 6) == 0))
												{
													memset(m_devInfoBuffer.endPointReference,0,sizeof(m_devInfoBuffer.endPointReference));
													strcpy(m_devInfoBuffer.endPointReference,serialNum);
												}
											}
											else
											{
												//printf("******ONVIF device discover serialLen = %d \n",serialLen);
											}

											m_devInfoBuffer.manu_id_val = (unsigned int)product_id;
										}


										//memset(m_pDevInfo->type,'\0',sizeof(m_pDevInfo->type));
										//strcpy(m_pDevInfo->type,m_pDiscoverResp->ProbeMatch[i].Types);

										m_devInfoBuffer.active = true;
										m_devInfoBuffer.noActiveNum = 0;
										m_devInfoBuffer.next = NULL;

										if(GenNewDevList(&m_devInfoBuffer))
										{
											if(m_pDevInfo != NULL)
											{
												delete m_pDevInfo;
											}
											m_pDevInfo = NULL;
											m_pDevInfo = new OnvifDisDevInfo;
											assert(m_pDevInfo!= NULL);
											memset(m_pDevInfo,0,sizeof(OnvifDisDevInfo));
											m_pDevInfo->next = NULL;

											strcpy(m_pDevInfo->Addr,m_devInfoBuffer.Addr);
											strcpy(m_pDevInfo->endPointReference,m_devInfoBuffer.endPointReference);
											m_pDevInfo->active = m_devInfoBuffer.active;
											m_pDevInfo->noActiveNum = m_devInfoBuffer.noActiveNum;
											strcpy(m_pDevInfo->szEther, szEther[ether]);

											m_pDevInfo->manufacturerId = MID_ONVIF;
	                                    
											//add new ONVIF device
											m_pDevInfo->next = m_pNewHead;
											m_pNewHead  = m_pDevInfo;
											m_pDevInfo = NULL;
										}

										m_pDevInfo = NULL;
									} //if	this device is ONVIF 
								} //if addr is not NULL
	#ifdef ONVIF_DISCOVERY_DEBUG
								else
								{
									printf("@@@addr is NULL!\n");
								}
	#endif
								i++;
							} //while			
						}
					}
				}
				else
				{
					if (m_pSoap->error) 
					{  
						result = m_pSoap->error; 
					}

					failCount++;
					if(failCount > 5)
					{
						break;
					}

					PUB_Sleep(500);
				}
			} //while soap message

			DiscoveryStop();

			memset(&m_discoverResp,0,sizeof(struct d__ProbeMatchesType));    
			if(m_pSoap->header != NULL)
			{
				m_pSoap->header = NULL;
			}
			onvif_free_soap(m_pSoap);
			m_pSoap = NULL;
		}

		m_lock.Lock();
		SetDeviceNoActive();
		UpdateDevInfoList();
		DeleteNoActiveDevice();
		m_lock.UnLock();

		//m_updataEvent.SetEvent();
		PUB_Sleep(2000);
	}  //thread while()
	return 0;
}

void COnvifDevDiscover::UpdateDevInfoList(void)
{
	  struct OnvifDisDevInfo *oldCurrent= NULL;
	  struct OnvifDisDevInfo *newCurrent = NULL;

	  //newCurrent = m_pNewHead;

	  while(m_pNewHead != NULL)
	  {
		  oldCurrent = m_pOldHead;

		  newCurrent = m_pNewHead;
		  m_pNewHead = m_pNewHead->next;
		  newCurrent->next = NULL;
		  //search 
		  while(oldCurrent != NULL)
		  {
			  //modify old device information
			  if(0 == strcmp(oldCurrent->endPointReference,newCurrent->endPointReference)
				  || 0 == strncmp(oldCurrent->Addr,newCurrent->Addr, 15) )
			  {
				  strcpy(oldCurrent->Addr,newCurrent->Addr);
				  strcpy(oldCurrent->endPointReference,newCurrent->endPointReference);
				  oldCurrent->active = true;
			      oldCurrent->noActiveNum = 0;

				  //this device is exit in the last search,so delete it
				  if(newCurrent != NULL)
						delete newCurrent;
                  newCurrent = NULL;
				  break;
			  }
			  oldCurrent = oldCurrent->next;
		  } //end while old device list
	      
		  //add device in the laster device information
		  if(oldCurrent == NULL)
		  {
			  newCurrent->next = m_pOldHead;
			  m_pOldHead = newCurrent;
		  }

	      oldCurrent= NULL;
		  newCurrent = NULL;
	  }//end while,new device list

	  oldCurrent= NULL;
	  newCurrent = NULL;
	  m_pNewHead = NULL;
}

bool COnvifDevDiscover::GenNewDevList(struct OnvifDisDevInfo *newDev)
{
	struct OnvifDisDevInfo *pCurrent=NULL;

	if(newDev == NULL)
	{
		return false;
	}

	pCurrent = m_pNewHead;
	while(pCurrent != NULL)
	{
		if(0 == strcmp(newDev->endPointReference,pCurrent->endPointReference)
			|| 0 == strncmp(newDev->Addr,pCurrent->Addr,15) ) //equal
		{
			strcpy(pCurrent->Addr,newDev->Addr);
			strcpy(pCurrent->endPointReference, newDev->endPointReference);
			pCurrent->active = true;
			pCurrent->noActiveNum = 0;
			pCurrent = NULL;
			return false;
		}
		pCurrent = pCurrent->next;
	}

	return true;
}


void COnvifDevDiscover::SetDeviceNoActive(void)
{
	struct OnvifDisDevInfo *pCurrent = NULL;

	if(m_pOldHead == NULL)
	{
		//printf("m_pOldHead == NULL!\n");
		return;
	}
	pCurrent = m_pOldHead;
	while(pCurrent != NULL)
	{
		pCurrent->active = false;
		pCurrent=pCurrent->next;
	}
    pCurrent = NULL;

}

void COnvifDevDiscover::DeleteNoActiveDevice(void)
{
	OnvifDisDevInfo *pPreDev = NULL;
	OnvifDisDevInfo *pCurrent = NULL;
    
	if(m_pOldHead == NULL)
	{
		return;
	}
	else
	{
		pPreDev = m_pOldHead;
		pCurrent = m_pOldHead->next;
		while(pCurrent != NULL)
		{
			if(false == pCurrent->active)
			{
				if(pCurrent->noActiveNum < 3)
				{
					pCurrent->noActiveNum++;
					pPreDev = pPreDev->next;
				}
				else
				{
					pPreDev->next = pCurrent->next;
					pCurrent->next =NULL;
                  if(pCurrent != NULL)
						delete pCurrent;
					pCurrent =NULL;
				}
			}
			else
			{
				pPreDev = pPreDev->next;
			}
			pCurrent = pPreDev->next;
		}

		//head is no active
		if(false == m_pOldHead->active)
		{
			if(m_pOldHead->noActiveNum < 3)
			{
              m_pOldHead->noActiveNum++;
			}
			else
			{
				pCurrent = m_pOldHead;
				m_pOldHead = m_pOldHead->next;
				pCurrent->next = NULL;
				if(pCurrent != NULL)
					delete pCurrent;
				pCurrent = NULL;
			}
		}

		pCurrent = NULL;
		pPreDev =NULL;
	}
}

void COnvifDevDiscover::FreeDeviceInfo(OnvifDisDevInfo *aListHead)
{
   	OnvifDisDevInfo *pCurrent = NULL;
	if(aListHead != NULL)
	{
		pCurrent = aListHead->next;
		while(pCurrent != NULL)
		{
			aListHead->next = pCurrent->next;
			pCurrent->next = NULL;
			delete pCurrent;
			pCurrent = NULL;
			pCurrent = aListHead->next;
		}
		delete aListHead;
	}
    aListHead = NULL;
}

int  COnvifDevDiscover::GetListNodeNum(OnvifDisDevInfo *aListHead)
{
    int num = 0;
 	struct OnvifDisDevInfo *tmpDev=NULL;  

	if(aListHead != NULL)
	{
		tmpDev= aListHead;
		while(tmpDev != NULL)
		{
			num++;
			tmpDev = tmpDev->next;
		}    
	}
	tmpDev = NULL;
	return num;
}

int COnvifDevDiscover::GetDevSerial(char *servAddr,char *serialNum,int &serialLen, int &product_id)
{
	int result = -1;
	struct _tds__GetDeviceInformation information;
	struct _tds__GetDeviceInformationResponse response;

	enum  EResultType eResult = OTHER_FAULT;
	struct soap *pSoap = NULL;

	information.dummy = 0;
	if(servAddr == NULL)
	{
		return SERVER_ADDR_FAULT;
	}

	if(serialNum == NULL)
	{
		return PARAMETER_FAULT;
	}

	pSoap = onvif_init_soap(TDEVICE_MGT,false);
	if(pSoap == NULL)
	{
		return SOAP_INIT_FAULT;
	}

	result = soap_call___tds__GetDeviceInformation(pSoap, servAddr,NULL,&information,&response);
	if(SOAP_OK == result)
	{
		char ManufacStr[32] = {0};
		char ProdMode[32] = {0};
		char FirwareInfo[64] = {0};

		if( response.Manufacturer && response.Model )
		{
			strncpy( ManufacStr, response.Manufacturer, 30 );
			strncpy( ProdMode, response.Model, 30 );
			strncpy( FirwareInfo, response.FirmwareVersion, 60 );

			product_id = GetIPCProductIDNum( ManufacStr, ProdMode, FirwareInfo );
		}

		if(response.SerialNumber != NULL)
		{
			strcpy(serialNum,response.SerialNumber);
			serialLen = strlen(response.SerialNumber);
		}
		eResult = OPERATE_SUCCESS;
	}
	else
	{
		eResult = OPERATE_FAILED;
	}

	onvif_free_soap(pSoap);
	pSoap =NULL;
	return eResult;
}


int COnvifDevDiscover::SecondDiscoveryStart(struct soap *soap, const char *soap_endpoint, const char *soap_action, struct d__ProbeType *d__Probe, const char *pEther)
{
	struct __dndl__Probe soap_tmp___dndl__Probe;

	if (soap_action == NULL)
	{
		soap_action = "http://www.onvif.org/ver10/network/wsdl/Probe";
	}
	soap->encodingStyle = NULL;
	soap_tmp___dndl__Probe.d__Probe = d__Probe;
	soap_begin(soap);

	soap_serializeheader(soap);

	soap_serialize___dndl__Probe(soap, &soap_tmp___dndl__Probe);
	if (soap_begin_count(soap))
	{
		return soap->error;
	}

	if (soap->mode & SOAP_IO_LENGTH)
	{	
		if (soap_envelope_begin_out(soap)
			|| soap_putheader(soap)
			|| soap_body_begin_out(soap)
			|| soap_put___dndl__Probe(soap, &soap_tmp___dndl__Probe, "-dndl:Probe", NULL)
			|| soap_body_end_out(soap)
			|| soap_envelope_end_out(soap))
			return soap->error;
	}
	if (soap_end_count(soap))
	{
		return soap->error;
	}
	if (soap_connect(soap, soap_endpoint, soap_action, pEther)
		|| soap_envelope_begin_out(soap)
		|| soap_putheader(soap)
		|| soap_body_begin_out(soap)
		|| soap_put___dndl__Probe(soap, &soap_tmp___dndl__Probe, "-dndl:Probe", NULL)
		|| soap_body_end_out(soap)
		|| soap_envelope_end_out(soap)
		|| soap_end_send(soap))
	{
		return soap_closesock(soap);
	}
	return SOAP_OK;
}

int COnvifDevDiscover::SecondDiscoveryRecv(struct soap *soap, struct d__ProbeMatchesType *d__ProbeMatches)
{
	if (d__ProbeMatches == NULL)
	{
		return soap_closesock(soap);
	}

	soap_default_d__ProbeMatchesType(soap, d__ProbeMatches);

	if (soap_begin_recv(soap)
		|| soap_envelope_begin_in(soap)
		|| soap_recv_header(soap)
		|| soap_body_begin_in(soap))
	{
		return soap_closesock(soap);
	}
	soap_get_d__ProbeMatchesType(soap, d__ProbeMatches, "d:ProbeMatches", "d:ProbeMatchesType");
	if (soap->error == SOAP_TAG_MISMATCH && soap->level == 2)
		soap->error = SOAP_NO_METHOD;
	if (soap->error
		|| soap_body_end_in(soap)
		|| soap_envelope_end_in(soap)
		|| soap_end_recv(soap))
	{
		return soap_closesock(soap);
	}
	return soap_closesock(soap);

}


int COnvifDevDiscover::SecondDevicesDiscover(int i_eth_pos)
{
	struct soap *soap;  
	d__ProbeType req;  
	d__ProbeMatchesType resp;  
	d__ScopesType sScope;  
	struct SOAP_ENV__Header header;  
	int count = 0;  
	int result = 0;   
	char buf[64] = {0};
	char szEther[8] = {0};

	if(0 == i_eth_pos)
	{
		strcpy(szEther, MAIN_ETHER);
	}
	else
	{
		strcpy(szEther, POE_ETHER);
	}

	soap = soap_new();   

	if(soap==NULL)  
	{  
		return -1;  
	}  
	soap_init2(soap, SOAP_IO_UDP|SOAP_IO_FLUSH, SOAP_IO_UDP|SOAP_IO_FLUSH);
	soap_set_namespaces(soap, namespaces);   
	soap->recv_timeout = 2;      //超过2秒钟没有数据就退出  
	soap->bind_flags = SO_REUSEADDR;
	soap_default_SOAP_ENV__Header(soap, &header);  

	onvif_soap_generate_uuid(buf);

	header.wsa__MessageID = buf;  
	header.wsa__To= "urn:schemas-xmlsoap-org:ws:2005:04:discovery";  
	header.wsa__Action= "http://schemas.xmlsoap.org/ws/2005/04/discovery/Probe";  
	soap->header = &header;  

	soap_default_d__ScopesType(soap, &sScope);  
	sScope.__item = "";  
	soap_default_d__ProbeType(soap, &req);  
	req.Scopes = NULL;// &sScope;  
	req.Types = ""; //"tds:Device tdn:NetworkVideoTransmitter";  

	result = SecondDiscoveryStart(soap, "soap.udp://239.255.255.250:3702", NULL, &req, (const char*)szEther );  

	int nOffset = 7; // 7 = strlen("http://");
	ONVIF_SECOND_DEV_INFO stDeviceInfo;

	printf( "\n\n" );

	do{  
		result = SecondDiscoveryRecv(soap, &resp);   
		if (soap->error)   
		{   
			if (soap->error == SOAP_NO_METHOD )
				continue;

			result = soap->error;   
			break;  
		}   
		else  
		{  
			if ( resp.__sizeProbeMatch <= 0)
				continue;

			if( !resp.ProbeMatch )
				continue;

			//some cases the reply data is more then 1
			for( int i = 0; i < resp.__sizeProbeMatch; i++ )
			{
				if(!resp.ProbeMatch[i].XAddrs)
				{
					continue;
				}

				if(!strstr(resp.ProbeMatch[i].XAddrs,"onvif") != NULL)
				{
					continue;
				}

				memset(&stDeviceInfo,0,sizeof(ONVIF_SECOND_DEV_INFO));	
				
				if( strlen(resp.ProbeMatch[i].XAddrs) >= 127 )
				{
					strncpy( stDeviceInfo.Addr, resp.ProbeMatch[i].XAddrs, 127 );
				}
				else
				{
					strcpy( stDeviceInfo.Addr,resp.ProbeMatch[i].XAddrs);
				}

				if(resp.ProbeMatch[i].wsa__EndpointReference.Address != NULL)
				{
					if( strlen(resp.ProbeMatch[i].wsa__EndpointReference.Address) > 30 )
					{
						strncpy(stDeviceInfo.endPointReference,m_discoverResp.ProbeMatch[i].wsa__EndpointReference.Address,30);
					}
					else
					{
						strcpy(stDeviceInfo.endPointReference,m_discoverResp.ProbeMatch[i].wsa__EndpointReference.Address);
					}
				}

				printf( "%s---%s,%d\n", stDeviceInfo.Addr, __FILE__, __LINE__ );

				bool bTvtIpc = false;
				if(resp.ProbeMatch[i].__size > 0)
				{
					if(resp.ProbeMatch[i].__any != NULL)
					{
						if(strstr(resp.ProbeMatch[i].__any[0],"TDigitalIPC") != NULL)
						{
							bTvtIpc = true;
							break;
						}
					}
				}

				m_lock.Lock();
				bool bHasDev = SecondCheckDevhas( &stDeviceInfo );
				m_lock.UnLock();
				if( bHasDev )
				{
					break;
				}

				stDeviceInfo.manu_id_val = MANU_ONVIF_NORMAL;
				stDeviceInfo.ullSearchTimes = (ULONGLONG)GetCurrTime();
				strcpy(stDeviceInfo.szEther, szEther );

				m_lock.Lock();
				m_listOnvifDevice.push_back(stDeviceInfo);
				m_lock.UnLock();

				break;
			}	

		}  
	}while(1);  

	soap_destroy(soap); // remove deserialized class instances (C++ only)    
	soap_end(soap);     // clean up and remove deserialized data   
	soap_done(soap);  

	return 0;
}

bool COnvifDevDiscover::SecondRegetIpcProdInfo(const unsigned char* szIpAddr, ONVIF_SECOND_DEV_INFO& stOnvifInfo )
{
	std::list<ONVIF_SECOND_DEV_INFO>::iterator it;

	stOnvifInfo.manu_id_val = MANU_ONVIF_NORMAL;
	for (it=m_listOnvifDevice.begin(); it!=m_listOnvifDevice.end();++it)
	{
		if( strstr( (char*)it->Addr, (char*)szIpAddr ) )
		{
			if( OPERATE_SUCCESS != SecondGetDevSerial(it->Addr, &(*it) ) )
			{
				if( strlen( stOnvifInfo.endPointReference ) <= 0 )
				{
					if( strlen( it->Addr ) > 30 )
					{
						strncpy( stOnvifInfo.endPointReference, it->Addr, 30 );
					}
					else
					{
						strcpy( stOnvifInfo.endPointReference, it->Addr );
					}
				}

				strcpy( stOnvifInfo.Manufacturer, "ONVIF" );
			}
			else
			{
				strlwr(it->endPointReference);
				memcpy( &stOnvifInfo, &(*it), sizeof(ONVIF_SECOND_DEV_INFO) );
			}

			return true;
		}
	}

	strcpy( stOnvifInfo.endPointReference, (char*)szIpAddr );
	strcpy( stOnvifInfo.Manufacturer, "ONVIF" );

	return true;
}


bool COnvifDevDiscover::SecondCheckDevhas(ONVIF_SECOND_DEV_INFO* pDevInfo )
{
	std::list<ONVIF_SECOND_DEV_INFO>::iterator it;

	for (it=m_listOnvifDevice.begin(); it!=m_listOnvifDevice.end(); ++it)
	{
		if( 0 == strcmp( it->Addr, pDevInfo->Addr ) )
		{
			it->ullSearchTimes = (ULONGLONG)GetCurrTime();
			return true;
		}
	}

	return false;
}

void COnvifDevDiscover::SecondDeleteTimerOld( ULONGLONG ullTimes )
{
	std::list<ONVIF_SECOND_DEV_INFO>::iterator it;
	ULONGLONG ullTmpOldTm = 0;

	if( m_listOnvifDevice.size() <= 0 )
	{
		return;
	}

	m_lock.Lock();
	for (it=m_listOnvifDevice.begin(); it!=m_listOnvifDevice.end(); )
	{
		if( ullTimes >= it->ullSearchTimes )
		{
			ullTmpOldTm = ullTimes - it->ullSearchTimes;

			if( ullTmpOldTm > 5*60*1000*1000 )
			{
				it = m_listOnvifDevice.erase(it);
			}
			else
			{
				++it;
			}
		}
		else
		{
			it = m_listOnvifDevice.erase(it);
		}
	}
	m_lock.UnLock();
}

int COnvifDevDiscover::GetIPCProductIDNum(char* ManufacStr,char* ProdMode,char* firwareversion )
{
	int manu_id_val = MANU_ONVIF_NORMAL;

	if( strlen( ManufacStr) > 0 )
	{
		strlwr( ManufacStr );
		strlwr( ProdMode );
		strlwr( firwareversion );

		if( strstr( ManufacStr, "dahua" ) )
		{
			manu_id_val = MANU_ONVIF_DAHUA;
		}
		else if( strstr( ManufacStr, "hikvision" ) )
		{
			manu_id_val = MANU_ONVIF_HIKVISION;
		}
		else if( 3 == strlen( ManufacStr ) && strstr(ManufacStr, "znv"))
		{
			manu_id_val = MANU_ONVIF_ZNV;
		}
		else if( strstr( ManufacStr, "264") && strstr(ProdMode, "nvt"))
		{
			manu_id_val = MANU_ONVIF_XIONGMAI;
		}
		else if( strstr( ManufacStr, "onvif-ipnc") && strstr(ProdMode,"th"))
		{
			manu_id_val = MANU_ONVIF_TIANSHITONG;
		}
		else if( strstr( ManufacStr, "herospeed") )
		{
			manu_id_val = MANU_ONVIF_LONGPLUS;
		}
		else if( strstr( ManufacStr, "genuine" ))
		{
			manu_id_val = MANU_ONVIF_KEC;
		}
		else if( strstr( ManufacStr, "sunell" ))
		{
			manu_id_val = MANU_ONVIF_SUNELL;
		}
		else if( strstr( ManufacStr, "nvsip" ))
		{
			manu_id_val = MANU_ONVIF_NVSIP;
		}
		else if( strstr( ManufacStr, "stanley" ))
		{
			manu_id_val = MANU_ONVIF_STENLEY;
		}
		else if(0 == strcmp( ManufacStr, "qsee"))
		{
			manu_id_val = MANU_ONVIF_QSEE;
			if( strstr( firwareversion, "qsee") && strstr( firwareversion, "build:") )
			{
				manu_id_val = MANU_ONVIF_DAHUA;
			}
		}
		else if( 0 == strcmp( ManufacStr, "etrovision technology") )
		{
			manu_id_val = MANU_ONVIF_XTS;
		}
		else if(0 == strcmp( ManufacStr, "general"))
		{
			if( strstr( firwareversion, "general") && strstr( firwareversion, "build:") )
			{
				manu_id_val = MANU_ONVIF_DAHUA;
			}
		}

	}

	return manu_id_val;

}

int COnvifDevDiscover::SetCurDevicePoeChnnCnt( int poe_chnn_cnt )
{
	if( 0 == poe_chnn_cnt )
	{
		m_bHasMustSearchEth1Dev = false;
	}
	else
	{
		m_bHasMustSearchEth1Dev = true;
	}
	printf( "set cur device poe chnn number = %d,%d  %s,%d\n", poe_chnn_cnt, (int)m_bHasMustSearchEth1Dev, __FILE__, __LINE__ );

	return 0;
}

int COnvifDevDiscover::GetIPCDevviceSerial(char *servAddr, char *aUserName, char *aPassKey, int aDigest_type, bool& bGetSerialFlag )
{
	int result = -1;
	struct _tds__GetDeviceInformation information;
	struct _tds__GetDeviceInformationResponse response;
	unsigned int manu_prod = MANU_ONVIF_NORMAL;
	struct SOAP_ENV__Header *pHeader =NULL;

	enum  EResultType eResult = OTHER_FAULT;
	struct soap *pSoap = NULL;

	information.dummy = 0;
	if(servAddr == NULL)
	{
		return SERVER_ADDR_FAULT;
	}

	pSoap = onvif_init_soap(TDEVICE_MGT,false);
	if(pSoap == NULL)
	{
		return SOAP_INIT_FAULT;
	}


	if(aDigest_type == 0)
	{
		pHeader = onvif_init_soap_header(pSoap,false,NULL,aUserName,aPassKey,false,NULL);
		if(pHeader == NULL)
		{
			onvif_free_soap(pSoap);
			return SOAP_INIT_FAULT;
		}
	}
	else if( aDigest_type == 1 )
	{
		pHeader = onvif_init_soap_header(pSoap,false,NULL,aUserName,aPassKey,true,NULL);
		if(pHeader == NULL)
		{
			onvif_free_soap(pSoap);
			return SOAP_INIT_FAULT;
		}
	}
	else 
	{
		pHeader = NULL;
	}

	pSoap->header = pHeader;

	result = soap_call___tds__GetDeviceInformation(pSoap, servAddr,NULL,&information,&response);
	if(SOAP_OK == result)
	{

		char ManufacStr[32] = {0};
		char ProdMode[32] = {0};
		char FirwareInfo[64] = {0};

		if( response.SerialNumber )
		{
			if ((0 == strncmp( response.SerialNumber, "0018ae", 6 )) || (0 == strncmp( response.SerialNumber, "a81b189a", 6)))
			{
				manu_prod = MANU_ONVIF_TVT;
			}
		}

		if( MANU_ONVIF_TVT != manu_prod && response.Manufacturer && response.Model )
		{
			strncpy( ManufacStr, response.Manufacturer, 30 );
			strncpy( ProdMode, response.Model, 30 );
			strncpy( FirwareInfo, response.FirmwareVersion, 60 );

			manu_prod = GetIPCProductIDNum( ManufacStr, ProdMode, FirwareInfo );
		}

		bGetSerialFlag = true;
		eResult = OPERATE_SUCCESS;
	}
	else
	{
		eResult = OPERATE_FAILED;
	}

	if( pHeader )
	{
		pSoap->header = NULL;
		onvif_free_soap_header(pHeader);
		pHeader = NULL;
	}
	onvif_free_soap(pSoap);
	pSoap =NULL;

	return manu_prod;
}

int COnvifDevDiscover::SecondGetDevSerial(char *servAddr,ONVIF_SECOND_DEV_INFO* pOvifInfo)
{
	int result = -1;
	struct _tds__GetDeviceInformation information;
	struct _tds__GetDeviceInformationResponse response;

	enum  EResultType eResult = OTHER_FAULT;
	struct soap *pSoap = NULL;

	information.dummy = 0;
	if(servAddr == NULL)
	{
		return SERVER_ADDR_FAULT;
	}

	if(pOvifInfo == NULL)
	{
		return PARAMETER_FAULT;
	}

	pSoap = onvif_init_soap(TDEVICE_MGT,false);
	if(pSoap == NULL)
	{
		return SOAP_INIT_FAULT;
	}

	result = soap_call___tds__GetDeviceInformation(pSoap, servAddr,NULL,&information,&response);
	if(SOAP_OK == result)
	{
		if(response.SerialNumber != NULL)
		{
			if( strlen( response.SerialNumber ) > 30 )
			{
				strncpy( pOvifInfo->endPointReference, response.SerialNumber, 30 );
			}
			else
			{
				strcpy(pOvifInfo->endPointReference,response.SerialNumber);
			}
		}

		if( response.Manufacturer != NULL )
		{
			if( strlen(pOvifInfo->Manufacturer) > 30 )
			{
				strncpy( pOvifInfo->Manufacturer, response.Manufacturer, 30 );
			}
			else
			{
				strcpy( pOvifInfo->Manufacturer, response.Manufacturer );
			}
		}


		char ManufacStr[32] = {0};
		char ProdMode[32] = {0};
		char FirwareInfo[64] = {0};

		if( response.Manufacturer && response.Model )
		{
			strncpy( ManufacStr, response.Manufacturer, 30 );
			strncpy( ProdMode, response.Model, 30 );
			strncpy( FirwareInfo, response.FirmwareVersion, 60 );

			pOvifInfo->manu_id_val = GetIPCProductIDNum( ManufacStr, ProdMode, FirwareInfo );
		}

		printf( "%s++%s++%s++%s++%s++%s\n", servAddr, 
			response.FirmwareVersion,
			response.HardwareId,
			response.Manufacturer,
			response.Model,
			response.SerialNumber );

		eResult = OPERATE_SUCCESS;
	}
	else
	{
		eResult = OPERATE_FAILED;
	}

	onvif_free_soap(pSoap);
	pSoap =NULL;
	return eResult;
}


int COnvifDevDiscover::SecondAllEthDeviceDiscover()
{
	SecondDevicesDiscover(0);

#ifdef __ENVIRONMENT_LINUX__
	SecondDevicesDiscover(1);
#endif

	return 0;
}


int COnvifDevDiscover::SetONVIFDeviceIPAddr(char* search_onvif_ip_addr)
{
	COnvifDevMgt devMgt;
	struct OnvifNwParameter ipAddrSet;
	OnvifDevCapaServ capaServ;
	int result = -1;
	char servUri[128] = {'\0'};
	char userName[32] = {'\0'};
	char userPassword[32] = {'\0'};
	char *pTmp = NULL;

	printf( "search %s\n", search_onvif_ip_addr );

	pTmp = strstr( search_onvif_ip_addr, "onvif" );
	if( !pTmp )
	{
		printf( "ONVIF SetIP search onvif Error %s,%s,%d\n", search_onvif_ip_addr, __FILE__, __LINE__ );
		return -1;
	}

	strncpy( servUri, search_onvif_ip_addr, (int)(pTmp - search_onvif_ip_addr)+20 );
	strcpy( userName, "admin" );
	strcpy( userPassword, "123456" );

	memset( &capaServ, 0, sizeof(OnvifDevCapaServ) );
	if( 0 )
	{
		devMgt.InitDevMgtParameter(servUri,userName,userPassword);
		devMgt.InitDevUtcTimes(false,servUri,ONVIF_CTRL_DEV_USED_UTC_TIEM);
		result = devMgt.GetDevCapability(servUri,TAll,&capaServ,1);
		if(result != OPERATE_SUCCESS)
		{
			result = devMgt.GetDevCapability(servUri,TAll,&capaServ,0);
		}

		if( OPERATE_SUCCESS != result )
		{
			printf( "ONVIF SetIP GetDevCapability Error %s,%s,%d\n", servUri, __FILE__, __LINE__ );
			return -1;
		}

		if(!capaServ.bDevMgtEnable)
		{
			printf( "ONVIF SetIP capaServ.bDevMgtEnable Error %s,%s,%d\n", servUri, __FILE__, __LINE__ );
			return -1;
		}
	}

	memset( &ipAddrSet, 0, sizeof(struct OnvifNwParameter) );
	devMgt.InitDevMgtParameter(servUri,userName,userPassword);
	devMgt.InitDevUtcTimes(false,servUri,ONVIF_CTRL_DEV_USED_UTC_TIEM);
	devMgt.GetNetworkParameter(&ipAddrSet,1);   
	//devMgt.SetNetworkParameter(&ipAddrSet,0);

	return 0;
}

