#include "IniReader.h"
#include <assert.h>
#include <algorithm>
#include <iostream>
#include <fstream>
#ifdef __ENVIRONMENT_WIN32__
#include <string>
#endif

#ifdef __ENVIRONMENT_WIN32__
std::string CIniReader::s_strPath = "\\ui\\color.ini";
#else
std::string CIniReader::s_strPath = "/ui/color.ini";
#endif

CIniReader::CIniReader()
{

}

CIniReader::~CIniReader()
{
	Quit();
}

bool CIniReader::Initial(const char* pPath)
{
	Quit();

	char filename[512] = {0};

	if (NULL != pPath)
	{
		//方便linux下查找文件路径,末端都没带'\'或'/'
		strcpy(filename, pPath);
		int len = strlen(filename);
#ifdef __ENVIRONMENT_WIN32__
		if ('/' == filename[len - 1] || '\\' == filename[len - 1] )
		{
			filename[len - 1] = '\0';
		}
		strcat(filename, s_strPath.c_str());
#else
		if ('/' == filename[len - 1])
		{
			filename[len - 1] = '\0';
		}
		strcat(filename, s_strPath.c_str());
#endif
	}
	else
	{
#ifdef __ENVIRONMENT_WIN32__
		strcpy(filename, s_strPath.c_str());
#else
		strcpy(filename, s_strPath.c_str());
#endif
	}

	return LoadIniFile(filename);
}

void CIniReader::Quit()
{
	m_node_map.clear();
}

std::string CIniReader::GetKeyValue(const std::string strKey)
{
	INI_NODE_MAP::iterator iter = m_node_map.find(strKey);
	if(iter != m_node_map.end())
	{
		return iter->second;
	}
	return "";
}

void CIniReader::Trim(std::string& str , const std::string& chars)
{
	str.erase(str.find_last_not_of(chars)+1);
	str.erase(0, str.find_first_not_of(chars));
}

bool CIniReader::LoadIniFile(const char* pFileName)
{
	std::ifstream infile;

	infile.open(pFileName, std::ios::binary);

	if(!infile.is_open())
	{
		return false;
	}

	std::string sRead;
	while(std::getline(infile ,sRead))
	{
		Trim(sRead);

		if(!sRead.empty())
		{
			unsigned int nType = (sRead.find_first_of("[") == 0 && (sRead[sRead.find_last_not_of(" \t\r\n")] == ']')) ? 1 : 0; //Section
			int pos = sRead.find_first_of("=");
			nType = ((nType == 0) && (pos > 0)) ? 2 : nType;	//有效key
			nType = (sRead.find_first_of("#") == 0) ? 3 : nType;	//注释

			if(nType == 2)
			{
				std::string sKey = sRead.substr(0, pos);
				std::string sValue = sRead.substr(pos + 1);

				m_node_map.insert(std::make_pair(sKey, sValue));
			}
		}
	}

	infile.close();
	return true;
}

//end
