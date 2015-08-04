/*
适用于FTP UI界面向前、向后、向上按键的状态分析
并对界面Edit上显示的内容进行处理和保存等

注意：windows下获取文件夹下文件的路径为右下划线
而linux和FTP为左下划线

*/
#ifndef __FTP_BUTTON_STATE__
#define __FTP_BUTTON_STATE__

#include	<list>
#include	<string.h>
#include	<string>

using namespace std;

typedef enum
{
	PRE,
	NEXT,
	UP,
	FOLDER,
}FTP_BTN_TYPE;

class CFTPWidgetInfoMan
{
public:
	CFTPWidgetInfoMan();
	~CFTPWidgetInfoMan();
	
	void Initial(const char *InitPath);
	void OnButtonPre();					//按下Pre键的操作
	void OnButtonNext();				//按下Next键的操作
	void OnButtonUp();					//按下Up键的操作
	void OnDBClickFolder(const char *CurrentName);				//双击的操作
	bool IsPreEnable();					//判断Pre是否要显示  type为当前按下的键
	bool IsNextEnable();				//判断Next是否要显示	type为当前按下的键
	bool IsUpEnable();					//判断Up是否要显示 注意:要先调用OnButtonUp()进行刷新
	string GetCurPath();				//取得当前路径
	string USBGetEditPath();			//方便USB显示
	void OnPreError();
	void OnNextError();
		
protected:
	string GetPrePathName(string PathName);
	bool IsEndObliqueLine(const char *str);
	void DeleteNextList();
private:
	std::string					m_strCurrentPath;//Edit上显示的路径
	std::string					m_strlastSuccussPath;//上一次成功显示的路径
	std::string					m_strRootPath;//保存根路径，即当前选择的某一U盘的根路径

	std::list<string>			m_listPre;//保存后退按钮的路径
	std::list<string>			m_listNext;//保存前进按钮的路径
	
};

#endif
