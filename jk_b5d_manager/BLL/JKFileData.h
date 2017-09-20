#pragma once

#include <iostream>
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

	bool isOffice();
	void updateXMLNode(const QString &fileName, const QString & opt, const QString & fieldName, const QString & value);

private:
	std::string m_FileName;
	std::string m_FullPath;
	std::string m_VersionNum;

	std::string cloudStr;
};


