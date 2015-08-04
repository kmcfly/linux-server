#include "DrawManager.h"
#include "ImageList.h"

using namespace GUI;
TRANSPARENCY GUI::CDrawManager::s_transparency = TRANSPARENCY_0;

#ifdef __TDHISI__
unsigned char* GUI::CDrawManager::m_pImageDataAddr = NULL;
#endif

CDrawManager::CDrawManager() : m_pData(NULL)
, m_dataLen(0)
{

}

CDrawManager::~CDrawManager()
{
#ifdef __TDHISI__
	if (m_pData == m_pImageDataAddr)
	{
		m_pData = NULL;
	}
	else
	{
		if (NULL != m_pData)
		{
			delete [] m_pData;
			m_pData = NULL;	
		}
	}
#else
	if (NULL != m_pData)
	{
		delete [] m_pData;
		m_pData = NULL;
	}
#endif

	m_dataLen = 0;
}

BYTE* CDrawManager::Stretch(BITMAP_ID bmpID, int dst_cx, int dst_cy, int copy_x, int copy_y, int * pLen /* = NULL */, bool bUseAppBuf/* = false*/)
{
	//读取图片
	GUI::CImageList* pImageList = CImageList::Instance();
	GUI::CImage* pImage = pImageList->GetImage(bmpID);
	assert(NULL != pImage);
	//输入参数判断
	if (CDrawManager::INVLAID != copy_x)
	{
		if ((copy_x > pImage->Width()) || (dst_cx <= pImage->Width()))
		{
			assert(false);
			return NULL;
		}
	}
	if (CDrawManager::INVLAID != copy_y)
	{
		if ((copy_y > pImage->Height()) || (dst_cy <= pImage->Height()))
		{
			assert(false);
			return NULL;
		}
	}

#ifdef __TDHISI__
	if (bUseAppBuf)
	{
		m_pData = NULL;
	}
	else
	{
		if (NULL != m_pData)
		{
			delete [] m_pData;
			m_pData = NULL;	
		}
	}
#else
	//
	if (NULL != m_pData)
	{
		delete [] m_pData;
		m_pData = NULL;	
	}
#endif

	m_dataLen = 0;

	IMAGE_INFO src;	
	IMAGE_INFO dst;

	if ((CDrawManager::INVLAID != copy_x) && (CDrawManager::INVLAID != copy_y))
	{
		src.pData = pImage->GetDate();
		src.len = pImage->Size();
		src.width = pImage->Width();
		src.height = pImage->Height();

		IMAGE_INFO dst_h;
		dst_h.pData = NULL;
		dst_h.len = 0;
		dst_h.width = dst_cx;
		dst_h.height = pImage->Height();

		StretchHorizontal(dst_h, src, copy_x);

		dst.pData = NULL;
		dst.len = 0;
		dst.width = dst_cx;
		dst.height = dst_cy;

		StretchVertical(dst, dst_h, copy_y);
		
		delete [] dst_h.pData;
		dst_h.pData = NULL;
	}
	else if (CDrawManager::INVLAID != copy_x)
	{
		src.pData = pImage->GetDate();
		src.len = pImage->Size();
		src.width = pImage->Width();
		src.height = pImage->Height();

		dst.pData = NULL;
		dst.len = 0;
		dst.width = dst_cx;
		dst.height = dst_cy;

		StretchHorizontal(dst, src, copy_x);
	}
	else if (CDrawManager::INVLAID != copy_y)
	{
		src.pData = pImage->GetDate();
		src.len = pImage->Size();
		src.width = pImage->Width();
		src.height = pImage->Height();

		dst.pData = NULL;
		dst.len = 0;
		dst.width = dst_cx;
		dst.height = dst_cy;

		StretchVertical(dst, src, copy_y);
	}
	else
	{
		assert(false);
		return NULL;
	}
	
	m_dataLen = dst.len;

	//
#ifdef __TDHISI__
	if (bUseAppBuf)
	{
		assert (NULL != m_pImageDataAddr);

		unsigned char *pDec = m_pImageDataAddr;
		unsigned char *pSrc = dst.pData;

		for (unsigned short h=0; h<dst.height; ++h)
		{
			for (unsigned short w=0; w<dst.width; ++w)
			{
				if(4 == FRAME_BUFFER_PIXSEL_NUM)
				{
					memcpy(pDec, pSrc, 4);
					pDec[3] = s_transparency;
				}
				else if(2 == FRAME_BUFFER_PIXSEL_NUM)
				{
					memcpy(pDec, pSrc, 2);
				}
				else
				{
					assert(false);
				}

				pDec += FRAME_BUFFER_PIXSEL_NUM;
				pSrc += FRAME_BUFFER_PIXSEL_NUM;
			}
		}

		if (NULL != pLen)
		{
			*pLen = m_dataLen;
		}

		delete [] dst.pData;

		return m_pImageDataAddr;
	}
	else
	{
		m_pData = new unsigned char [m_dataLen];
		assert (NULL != m_pData);

		unsigned char *pDec = m_pData;
		unsigned char *pSrc = dst.pData;

		for (unsigned short h=0; h<dst.height; ++h)
		{
			for (unsigned short w=0; w<dst.width; ++w)
			{
				if(4 == FRAME_BUFFER_PIXSEL_NUM)
				{
					memcpy(pDec, pSrc, 4);
					pDec[3] = s_transparency;
				}
				else if(2 == FRAME_BUFFER_PIXSEL_NUM)
				{
					memcpy(pDec, pSrc, 2);
				}
				else
				{
					assert(false);
				}

				pDec += FRAME_BUFFER_PIXSEL_NUM;
				pSrc += FRAME_BUFFER_PIXSEL_NUM;
			}
		}
	}
#else
	m_pData = new unsigned char [m_dataLen];
	assert (NULL != m_pData);

	unsigned char *pDec = m_pData;
	unsigned char *pSrc = dst.pData;

	for (unsigned short h=0; h<dst.height; ++h)
	{
		for (unsigned short w=0; w<dst.width; ++w)
		{
			if(4 == FRAME_BUFFER_PIXSEL_NUM)
			{
				memcpy(pDec, pSrc, 4);
				pDec[3] = s_transparency;
			}
			else if(2 == FRAME_BUFFER_PIXSEL_NUM)
			{
				memcpy(pDec, pSrc, 2);
			}
			else
			{
				assert(false);
			}

			pDec += FRAME_BUFFER_PIXSEL_NUM;
			pSrc += FRAME_BUFFER_PIXSEL_NUM;
		}
	}
#endif

	if (NULL != pLen)
	{
		*pLen = m_dataLen;
	}

	delete [] dst.pData;

	return m_pData;
}

