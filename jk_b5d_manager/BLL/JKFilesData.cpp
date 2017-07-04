#include "JKFilesData.h"
#include "JKFileData.h"
#include <json/reader.h>
#include <json/value.h>
#include <File/JKFile.h>

using namespace JK_NAMESPACE;

const JKString B5D_JSON_DIR(_str("b5d.json"));

JKFilesData::JKFilesData()
{
	this->initialize();
}

JKFilesData::~JKFilesData()
{
	for each (JKFileData* file in m_VecFiles)
	{
		delete file;
	}
	m_VecFiles.clear();
}

void JKFilesData::initialize()
{
// 	wxTextFile tfile;
// 	tfile.Open(B5D_JSON_DIR);
// 	m_jsonStr += tfile.GetFirstLine();
// 	while (!tfile.Eof())
// 	{
// 		m_jsonStr += tfile.GetNextLine();
// 	}

	JKString data;
	size_t dataLen = 0;
	JKFile::ReadFile(B5D_JSON_DIR, JKFile::Read, data, dataLen);


	Json::Reader reader;
	Json::Value result;
	reader.parse(data, result);

	Json::Value b5DFiles = result["B5DFiles"];
	for (int i = 0; i< b5DFiles.size(); ++i)
	{
		Json::Value b5dFile = b5DFiles[i];
		
		JKFileData* pB5DFile = new JKFileData;
		pB5DFile->setFileName(b5dFile["name"].asString());
		pB5DFile->setFullPath(b5dFile["path"].asString());
		pB5DFile->setVersionNum(b5dFile["versionNum"].asString());

		m_VecFiles.push_back(pB5DFile);
	}
}

JKFileData * JKFilesData::getFileData(const int & idx)
{
	return m_VecFiles[idx];
}

void JKFilesData::addFile(JKFileData* pB5DFile)
{
	assert(pB5DFile);

	m_VecFiles.push_back(pB5DFile);
}

void JKFilesData::deleteFile(const int & idx)
{
	if (m_VecFiles.size() <= idx)
		return;

	std::vector<JKFileData*>::iterator iterTemp = std::find(m_VecFiles.begin(), m_VecFiles.end(), m_VecFiles[idx]);
	if (iterTemp != m_VecFiles.end())
	{
		m_VecFiles.erase(iterTemp);
	}
}

bool WriteFile(const char* fileName, const char *_Mode, const char *buffer, const long &lSize)
{
	FILE *pFile;
	pFile = fopen(fileName, _Mode);
	if (pFile == NULL) { fputs("File error", stderr); return false; }

	fwrite(buffer, sizeof(char), lSize, pFile);

	fclose(pFile);
	return true;
}

void JKFilesData::saveB5DFile()
{
	Json::Value root;
	Json::Value arrayObj;

	for each (JKFileData* pB5DFile in m_VecFiles)
	{
		Json::Value item;
		item["name"] = pB5DFile->getFileName();
		item["path"] = pB5DFile->getFullPath();
		item["versionNum"] = pB5DFile->getVersionNum();
		arrayObj.append(item);
	}

	root["B5DFiles"] = arrayObj;

	std::string out = root.toStyledString();
	JKFile::WriteFile(B5D_JSON_DIR, JKFile::Write_Plus, JKString(), out.size());
}


JKFileData * JKFilesData::getB5DFile(const int &idx)
{
	return m_VecFiles[idx];
}

int JKFilesData::fileCount()
{
	return m_VecFiles.size();
}
