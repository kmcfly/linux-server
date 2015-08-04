#ifndef _IMAGE_H_
#define _IMAGE_H_
#ifdef __ENVIRONMENT_WIN32__
#include <string>
#else
#include <string.h>
#endif
#include "PUB_common.h"

namespace GUI
{
#define _BITS_PER_PIXEL_32	32									// 32-bit color depth
#define _BITS_PER_PIXEL_24	24									// 24-bit color depth
#define _BITS_PER_PIXEL_16	16									// 16-bit color depth
#define _RGB(r,g,b)	(((r) << 16) | ((g) << 8) | (b))			// Convert to RGB
#define _RGB1555(r,g,b) ( \
		(unsigned short) \
		( \
		(0x8000) | \
		(((r & 0xFF) >> 3) << 10) | \
		(((g & 0xFF) >> 3) << 5) | \
		((b & 0xFF) >> 3) \
		) \
		)

	class CImage
	{
	public:
		CImage(void);
		virtual ~CImage(void);

		bool LoadImage(const char* pFileName, bool bClarity = false); //加载位图
		bool LoadImage(const BYTE* pData, int len, int width, int height, bool bClarity = false);

		const unsigned char* GetDate() const { return m_lpData; } //获得数据
		unsigned char* GetDate() { return m_lpData; }

		long Width() const//获得宽度
		{ return m_bih.biWidth; }

		long Height() const//获得高度
		{ return m_bih.biHeight;}

		long Size() const;
		static void SetTransparency(TRANSPARENCY transparency) {s_transparency = transparency;}

#ifdef __TDHISI__
		static void SetImageDataAddr(unsigned char *pAddr) {m_pImageDataAddr = pAddr;}
#endif
	protected:
#ifdef __TDHISI__
		static unsigned char* m_pImageDataAddr;
#endif

		unsigned char* m_lpData;
		long m_iPitch;
		long m_iBpp;
		BITMAPFILEHEADER m_bfh;
		BITMAPINFOHEADER m_bih;
		unsigned long m_dwSize;
		long m_iPaletteEntries;
		RGBQUAD m_lpPalette[256];

		static std::string s_strPath;
		static TRANSPARENCY s_transparency;
	private:
		bool IsValid()  const {return (m_lpData != NULL);}
		void _ConvertTo32Bpp(bool bClarity);
		void _ConvertTo16Bpp(bool bClarity);
		void AdjustBMP(bool bClarity);//重新排列数据，反转
	};

};
#endif //_IMAGE_H_
