// NatUpnpMapper.h: interface for the NatUpnpMapper class.
//
//////////////////////////////////////////////////////////////////////
#ifndef __NAT_UPNP_MAPPER_H__
#define __NAT_UPNP_MAPPER_H__

#include <vector>

class CNatUpnpClient;

typedef struct __nat_upnp_mapper_config__
{
	unsigned short		localPort;			/**< UPNP映射的本地端口	 */
	char				deviceNo[80];		/**< 设备序列号 */
	char				multicastif[256];	/**< 多播地址，主要用于UPNP，为空表示由内部选择 */
}NAT_UPNP_MAPPER_CONFIG;

class CNatUpnpMapper
{
public:
	static const unsigned short EXTERNAL_PORT_BEGIN = 17000;
	static const unsigned short EXTERNAL_PORT_COUNT = 1000;
public:
	enum NAT_UPNP_MAPPER_STATE
	{
		STATE_NONE,
		STATE_INITED,
		STATE_MAPPING,
		STATE_ERROR
	};

	enum PORT_MAPPING_STATE
	{
		PORT_MAPPING_INIT,
		PORT_MAPPING_SUCCEEDED,
		PORT_MAPPING_ERROR
	};

	typedef struct _port_mapped_info_
	{
		PORT_MAPPING_STATE		state;
		unsigned short			localPort;
		unsigned short			externalPort;
	}PORT_MAPPING_INFO;

	class CPortMappingChangeNotifier
	{
	public:
		virtual void OnPortMappingChange(PORT_MAPPING_STATE oldState, const PORT_MAPPING_INFO &info) = 0;
	};

public:
	CNatUpnpMapper();
	virtual ~CNatUpnpMapper();
public:
	void SetNotifier(CPortMappingChangeNotifier *notifier);

	bool Start(const NAT_UPNP_MAPPER_CONFIG *config);

	void Stop();

	bool IsStarted();
	void ChangeLocalPort(unsigned short port);
private:
	class CUpnpMappingContext;

private:
	static PUB_THREAD_RESULT PUB_THREAD_CALL WorkThreadFunc(void *pParam);
	int RunWork();

	void Clear();
	unsigned short NewExternalPort();
	void NotifyPortMappingChange(PORT_MAPPING_STATE oldState, const PORT_MAPPING_INFO &info);
	bool InitUpnpClient(unsigned long curTickCount);
	void FinalUpnpClient(unsigned long curTickCount);
	void ProcUpnpClean(unsigned long curTickCount);	
private:
	static const int UPNP_CLIENT_INIT_INTERVAL = 30 * 1000;
	static const int LONG_TIME_ERROR_TIMEOUT = 3 * 60 * 1000;
private:
	CUpnpMappingContext* m_context;
	CNatUpnpClient *m_upnpClient;
	NAT_UPNP_MAPPER_STATE m_state;
	NAT_UPNP_MAPPER_CONFIG m_config;

	unsigned short m_externalPortBegin;
	unsigned short m_externalPortEnd;

	int m_isCanceled;
	bool m_workThreadRunning;
	PUB_thread_t m_workThreadID;

	unsigned long	m_localIp;
	unsigned short  m_curExternalPort;
	char			m_upnpDesc[80];
	CPortMappingChangeNotifier *m_portMappingChangeNotifier;
	bool			m_isUpnpCleanCompleted;
	unsigned long	m_upnpCleanTime;
	int				m_upnpCleanIndex;
};

#endif//__NAT_UPNP_MAPPER_H__