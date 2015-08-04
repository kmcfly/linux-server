///////////////////////////////////////////////////////////////////////////////////////////////////
#ifndef __PIC_CREATE_H__
#define __PIC_CREATE_H__

#include <string.h>
#include <assert.h>

#include "Font.h"
#include "gui.h"
#include "FBInterface.h"
#include "Thai.h"
#include "Arabic.h"

using namespace GUI;

class CPicCreate
{
public:
	CPicCreate();
	~CPicCreate();
	int CreatePic(const char * pStr, int w, int h, unsigned long color, unsigned long backColor=0x7fff, bool bResort = true);
	int GetPicData(unsigned char ** ppData, unsigned long &dataLen);
	void SetPicToAuxiliaryOutputDevice();
protected:
private:
	int CreateRect(unsigned long color=0x7fff);
	int CreateText(const char * pStr, unsigned long color, bool bResort=false);

	unsigned long DrawText(const char *pStr, unsigned long len, unsigned short x, unsigned short y, unsigned short width, unsigned short height, unsigned long color, unsigned char zoom = 2, bool bForceDrawBorder = false, bool bResort = true);
private:
	unsigned char *			m_pDrawBuf;
	unsigned long	m_pDrawBufLen;

	int				m_width;
	int				m_height;

	int				m_cx;
	int				m_cy;

	TRANSPARENCY	m_transparency;
};
#endif  /*__PIC_CREATE_H__*/
//////////////////////////////////////////////////////////////////////////////////////////////////