BYTE* CDrawManager::CopyImage(BYTE* pImg, int len, bool bUseAppBuf /*= false*/)
{
#ifdef __TDHISI__
	if (bUseAppBuf)
	{
		m_pData = NULL;
	}
	else
	{
		if (NULL != m_pData)
		{
			delete [] m_pData;
			m_pData = NULL;	
		}
	}
#else
	//
	if (NULL != m_pData)
	{
		delete [] m_pData;
		m_pData = NULL;	
	}
#endif

	m_dataLen = len;

#ifdef __TDHISI__
	if (bUseAppBuf)
	{
		assert (NULL != m_pImageDataAddr);
		m_pData = m_pImageDataAddr;
	}
	else
	{
		m_pData = new BYTE[m_dataLen];
	}
#else
	m_pData = new BYTE[m_dataLen];
	assert (NULL != m_pData);
#endif

	//Copy
	memcpy(m_pData, pImg, len);

	return m_pData;
}

BYTE* CDrawManager::CopyHorizontal(BITMAP_ID bmpID, int x, int y, int cx, int cy, int * pLen /* = NULL */, bool bUseAppBuf/* = false*/)
{
#ifdef __TDHISI__
	if (bUseAppBuf)
	{
		m_pData = NULL;
	}
	else
	{
		if (NULL != m_pData)
		{
			delete [] m_pData;
			m_pData = NULL;	
		}
	}
#else
	//
	if (NULL != m_pData)
	{
		delete [] m_pData;
		m_pData = NULL;	
	}
#endif

	m_dataLen = 0;

	GUI::CImageList* pImageList = CImageList::Instance();
	GUI::CImage* pImage = pImageList->GetImage(bmpID);

	int srcWidth = pImage->Width();
	int srcHeight = pImage->Height();
	int srcPitch = srcWidth * FRAME_BUFFER_PIXSEL_NUM;
	if (cx > srcWidth)
	{
		assert(false);
		return NULL;
	}
	int dstWidth = cx;
	int dstHeight = (cy > srcHeight) ? cy : srcHeight;
	int dstPitch = dstWidth * FRAME_BUFFER_PIXSEL_NUM;
	int dstLen = dstPitch * dstHeight;
	assert(dstLen > 0);

#ifdef __TDHISI__
	if (bUseAppBuf)
	{
		assert (NULL != m_pImageDataAddr);

		//Copy
		BYTE* pSrcData = pImage->GetDate() + y * dstPitch + x * FRAME_BUFFER_PIXSEL_NUM;
		BYTE* pDstData = m_pImageDataAddr;

		for (int y = 0; y < dstHeight; ++y)
		{
			memcpy(pDstData, pSrcData, dstPitch);
			pDstData += dstPitch;
			pSrcData += srcPitch;
		}

		m_dataLen = dstLen;

		if (NULL != pLen)
		{
			*pLen = m_dataLen;
		}

		return m_pImageDataAddr;
	}
	else
	{
		m_pData = new BYTE[dstLen];
		assert (NULL != m_pData);

		//Copy
		BYTE* pSrcData = pImage->GetDate() + y * dstPitch + x * FRAME_BUFFER_PIXSEL_NUM;
		BYTE* pDstData = m_pData;

		for (int y = 0; y < dstHeight; ++y)
		{
			memcpy(pDstData, pSrcData, dstPitch);
			pDstData += dstPitch;
			pSrcData += srcPitch;
		}

		m_dataLen = dstLen;
	}
#else
	m_pData = new BYTE[dstLen];
	assert (NULL != m_pData);

	//Copy
	BYTE* pSrcData = pImage->GetDate() + y * dstPitch + x * FRAME_BUFFER_PIXSEL_NUM;
	BYTE* pDstData = m_pData;

	for (int y = 0; y < dstHeight; ++y)
	{
		memcpy(pDstData, pSrcData, dstPitch);
		pDstData += dstPitch;
		pSrcData += srcPitch;
	}

	m_dataLen = dstLen;
#endif

	if (NULL != pLen)
	{
		*pLen = m_dataLen;
	}

	return m_pData;
}

