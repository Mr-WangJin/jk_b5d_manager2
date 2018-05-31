#pragma once

#include <vector>
#include <json/value.h>

class JKFileData;

class JKFilesData
{
public:
	JKFilesData();
	~JKFilesData();

	void initialize();

	JKFileData* getFileData(const int &idx);
	void addFile(JKFileData* pFileData);
	void deleteFile(const JKFileData* pFileData);
	void saveB5DFile();

	JKFileData* getB5DFile(const int &idx);
	int fileCount();

	inline JKFileData* operator [] (const int &idx) const;

	//����
	static bool upgrade(Json::Value& result);

	//���л�
	void serializable(Json::Value& result) noexcept;
	void deSerializable(Json::Value &) noexcept;


private:
	unsigned int version;				//���ݰ汾��

	std::vector<JKFileData*> m_VecFiles;
};

inline JKFileData* JKFilesData::operator [] (const int &idx) const
{
	return m_VecFiles[idx];
}
