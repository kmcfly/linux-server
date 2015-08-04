#ifndef _INI_READER_H_
#define _INI_READER_H_

#include <assert.h>
#include <algorithm>
#include <iostream>
#include <fstream>
#ifdef __ENVIRONMENT_WIN32__
#include <string>
#else
#include <string.h>
#endif
#include <map>

class CIniReader
{
public:
	CIniReader();
	~CIniReader();

	bool Initial(const char* pPath);
	void Quit();

	std::string GetKeyValue(const std::string strKey);

protected:
	void Trim(std::string& str , const std::string& chars = " \t");
	bool LoadIniFile(const char* pFileName);

protected:
	typedef std::map<std::string, std::string> INI_NODE_MAP;

	INI_NODE_MAP	m_node_map;

	static std::string s_strPath;
};

#endif

//end
