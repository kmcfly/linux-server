#include "dvrdvsdef.h"
#include "PUB_common.h"

class CTvtAlarm
{
public:

	CTvtAlarm();
	~CTvtAlarm();
	bool Init();

	static CTvtAlarm* Instance();

	int Start();
	int Stop();

	static PUB_THREAD_RESULT PUB_THREAD_CALL TvtAlarmThread(void *pParam);

private:
	
	void CheckProc();

	ULONGLONG m_ullMotionOld;
	ULONGLONG m_ullSensorOld;
	ULONGLONG m_ullVideoLossOld;

	PUB_thread_t m_TvtAlarmID;
	bool		m_bTvtAlarmRun;
};