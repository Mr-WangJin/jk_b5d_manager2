#include "JKFilesData.h"
#include "JKFileData.h"
#include <json/reader.h>
#include <JKFile/JKFileIO.h>


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
	JKString data;
	size_t dataLen = 0;
	
	if (JKFileIO::ReadFile(B5D_JSON_DIR, JKFileIO::Read, data, dataLen))
	{
		Json::Reader reader;
		Json::Value result;
		reader.parse(data, result);

		this->upgrade(result);
		this->deSerializable(result);
	}

	
}

JKFileData * JKFilesData::getFileData(const int & idx)
{
	return m_VecFiles[idx];
}

void JKFilesData::addFile(JKFileData* pFileData)
{
	assert(pFileData);

	m_VecFiles.push_back(pFileData);
}

void JKFilesData::deleteFile(const JKFileData * pFileData)
{
	std::vector<JKFileData*>::iterator iterTemp = std::find(m_VecFiles.begin(), m_VecFiles.end(), pFileData);
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

	this->serializable(root);

	std::string out = root.toStyledString();
	JKFileIO::WriteFile(B5D_JSON_DIR, JKFileIO::Write_Plus, out, out.size());
}


JKFileData * JKFilesData::getB5DFile(const int &idx)
{
	if (idx >= m_VecFiles.size())
		return nullptr;
	return m_VecFiles[idx];
}

void JKFilesData::serializable(Json::Value& result) noexcept
{
	Json::Value arrayObj;
	for each (JKFileData* pB5DFile in m_VecFiles)
	{
		Json::Value item;
		pB5DFile->serializable(item);
		arrayObj.append(item);

	}
	result["version"] = version;
	result["B5DFiles"] = arrayObj;
}
void JKFilesData::deSerializable(Json::Value& result) noexcept
{
	Json::Value b5DFiles = result["B5DFiles"];
	version = result["version"].asUInt();
	for (int i = 0; i < b5DFiles.size(); ++i)
	{
		Json::Value b5dFile = b5DFiles[i];

		JKFileData* pB5DFile = new JKFileData;
		pB5DFile->deSerializable(b5dFile);

		m_VecFiles.push_back(pB5DFile);
	}
}

bool JKFilesData::upgrade(Json::Value& result)
{
	int curVersion = 0;

	if (!result["version"].isNull())
		curVersion = result["version"].asUInt();

	if (curVersion < 1)
	{
		result["version"] = 1;
	}


	Json::Value b5DFiles = result["B5DFiles"];
	for (int i = 0; i < b5DFiles.size(); ++i)
	{
		Json::Value b5dFile = b5DFiles[i];

		JKFileData::upgrade(curVersion, b5dFile);
	}

	return true;
}

int JKFilesData::fileCount()
{
	return m_VecFiles.size();
}