BYTE* CDrawManager::FillHorizontal(BITMAP_ID bmpID, int src_x, int dst_cx, int dst_cy, int * pLen /* = NULL */, bool bUseAppBuf/* = false*/)
{
#ifdef __TDHISI__
	if (bUseAppBuf)
	{
		m_pData = NULL;
	}
	else
	{
		if (NULL != m_pData)
		{
			delete [] m_pData;
			m_pData = NULL;	
		}
	}
#else
	//
	if (NULL != m_pData)
	{
		delete [] m_pData;
		m_pData = NULL;	
	}
#endif
	m_dataLen = 0;

	GUI::CImageList* pImageList = CImageList::Instance();
	GUI::CImage* pImage = pImageList->GetImage(bmpID);

	int srcWidth = pImage->Width();
	int srcHeight = pImage->Height();
	int srcPitch = srcWidth * FRAME_BUFFER_PIXSEL_NUM;
	
	int dstWidth = dst_cx;
	int dstHeight = (dst_cy > srcHeight) ? dst_cy : srcHeight;
	int dstPitch = dstWidth * FRAME_BUFFER_PIXSEL_NUM;
	int dstLen = dstPitch * dstHeight;
	assert(dstLen > 0);

#ifdef __TDHISI__
	if (bUseAppBuf)
	{
		assert (NULL != m_pImageDataAddr);

		//Copy
		const BYTE* pSrcData = pImage->GetDate() + src_x * FRAME_BUFFER_PIXSEL_NUM;
		BYTE* pDstData = NULL;

		for (int y = 0; y < dstHeight; ++y)
		{
			pDstData = m_pImageDataAddr + y * dstPitch;
			for (int x = 0; x < dstWidth; ++ x)
			{
				memcpy(pDstData, pSrcData, FRAME_BUFFER_PIXSEL_NUM);
				pDstData += FRAME_BUFFER_PIXSEL_NUM;
			}

			pSrcData += srcPitch;
		}

		m_dataLen = dstLen;

		if (NULL != pLen)
		{
			*pLen = m_dataLen;
		}

		return m_pImageDataAddr;
	}
	else
	{
		m_pData = new BYTE[dstLen];
		assert (NULL != m_pData);

		//Copy
		const BYTE* pSrcData = pImage->GetDate() + src_x * FRAME_BUFFER_PIXSEL_NUM;
		BYTE* pDstData = NULL;

		for (int y = 0; y < dstHeight; ++y)
		{
			pDstData = m_pData + y * dstPitch;
			for (int x = 0; x < dstWidth; ++ x)
			{
				memcpy(pDstData, pSrcData, FRAME_BUFFER_PIXSEL_NUM);
				pDstData += FRAME_BUFFER_PIXSEL_NUM;
			}

			pSrcData += srcPitch;
		}

		m_dataLen = dstLen;
	}
#else
	m_pData = new BYTE[dstLen];
	assert (NULL != m_pData);

	//Copy
	const BYTE* pSrcData = pImage->GetDate() + src_x * FRAME_BUFFER_PIXSEL_NUM;
	BYTE* pDstData = NULL;

	for (int y = 0; y < dstHeight; ++y)
	{
		pDstData = m_pData + y * dstPitch;
		for (int x = 0; x < dstWidth; ++ x)
		{
			memcpy(pDstData, pSrcData, FRAME_BUFFER_PIXSEL_NUM);
			pDstData += FRAME_BUFFER_PIXSEL_NUM;
		}

		pSrcData += srcPitch;
	}

	m_dataLen = dstLen;
#endif

	if (NULL != pLen)
	{
		*pLen = m_dataLen;
	}

	return m_pData;
}

