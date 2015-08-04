/*
* Copyright (C) Tongwei Video Technology Co.,Ltd. All rights reserved.
* Author       : xian
* Date         : 2014-09-16
* Name         : CruiseOther.h
* Version      : 1.0
* Description  : 
* Modify Record:
*/

#include <vector>

class CruiseDelData{
	friend class CCfgDlg;
	friend class CCfgPtzCruiseDlg;
private:
	CruiseDelData(int deleteInfo,
    int currchannel):m_deleteInfo(deleteInfo),m_currchannel(currchannel){}
	/*
	| 31...........16|15...............8|7..............0 |
	|                |是否点击OK键      |代表0到7号巡航线 |
	*/
	unsigned int m_deleteInfo;
	unsigned int m_currchannel;
};

class CCruiseDelete{
	friend class CCfgDlg;
	friend class CCfgPtzCruiseDlg;
public:
	static CCruiseDelete * GetInstanceCruiseDel(){
        static CCruiseDelete instance;
		return &instance;
	}
	//~CCruiseDelete(){
	//	((CCruiseDelete *)m_pCruiseDel)->m_CruiseDel->clear();
	//	delete m_pCruiseDel;
	//}

private:
	CCruiseDelete(){}
private:
	std::vector<CruiseDelData> m_CruiseDel;
};
