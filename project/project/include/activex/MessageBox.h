#ifndef _MESSAGE_BOX_H_
#define _MESSAGE_BOX_H_

#include "Dialog.h"
#include "Static.h"
#include "PictureCtrl.h"
#include "RichTextCtrl.h"

namespace GUI
{
	class CMessageBox : public GUI::CDialog
	{
	public:
		CMessageBox();
		CMessageBox(const char* szText,const char* szCaption,unsigned long type);
		virtual ~CMessageBox();

	protected:
		virtual void OnInitial();
		void OnPaint(PAINT_STATUS status);
	private:
		BITMAP_ID GetImageID(unsigned long type) const;

		//GUI::CStatic m_staticText;
		GUI::CPictureCtrl m_picture;
		GUI::CRichTextCtrl m_richTextCtrl;

		std::string m_strText;
	};
}

#endif

