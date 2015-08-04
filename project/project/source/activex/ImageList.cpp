#include "ImageList.h"
#include "DrawManager.h"

using namespace GUI;

const int DLG_COPY_POS_X = 20;
const int DLG_COPY_POS_Y = 47;
const int DLGCHILD_COPY_POS_X = 6;
const int DLGCHILD_COPY_POS_Y = 7;

GUI::CImageList::CImageList()
{
	
}

GUI::CImageList::~CImageList()
{

}

GUI::CImageList* GUI::CImageList::Instance()
{
	static GUI::CImageList imageList;
	return &imageList;
}

void GUI::CImageList::Initial()
{

}

void GUI::CImageList::Load()
{
	Clear();

#ifndef __RELEASE__
	m_memlen = 0;
#endif
	CreateFromFile();
	CreateFromMem();

#ifndef __RELEASE__
	printf("Image Mem Len:%dk, image num:%d\n", m_memlen/1024, m_imageList.size());
#endif
}


void GUI::CImageList::Clear()
{
	IMAGE_LIST::iterator iter;
	for (iter = m_imageList.begin(); iter != m_imageList.end(); ++iter)
	{
		delete iter->second;
		iter->second = NULL;
	}
	m_imageList.clear();
}

GUI::CImage* GUI::CImageList::GetImage(BITMAP_ID id)
{
	IMAGE_LIST::iterator iter = m_imageList.find(id);
	if (iter != m_imageList.end())
	{
		return iter->second;
	} 
	else
	{
		printf("(%s.%d)Error.Get image. Invalid id = 0x%X.\n", __FILE__, __LINE__, id);
		assert(false);
		return NULL;
	}
}

bool GUI::CImageList::CreateFromFile()
{
	char szCmd[512] = {0};
#if defined(__COMPRESS_UI_FONT__) 
#if defined(__CHIPTI__) || defined(__CHIP3521__) || defined(__CHIP3520A__) || defined(__CHIP3520D__)
	GetUI();
#else
	//�������������꣬��ʾui�е�ͼƬ����ѹ����ʽ����ui�е�*.bmp �� font.dat��ѹ����ui.tar
	//ʹ��ʱ���Ȱ�ui.tar��ѹ��/nfsdir��������ڴ棩,Ȼ���ٴ�/nfsdir�м���ͼƬ��
	//������Ŀ����Ϊ�˽�Լflash�ռ䡣����������ֻ��td3515���С�3520��flash�ռ�Ƚϣ��ʲ���������������
	int retVal = 0;
	
	snprintf(szCmd, 512, "mkdir /nfsdir/ui -p");
	do 
	{
		if (0 !=  DVRSystem(szCmd))
		{
			printf("%s:%s:%d, system(%s) error\n", __FUNCTION__, __FILE__, __LINE__, szCmd);
			PUB_Sleep(50);
		}
		else
		{
			break;
		}
	} while (1);
	
#ifdef __TDFH__
	PUB_Sleep(50);
#endif

	snprintf(szCmd, 512, "cd /nfsdir/ui && tar -xzf /mnt/mtd/ui/ui.tar");
	do 
	{
		if (0 != DVRSystem(szCmd))
		{
			printf("%s:%s:%d, system(%s) error\n", __FUNCTION__, __FILE__, __LINE__, szCmd);
			PUB_Sleep(50);
		}
		else
		{
			break;
		}
	} while (1);

#ifdef __TDFH__
	PUB_Sleep(50);
#endif


	snprintf(szCmd, 512, "cp /mnt/mtd/ui/icon_logo.bmp /nfsdir/ui/ -rf");
	
	do 
	{
		if (0 != DVRSystem(szCmd))
		{
			printf("%s:%s:%d, system(%s) error\n", __FUNCTION__, __FILE__, __LINE__, szCmd);
			PUB_Sleep(50);
		}
		else
		{
			break;
		}
	} while (1);

#ifdef __TDFH__
	PUB_Sleep(50);
#endif


	snprintf(szCmd, 512, "cp /mnt/mtd/ui/icon_logo_trans.bmp /nfsdir/ui/ -rf");
	do 
	{
		if (0 != DVRSystem(szCmd))
		{
			printf("%s:%s:%d, system(%s) error\n", __FUNCTION__, __FILE__, __LINE__, szCmd);
			PUB_Sleep(50);
		}
		else
		{
			break;
		}
	} while (1);

#ifdef __TDFH__
	PUB_Sleep(50);
#endif
#endif
#endif

	BITMAP_FILE bitmapFile;
	MapBitmapFile(bitmapFile);

	bool bClarity = false;

	BITMAP_FILE::const_iterator iter;
	for (iter = bitmapFile.begin(); iter != bitmapFile.end(); ++iter)
	{
		GUI::CImage* pImage = new GUI::CImage;

		if ((BMP_LIVE_AUDIO == iter->first) || (BMP_LIVE_MOTION == iter->first) ||\
			(BMP_LIVE_PTZ == iter->first) || (BMP_WAITTING == iter->first) ||\
			(BMP_LIVE_MOTION_BIG == iter->first) || (BMP_LIVE_SNAP_TIP == iter->first) ||\
			(BMP_LIVE_SNAP_TIP_FAIL == iter->first) || (BMP_ICON_LOGO_TRANS == iter->first))
		{
			bClarity = true;
		}
		else
		{
			bClarity = false;
		}

		if (pImage->LoadImage(iter->second.c_str(), bClarity))
		{
			m_imageList[iter->first] = pImage;
#ifndef __RELEASE__
			m_memlen += pImage->Size();
#endif
		}
		else
		{
			printf("No Image File: %s\n", iter->second.c_str());
			assert(false);

			delete pImage;
#ifdef __COMPRESS_UI_FONT__
			snprintf(szCmd, 512, "rm -rf /nfsdir/ui && cd ~");
			DVRSystem(szCmd);	
#endif
			return false;
		}
	}


#ifdef __COMPRESS_UI_FONT__
	snprintf(szCmd, 512, "rm -rf /nfsdir/ui && cd ~");
	DVRSystem(szCmd);	
#endif
	return true;
}

