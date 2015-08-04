#ifndef _DRAW_MANAGER_H_
#define _DRAW_MANAGER_H_

#include "Wnd.h"

namespace GUI
{
	class CDrawManager
	{
	public:
		CDrawManager();
		virtual ~CDrawManager();

		enum { INVLAID = -1 };

		//拷贝外部图片
		BYTE* CopyImage(BYTE* pImg, int len, bool bUseAppBuf = false);

		//拷贝数据
		BYTE* CopyHorizontal(BITMAP_ID bmpID, int x, int y, int cx, int cy, int * pLen = NULL, bool bUseAppBuf = false);
		//将某一列的数据（src_x指向)填充到指定区域
		BYTE* FillHorizontal(BITMAP_ID bmpID, int src_x, int dst_cx, int dst_cy, int * pLen = NULL, bool bUseAppBuf = false);

		//拉伸图片，指定copy_x、copy_y方向填充
		BYTE* Stretch(BITMAP_ID bmpID, int dst_cx, int dst_cy, int copy_x, int copy_y, int * pLen = NULL, bool bUseAppBuf = false);

		BYTE* GetData() { return m_pData; }
		const BYTE* GetData() const { return m_pData; }
		int GetDataLen() const { return m_dataLen; }

		static void SetTransparency(TRANSPARENCY transparency) {s_transparency = transparency;}
#ifdef __TDHISI__
		static void SetImageDataAddr(unsigned char *pAddr) {m_pImageDataAddr = pAddr;}
#endif

	protected:
#ifdef __TDHISI__
		static unsigned char* m_pImageDataAddr;
#endif

		struct IMAGE_INFO 
		{
			BYTE* pData;
			int len;
			int width;
			int height;
		};

		void StretchHorizontal(IMAGE_INFO& dst, const IMAGE_INFO& src, int middlePos) const;
		void StretchVertical(IMAGE_INFO& dst, const IMAGE_INFO& src, int middlePos) const;
		void CutVertical(IMAGE_INFO& dst, const IMAGE_INFO& src, int begin, int end) const;

		BYTE* m_pData;
		int m_dataLen;
		static TRANSPARENCY s_transparency;
	private:
	};
}

#endif

