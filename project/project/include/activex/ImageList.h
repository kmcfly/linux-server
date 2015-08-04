#ifndef _IMAGE_LIST_H_
#define _IMAGE_LIST_H_
#include <map>
#include "Image.h"
#include "BitmapID.h"

namespace GUI
{
	typedef std::map<int, CImage*>  IMAGE_LIST;
	class CImageList
	{	
	public:
		static CImageList* Instance();
		~CImageList();

		void Initial();

		void Load();
		void Clear();

		CImage* GetImage(BITMAP_ID id);

	protected:
		bool CreateFromFile();
		bool CreateFromMem();
		bool InsertMemImage(BITMAP_ID id, const BYTE* pData, int len, int cx, int cy);


		IMAGE_LIST m_imageList;
	private:
		CImageList();
		void GetUI();

#ifndef __RELEASE__
		int m_memlen;
#endif
	};
}

#endif