bool GUI::CImageList::InsertMemImage(BITMAP_ID id, const BYTE* pData, int len, int cx, int cy)
{
	assert(NULL != pData);

	CImage* pImage = new CImage;
	if (pImage->LoadImage(pData, len, cx, cy))
	{
		m_imageList[id] = pImage;
#ifndef __RELEASE__
		m_memlen += pImage->Size();
#endif
		return true;
	}
	else
	{
		assert(false);
		delete pImage;
		return false;
	}
}


bool GUI::CImageList::CreateFromMem()
{
	/*int count = sizeof(g_bitmapMem) / sizeof(g_bitmapMem[0]);
	for (int i = 0; i < count; ++i)
	{
		InsertDlgImage(g_bitmapMem[i].width, g_bitmapMem[i].height, g_bitmapMem[i].id);
	}*/

	CDrawManager drawMan;
	int len = 0;
	BYTE* pData = NULL;
	int width = 0;
	int height = 0;
	//////////////////////////////////////////////////////////////////////////

	//����������ϵ�������ȳ�ʼ��GUI::CWnd::SetSize(~,~);
	assert( 0 != GUI::CWnd::m_width );
	assert( 0 != GUI::CWnd::m_height );
	if( GUI::CWnd::m_width >= 1024 && GUI::CWnd::m_height >= 768 )
	{
		width = 960/*640*/;
		height = 600/*420*/;
		pData = drawMan.Stretch(BMP_DLG_BASE, width, height, DLG_COPY_POS_X, DLG_COPY_POS_Y, &len);
		InsertMemImage(BMP_MEM_DLG_LARGE, pData, len, width, height);
		//////////////////////////////////////////////////////////////////////////
	}
	else
	{
		width = 640;
		height = 420;
		pData = drawMan.Stretch(BMP_DLG_BASE, width, height, DLG_COPY_POS_X, DLG_COPY_POS_Y, &len);
		InsertMemImage(BMP_MEM_DLG_LARGE, pData, len, width, height);
		//////////////////////////////////////////////////////////////////////////
	}

	{
		//////////////////////////////////////////////////////////////////////////
		width = 640/*480*/;
		height = 420/*340*/;
		pData = drawMan.Stretch(BMP_DLG_BASE, width, height, DLG_COPY_POS_X, DLG_COPY_POS_Y, &len);
		InsertMemImage(BMP_MEM_DLG_NORMAL, pData, len, width, height);
		//////////////////////////////////////////////////////////////////////////
		width = 480/*380*/;
		height = 300/*260*/;
		pData = drawMan.Stretch(BMP_DLG_BASE, width, height, DLG_COPY_POS_X, DLG_COPY_POS_Y, &len);
		InsertMemImage(BMP_MEM_DLG_SMALL, pData, len, width, height);
		//////////////////////////////////////////////////////////////////////////
#ifndef __DVR_MENU__
		//width = GUI::CWnd::m_width;
		//height = 40;
		//pData = drawMan.Stretch(BMP_DLG_TOOLBAR, width, height, 0, CDrawManager::INVLAID, &len);
		//InsertMemImage(BMP_MEM_TOOLBAR, pData, len, width, height);
#endif
	}
	
	return false;
}

