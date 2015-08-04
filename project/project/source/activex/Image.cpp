#ifdef __ENVIRONMENT_WIN32__
#include <afxwin.h>
#include <assert.h>
#endif
#include "Font.h"
#include "Image.h"

#define BMP_FILE ((('M') << 8) | ('B'))

#ifdef __ENVIRONMENT_WIN32__
std::string GUI::CImage::s_strPath = "..\\ui\\";
#else
std::string GUI::CImage::s_strPath = "./ui/";
#endif

#ifdef __TDHISI__
unsigned char* GUI::CImage::m_pImageDataAddr = NULL;
#endif

TRANSPARENCY GUI::CImage::s_transparency = TRANSPARENCY_0;

const unsigned short DEF_COLOR_CLARITY = 0x7c1f;//(0x7fff & _RGB1555(255, 0, 255));

GUI::CImage::CImage(void) : m_lpData(NULL)
, m_iPitch(0)
, m_iBpp(0)
, m_iPaletteEntries(0)
, m_dwSize(0)
{
}

GUI::CImage::~CImage(void)
{
#ifdef __TDHISI__
	m_lpData = NULL;
#else
	if (NULL != m_lpData)
	{
		delete [] m_lpData;
		m_lpData = NULL;
	}
#endif
}

bool GUI::CImage::LoadImage(const BYTE* pData, int len, int width, int height, bool bClarity/* = false*/)
{
	assert(NULL != pData);

#ifdef __TDHISI__
	m_lpData = m_pImageDataAddr;
#else
	if (NULL != m_lpData)
	{
		delete [] m_lpData;
	}

	m_lpData = new BYTE[len];
#endif

	unsigned char *pDec = m_lpData;
	unsigned char *pSrc = (unsigned char *)pData;

	for (unsigned short h=0; h<height; ++h)
	{
		for (unsigned short w=0; w<width; ++w)
		{
			if(4 == FRAME_BUFFER_PIXSEL_NUM)
			{
				memcpy(pDec, pSrc, 4);
				pDec[3] = s_transparency;
			}
			else if(2 == FRAME_BUFFER_PIXSEL_NUM)
			{
				memcpy(pDec, pSrc, 2);
				
				if (bClarity && (DEF_COLOR_CLARITY == (*reinterpret_cast<unsigned short *>(pDec))))
				{
					*reinterpret_cast<unsigned short *>(pDec) &= 0x7fff;
				}
			}
			else
			{
				assert(false);
			}

			pDec += FRAME_BUFFER_PIXSEL_NUM;
			pSrc += FRAME_BUFFER_PIXSEL_NUM;
		}
	}

	m_bih.biWidth = width;
	m_bih.biHeight = height;
	m_iPitch = width * FRAME_BUFFER_PIXSEL_NUM;
	m_dwSize = m_iPitch * height;

#ifdef __TDHISI__
	m_pImageDataAddr += m_dwSize;
#endif

	return true;
}

