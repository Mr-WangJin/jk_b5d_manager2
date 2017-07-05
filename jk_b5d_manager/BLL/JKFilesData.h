#pragma once

#include <vector>

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

private:
	std::string m_jsonStr;

	std::vector<JKFileData*> m_VecFiles;
};

inline JKFileData* JKFilesData::operator [] (const int &idx) const
{
	return m_VecFiles[idx];
}