void GUI::CImageList::GetUI()
{
/*
	TI�Ľ�ѹ����
	#��ѹUI����
	rm /nfsdir/ui -rf 
	mkdir /nfsdir/ui -p
	cp /mnt/mtd/ui/ui.tar.lzma /nfsdir/
	cd /nfsdir/
	unlzma ui.tar.lzma
	cd ui
	tar xvf ../ui.tar
	cp /mnt/mtd/ui/icon_logo.bmp /nfsdir/ui/
	cp /mnt/mtd/ui/icon_logo_trans.bmp /nfsdir/ui/
	cd ..
	rm ui.tar
*/

#if defined(__CHIPTI__) || defined(__CHIP3521__) || defined(__CHIP3520A__) || defined(__CHIP3520D__)
	char szCmd[512] = {0};

	//����/nfsdir/uiĿ¼
	snprintf(szCmd, 512, "mkdir /nfsdir/ui -p");
	do 
	{
		if (0 !=  DVRSystem(szCmd))
		{
			printf("%s:%s:%d, system(%s) error\n", __FUNCTION__, __FILE__, __LINE__, szCmd);
			PUB_Sleep(50);
		}
		else
		{
			break;
		}
	} while (1);

	PUB_Sleep(50);

	//����lzma����
	snprintf(szCmd, 512, "cp /mnt/mtd/ui/ui.tar.lzma /nfsdir/");
	do 
	{
		if (0 != DVRSystem(szCmd))
		{
			printf("%s:%s:%d, system(%s) error\n", __FUNCTION__, __FILE__, __LINE__, szCmd);
			PUB_Sleep(50);
		}
		else
		{
			break;
		}
	} while (1);

	PUB_Sleep(50);

	//��ѹlzma����
	snprintf(szCmd, 512, "cd /nfsdir && unlzma ui.tar.lzma");
	do 
	{
		if (0 != DVRSystem(szCmd))
		{
			printf("%s:%s:%d, system(%s) error\n", __FUNCTION__, __FILE__, __LINE__, szCmd);
			PUB_Sleep(50);
		}
		else
		{
			break;
		}
	} while (1);

	PUB_Sleep(50);

	//��ѹtar����
	snprintf(szCmd, 512, "cd /nfsdir/ui && tar -xvf /nfsdir/ui.tar");
	do 
	{
		if (0 != DVRSystem(szCmd))
		{
			printf("%s:%s:%d, system(%s) error\n", __FUNCTION__, __FILE__, __LINE__, szCmd);
			PUB_Sleep(50);
		}
		else
		{
			break;
		}
	} while (1);
	PUB_Sleep(50);

	//ɾ��ui.tar
	snprintf(szCmd, 512, "rm /nfsdir/ui.tar");
	do 
	{
		if (0 != DVRSystem(szCmd))
		{
			printf("%s:%s:%d, system(%s) error\n", __FUNCTION__, __FILE__, __LINE__, szCmd);
			PUB_Sleep(50);
		}
		else
		{
			break;
		}
	} while (1);
	PUB_Sleep(50);

	//����logo
	snprintf(szCmd, 512, "cp /mnt/mtd/ui/icon_logo.bmp /nfsdir/ui/ -rf");
	do 
	{
		if (0 != DVRSystem(szCmd))
		{
			printf("%s:%s:%d, system(%s) error\n", __FUNCTION__, __FILE__, __LINE__, szCmd);
			PUB_Sleep(50);
		}
		else
		{
			break;
		}
	} while (1);

	PUB_Sleep(50);

	snprintf(szCmd, 512, "cp /mnt/mtd/ui/icon_logo_trans.bmp /nfsdir/ui/ -rf");
	do 
	{
		if (0 != DVRSystem(szCmd))
		{
			printf("%s:%s:%d, system(%s) error\n", __FUNCTION__, __FILE__, __LINE__, szCmd);
			PUB_Sleep(50);
		}
		else
		{
			break;
		}
	} while (1);

	PUB_Sleep(50);
#endif
}