bool GUI::CImage::LoadImage(const char* pFileName, bool bClarity/* = false*/)
{
	if (pFileName != NULL)
	{
		std::string strFileName = s_strPath + pFileName;

#ifdef __COMPRESS_UI_FONT__
		char szCmd[512] = {0};		
		snprintf(szCmd, 512, "/nfsdir/ui/%s", pFileName);
		strFileName = szCmd;
#endif

		FILE* file =  fopen(strFileName.c_str(), "rb");
		if (file != NULL)
		{
#ifdef __TDHISI__
			m_lpData = NULL;
#else
			if (m_lpData != NULL)
			{
				delete [] m_lpData;
				m_lpData = NULL;
			}
#endif
			int readLen = 0;
			// 读取 BITMAPFILEHEADER
			readLen = sizeof(BITMAPFILEHEADER);
			while (readLen != fread(&m_bfh, 1, readLen, file))
			{
				printf("%s:%s:%d, Read file error\n", __FUNCTION__, __FILE__, __LINE__);
				perror("Read file error:");
				
				PUB_Sleep(10);
			}
			
			if (BMP_FILE != m_bfh.bfType)
			{
				assert(false);//不是BMP文件
				fclose(file);
				return false;
			}

			// 读取 BITMAPINFOHEADER 
			readLen = sizeof(BITMAPINFOHEADER);
			fread(&m_bih, 1, readLen, file);

			// Calculate pitch
			m_iBpp = m_bih.biBitCount >> 3;
			m_iPitch = m_iBpp * m_bih.biWidth;
			m_iPitch = ((m_iPitch + 3)>>2)<<2;

			// Check for bit-depth (8, 16, 24 and 32bpp only)
			if (m_bih.biBitCount >= 8)
			{
				if (m_bih.biBitCount == 8)
				{	//调色板
					m_iPaletteEntries = m_bih.biClrUsed;
					if (m_iPaletteEntries == 0)
						m_iPaletteEntries = 256;

					//读取调色板信息
					fread(m_lpPalette, m_iPaletteEntries, sizeof(RGBQUAD), file);
				}

				// Read image data
				m_dwSize = m_iPitch * m_bih.biHeight;
				m_lpData = new unsigned char[m_dwSize];
				fread(m_lpData, m_dwSize, sizeof(unsigned char), file);

				// Convert to 32bpp bitmap
				if (m_iBpp != FRAME_BUFFER_PIXSEL_NUM)
				{
					if (4 == FRAME_BUFFER_PIXSEL_NUM)
					{
						_ConvertTo32Bpp(bClarity);
					}
					else if(2 == FRAME_BUFFER_PIXSEL_NUM)
					{
						_ConvertTo16Bpp(bClarity);
					}
					else
					{
						assert(false);
					}
				}

				AdjustBMP(bClarity);

#ifdef __TDHISI__
				memcpy(m_pImageDataAddr, m_lpData, m_dwSize);

				//删除16bits的内存空间，重新指向32位的内存空间。
				delete [] m_lpData;

				m_lpData = m_pImageDataAddr;

				m_pImageDataAddr += m_dwSize;
#endif
			}

			// Close .BMP file
			fclose(file);
			return true;
		}
		else
		{
			printf("%s:%s:%d, Open file error\n", __FUNCTION__, __FILE__, __LINE__);
			perror("Open file error:");
		}
	}
	else
	{
		printf("%s:%s:%d, No bmp file to open\n", __FUNCTION__, __FILE__, __LINE__);
	}

	return false;	
}

void GUI::CImage::AdjustBMP(bool bClarity)
{
	if (IsValid())
	{
		assert (m_dwSize > 0);
		unsigned char* lpData = new unsigned char[m_dwSize];
		assert (NULL != lpData);

		unsigned char *pDec = lpData;
		unsigned char *pSrc = m_lpData;
		unsigned short *pPixel = reinterpret_cast<unsigned short*>(pSrc);

		long _height = m_bih.biHeight - 1;

		for (long h=0; h<m_bih.biHeight; ++h)
		{
			pSrc = m_lpData + (_height*m_iPitch);

			for (long w=0; w<m_bih.biWidth; ++w)
			{
				if(4 == FRAME_BUFFER_PIXSEL_NUM)
				{
					memcpy(pDec, pSrc, 4);
					pDec[3] = s_transparency;
				}
				else if(2 == FRAME_BUFFER_PIXSEL_NUM)
				{
					pPixel = reinterpret_cast<unsigned short*>(pSrc);

					if (bClarity && (DEF_COLOR_CLARITY == (*pPixel)))
					{
						*pPixel &= 0x7fff;
					}
					else
					{
						*pPixel = (0x8000 | (*pPixel));
					}
					
					memcpy(pDec, pSrc, 2);
				}
				else
				{
					assert(false);
				}

				pDec += FRAME_BUFFER_PIXSEL_NUM;
				pSrc += FRAME_BUFFER_PIXSEL_NUM;
			}

			-- _height;
		}

		delete [] m_lpData;
		m_lpData = lpData;
	}
	else
	{
		assert(false);
	}

	
}