void CDrawManager::StretchHorizontal(IMAGE_INFO& dst, const IMAGE_INFO& src, int middlePos) const
{
	if (src.width > dst.width)
	{
		assert(false);
		return;
	}
	int dstPitch = dst.width * FRAME_BUFFER_PIXSEL_NUM;
	int srcPitch = src.width * FRAME_BUFFER_PIXSEL_NUM;

	//可能外部给定的高度和原图高度不一致,所以这里应该取两者大者,避免内存违规.
	unsigned long height = (src.height > dst.height) ? src.height : dst.height;
	dst.len = dstPitch * height;
	assert(dst.len > 0);
	dst.pData = new BYTE[dst.len];

	//复制边缘
	BYTE* pDstData = dst.pData;
	const BYTE* pSrcData = src.pData;
	int copyLenBorder = (middlePos + 1)* FRAME_BUFFER_PIXSEL_NUM;

	////Left
	for (int y = 0; y < src.height; ++y)
	{	
		memcpy(pDstData, pSrcData, copyLenBorder);
		pDstData += dstPitch;
		pSrcData += srcPitch;
	}
	////Right
	int copyLenBorder_right = (src.width - middlePos - 1) * FRAME_BUFFER_PIXSEL_NUM;
	pDstData = dst.pData + dst.width * FRAME_BUFFER_PIXSEL_NUM - copyLenBorder_right;
	pSrcData = src.pData + src.width * FRAME_BUFFER_PIXSEL_NUM - copyLenBorder_right;

	for (int y = 0; y < src.height; ++y)
	{	
		memcpy(pDstData, pSrcData, copyLenBorder_right);
		pDstData += dstPitch;
		pSrcData += srcPitch;
	}
	////Center
	int center_w = dst.width - src.width;
	if (center_w <= 0)
	{
		return;
	}

	int center_scr_pos = middlePos * FRAME_BUFFER_PIXSEL_NUM;
	for (int y = 0; y < src.height; ++y)
	{
		pDstData = dst.pData + copyLenBorder + dstPitch * y;
		pSrcData = src.pData + center_scr_pos + srcPitch * y;

		for (int x = 0; x < center_w; ++x)
		{
			memcpy(pDstData, pSrcData, FRAME_BUFFER_PIXSEL_NUM);
			pDstData += FRAME_BUFFER_PIXSEL_NUM;
		}		
	}
}

void CDrawManager::StretchVertical(IMAGE_INFO& dst, const IMAGE_INFO& src, int verticalMiddle) const
{
	if (src.width > dst.width)
	{
		assert(false);
		return;
	}
	int dstPitch = dst.width * FRAME_BUFFER_PIXSEL_NUM;
	int srcPitch = src.width * FRAME_BUFFER_PIXSEL_NUM;

	dst.len = dstPitch * dst.height;
	assert(dst.len > 0);
	dst.pData = new BYTE[dst.len];

	//复制边缘
	BYTE* pDstData = dst.pData;
	const BYTE* pSrcData = src.pData;

	////Top
	for (int y = 0; y < verticalMiddle; ++y)
	{	
		memcpy(pDstData, pSrcData, srcPitch);
		pDstData += dstPitch;
		pSrcData += srcPitch;
	}
	////Middle
	//pDstData = dst.pData + dstPitch * center_pos;
	//pSrcData = src.pData + srcPitch * center_pos;
	int middle_end = dst.height - (src.height - verticalMiddle);

	for (int y = verticalMiddle; y < middle_end; ++y)
	{	
		memcpy(pDstData, pSrcData, srcPitch);
		pDstData += dstPitch;
	}
	////Bottom
	for (int y = middle_end; y < dst.height; ++y)
	{
		memcpy(pDstData, pSrcData, srcPitch);
		
		pDstData += dstPitch;
		pSrcData += srcPitch;
	}
}

void CDrawManager::CutVertical(IMAGE_INFO& dst, const IMAGE_INFO& src, int begin, int end) const
{
	if (src.width > dst.width)
	{
		assert(false);
		return;
	}
	int dstPitch = dst.width * FRAME_BUFFER_PIXSEL_NUM;
	int srcPitch = src.width * FRAME_BUFFER_PIXSEL_NUM;

	dst.len = dstPitch * dst.height;
	assert(dst.len > 0);
	dst.pData = new BYTE[dst.len];
	memset(dst.pData, 0, dst.len);

	BYTE* pDstData = dst.pData;
	const BYTE* pSrcData = src.pData;

	for (int y = 0; y < dst.height; ++y)
	{
		if ((y < begin) || (y > end))
		{
			memcpy(pDstData, pSrcData, dstPitch);
			pDstData += dstPitch;
		}
		pSrcData += srcPitch;
	}
}
