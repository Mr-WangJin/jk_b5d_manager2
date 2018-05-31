#pragma once

#include <iostream>
#include <QString>
#include <JKCommon/JKCommon.h>
#include <json/value.h>

USING_JK_NAMESPACE

class JKFileData
{
public:
	JKFileData();
	JKFileData(const JKString &_fileName, const  JKString &_version,
		const JKString _remark, const JKString &_fullPath);
	~JKFileData();

	void setFileName(const JKString &_fileName);
	JKString getFileName();

	void setVersion(const JKString &versionNum);
	JKString getVersion();

	void setFullPath(const JKString &path);
	JKString getFullPath();

	void setRemark(const JKString &_remark);
	JKString getRemark();

	bool isOffice();
	void updateXMLNode(const QString &fileName, const QString & opt, const QString & fieldName, const QString & value);

	//升级
	static bool upgrade(const unsigned int &dataVersion, Json::Value& result);

	//序列化
	void serializable(Json::Value& result) noexcept;
	void deSerializable(Json::Value &) noexcept;

private:
	JKString fileName;
	JKString fullPath;
	JKString version;
	JKString remark;

	/** 业务临时变量 */
	JKString cloudStr;
};