void GUI::CImage::_ConvertTo32Bpp(bool bClarity)
{
	// Check for valid bitmap
	if (IsValid())
	{
		// Calculate new params
		long _bpp = (_BITS_PER_PIXEL_32 >> 3);
		long _pitch = m_bih.biWidth * _bpp;

		// Create temporary bitmap
		unsigned long dwSize = _pitch * m_bih.biHeight;
		unsigned char* lpData = new unsigned char[dwSize];

		// Convert bitmap
		unsigned long dwDstHorizontalOffset;
		unsigned long dwDstVerticalOffset = 0;
		unsigned long dwDstTotalOffset;
		unsigned long* lpDstData = reinterpret_cast<unsigned long*>(lpData);
		unsigned long dwSrcHorizontalOffset;
		unsigned long dwSrcVerticalOffset = 0;
		unsigned long dwSrcTotalOffset;
		for (long i=0; i<m_bih.biHeight; i++)
		{
			dwDstHorizontalOffset = 0;
			dwSrcHorizontalOffset = 0;
			for (long j=0; j<m_bih.biWidth; j++)
			{
				// Update destination total offset
				dwDstTotalOffset = dwDstVerticalOffset + dwDstHorizontalOffset;

				// Update source total offset
				dwSrcTotalOffset = dwSrcVerticalOffset + dwSrcHorizontalOffset;

				// Update bitmap
				switch (m_bih.biBitCount)
				{
				case 8:
					{
						unsigned char red = m_lpPalette[m_lpData[dwSrcTotalOffset]].rgbRed;
						unsigned char green = m_lpPalette[m_lpData[dwSrcTotalOffset]].rgbGreen;
						unsigned char blue = m_lpPalette[m_lpData[dwSrcTotalOffset]].rgbBlue;
						lpDstData[dwDstTotalOffset>>2] = _RGB(red, green, blue);
					}
					break;

				case 16:
					{
						unsigned short* lpSrcData = reinterpret_cast<unsigned short*>(m_lpData);
						unsigned char red = ((lpSrcData[dwSrcTotalOffset>>1] & 0x7C00) >> 10) << 3;
						unsigned char green = ((lpSrcData[dwSrcTotalOffset>>1] & 0x03E0) >> 5) << 3;
						unsigned char blue = (lpSrcData[dwSrcTotalOffset>>1] & 0x001F) << 3;
						lpDstData[dwDstTotalOffset>>2] = _RGB(red, green, blue);
					}
					break;

				case 24:
					{
						lpDstData[dwDstTotalOffset>>2] = _RGB(m_lpData[dwSrcTotalOffset+2], m_lpData[dwSrcTotalOffset+1], m_lpData[dwSrcTotalOffset]);
					}
					break;

				case 32:
					{
						unsigned long* lpSrcData = reinterpret_cast<unsigned long*>(m_lpData);
						lpDstData[dwDstTotalOffset>>2] = lpSrcData[dwSrcTotalOffset>>2];
					}
					break;
				}

				// Update destination horizontal offset
				dwDstHorizontalOffset += _bpp;

				// Update source horizontal offset
				dwSrcHorizontalOffset += m_iBpp;
			}

			// Update destination vertical offset
			dwDstVerticalOffset += _pitch;

			// Update source vertical offset
			dwSrcVerticalOffset += m_iPitch;
		}

		// Update bitmap info
		m_iPitch = _pitch;
		m_iBpp = _bpp;

		delete []m_lpData;
		m_dwSize = dwSize;
		m_lpData = lpData;
		m_bfh.bfSize = sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER) + m_dwSize;
		m_bfh.bfOffBits = sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER);
		m_bih.biBitCount = _BITS_PER_PIXEL_32;
	}
}

