/*
������FTP UI������ǰ��������ϰ�����״̬����
���Խ���Edit����ʾ�����ݽ��д���ͱ����

ע�⣺windows�»�ȡ�ļ������ļ���·��Ϊ���»���
��linux��FTPΪ���»���

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
	void OnButtonPre();					//����Pre���Ĳ���
	void OnButtonNext();				//����Next���Ĳ���
	void OnButtonUp();					//����Up���Ĳ���
	void OnDBClickFolder(const char *CurrentName);				//˫���Ĳ���
	bool IsPreEnable();					//�ж�Pre�Ƿ�Ҫ��ʾ  typeΪ��ǰ���µļ�
	bool IsNextEnable();				//�ж�Next�Ƿ�Ҫ��ʾ	typeΪ��ǰ���µļ�
	bool IsUpEnable();					//�ж�Up�Ƿ�Ҫ��ʾ ע��:Ҫ�ȵ���OnButtonUp()����ˢ��
	string GetCurPath();				//ȡ�õ�ǰ·��
	string USBGetEditPath();			//����USB��ʾ
	void OnPreError();
	void OnNextError();
		
protected:
	string GetPrePathName(string PathName);
	bool IsEndObliqueLine(const char *str);
	void DeleteNextList();
private:
	std::string					m_strCurrentPath;//Edit����ʾ��·��
	std::string					m_strlastSuccussPath;//��һ�γɹ���ʾ��·��
	std::string					m_strRootPath;//�����·��������ǰѡ���ĳһU�̵ĸ�·��

	std::list<string>			m_listPre;//������˰�ť��·��
	std::list<string>			m_listNext;//����ǰ����ť��·��
	
};

#endif
