#include "JKFileData.h"

JKFileData::JKFileData()
{
}

JKFileData::JKFileData(const std::string &fineName, const  std::string &versionNum, const std::string &fullPath)
	: m_FileName(fineName)
	, m_FullPath(fullPath)
	, m_VersionNum(versionNum)
{
}

JKFileData::~JKFileData()
{
}

void JKFileData::setFileName(const std::string & fileName)
{
	m_FileName = fileName;
}

std::string JKFileData::getFileName()
{
	return m_FileName;
}

void JKFileData::setVersionNum(const std::string & versionNum)
{
	m_VersionNum = versionNum;
}

std::string JKFileData::getVersionNum()
{
	return m_VersionNum;
}

void JKFileData::setFullPath(const std::string & path)
{
	m_FullPath = path;
}

std::string JKFileData::getFullPath()
{
	return m_FullPath;
}
