#ifndef __CSNAPPICTUREMAN_H__
#define __CSNAPPICTUREMAN_H__
#include "dvrdvsdef.h"
#include "PUB_common.h"

class CSnapPictureMan
{
public:
	~CSnapPictureMan(void);
	static CSnapPictureMan *GetInstance();	
	bool Initial(unsigned char chnnNum);
	bool Quite();
	bool SetSnapPicture(unsigned long chnnl, unsigned long type);
	bool SnapPicture(void *buf, int nLen, int nChannel, LONGLONG time , unsigned short width, unsigned short height, bool bLock = false);
	void SetSnapTimeS(const unsigned long timeS);
	void SetSnapImgNum(const unsigned long imgNum);
	unsigned long GetSnapTime();

private:
	CSnapPictureMan(void);
	
	unsigned long   	*m_pChannType;
	int                 *m_pChnnStatus;	
	int                 *m_pChnnCount;
	LONGLONG            *m_pLastTime;
	int                  m_nMaxSnap;
	CPUB_Lock            m_lock;
	static CSnapPictureMan *m_Instance;
	unsigned long        m_nMaxTime;
	int                 m_nCurSnapNum;
};
#endif