void GUI::CImage::_ConvertTo16Bpp(bool bClarity)
{
	// Check for valid bitmap
	if (IsValid())
	{
		// Calculate new params
		long _bpp = (_BITS_PER_PIXEL_16 >> 3);
		long _pitch = m_bih.biWidth * _bpp;

		// Create temporary bitmap
		unsigned long dwSize = _pitch * m_bih.biHeight;
		unsigned char* lpData = new unsigned char[dwSize];

		// Convert bitmap
		unsigned long dwDstHorizontalOffset;
		unsigned long dwDstVerticalOffset = 0;
		unsigned long dwDstTotalOffset;
		unsigned short* lpDstData = reinterpret_cast<unsigned short*>(lpData);
		unsigned long dwSrcHorizontalOffset;
		unsigned long dwSrcVerticalOffset = 0;
		unsigned long dwSrcTotalOffset;
		for (long i=0; i<m_bih.biHeight; i++)
		{
			dwDstHorizontalOffset = 0;
			dwSrcHorizontalOffset = 0;
			for (long j=0; j<m_bih.biWidth; j++)
			{
				// Update destination total offset
				dwDstTotalOffset = dwDstVerticalOffset + dwDstHorizontalOffset;

				// Update source total offset
				dwSrcTotalOffset = dwSrcVerticalOffset + dwSrcHorizontalOffset;

				// Update bitmap
				switch (m_bih.biBitCount)
				{
				case 8:
					{
						unsigned char red = m_lpPalette[m_lpData[dwSrcTotalOffset]].rgbRed;
						unsigned char green = m_lpPalette[m_lpData[dwSrcTotalOffset]].rgbGreen;
						unsigned char blue = m_lpPalette[m_lpData[dwSrcTotalOffset]].rgbBlue;
						lpDstData[dwDstTotalOffset/2] = _RGB1555(red, green, blue);
					}
					break;

				case 16:
					{
						unsigned short* lpSrcData = reinterpret_cast<unsigned short*>(m_lpData);
						//unsigned char red = (lpSrcData[dwSrcTotalOffset>>1] & 0x7C00) >> 10;
						//unsigned char green = (lpSrcData[dwSrcTotalOffset>>1] & 0x03E0) >> 5;
						//unsigned char blue = lpSrcData[dwSrcTotalOffset>>1] & 0x001F;
						lpDstData[dwDstTotalOffset/2] = (0x8000) | (*lpSrcData);
					}
					break;

				case 24:
					{
						lpDstData[dwDstTotalOffset/2] = _RGB1555(m_lpData[dwSrcTotalOffset+2], m_lpData[dwSrcTotalOffset+1], m_lpData[dwSrcTotalOffset]);
					}
					break;

				case 32:
					{
						unsigned long* lpSrcData = reinterpret_cast<unsigned long*>(m_lpData);
						lpDstData[dwDstTotalOffset/2] = GUI_RGB32_TO_RGB1555(lpSrcData[dwSrcTotalOffset>>2]);
					}
					break;
				}

				// Update destination horizontal offset
				dwDstHorizontalOffset += _bpp;

				// Update source horizontal offset
				dwSrcHorizontalOffset += m_iBpp;
			}

			// Update destination vertical offset
			dwDstVerticalOffset += _pitch;

			// Update source vertical offset
			dwSrcVerticalOffset += m_iPitch;
		}

		// Update bitmap info
		m_iPitch = _pitch;
		m_iBpp = _bpp;

		delete []m_lpData;
		m_dwSize = dwSize;
		m_lpData = lpData;
		m_bfh.bfSize = sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER) + m_dwSize;
		m_bfh.bfOffBits = sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER);
		m_bih.biBitCount = _BITS_PER_PIXEL_16;
	}
}

long GUI::CImage::Size() const
{
	return m_dwSize;
}

