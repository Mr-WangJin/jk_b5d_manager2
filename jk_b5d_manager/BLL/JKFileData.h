#pragma once

#include <QString>

class JKFileData
{
public:
	JKFileData();
	JKFileData(const std::string &fineName, const  std::string &versionNum, const std::string &fullPath);
	~JKFileData();

	void setFileName(const std::string &fileName);
	std::string getFileName();

	void setVersionNum(const std::string &versionNum);
	std::string getVersionNum();

	void setFullPath(const std::string &path);
	std::string getFullPath();

private:
	std::string m_FileName;
	std::string m_FullPath;
	std::string m_VersionNum;

};


