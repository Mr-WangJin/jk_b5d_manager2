#include "JKFileData.h"
#include <QFile>
#include <QDir>
#include <QDomDocument>
#include <QTextStream>

JKFileData::JKFileData()
{
}

JKFileData::JKFileData(const JKString &_fileName, const  JKString &_version,
	const JKString _remark, const JKString &_fullPath)
	: fileName(_fileName)
	, fullPath(_fullPath)
	, version(_version)
	, remark(_remark)
{
}

JKFileData::~JKFileData()
{
}

void JKFileData::setFileName(const JKString & _fileName)
{
	fileName = _fileName;
}

JKString JKFileData::getFileName()
{
	return fileName;
}

void JKFileData::setVersion(const JKString & versionNum)
{
	version = versionNum;
}

JKString JKFileData::getVersion()
{
	return version;
}

void JKFileData::setFullPath(const JKString & path)
{
	fullPath = path;
}

JKString JKFileData::getFullPath()
{
	return fullPath;
}

void JKFileData::setRemark(const JKString &_remark)
{
	remark = _remark;
}
JKString JKFileData::getRemark()
{
	return remark;
}

bool JKFileData::isOffice()
{
	if (cloudStr.empty())
	{
		JKString str = this->getFullPath();
		QDir dir(QString::fromStdString(str));
		dir.cdUp();

		QString fileName = dir.path() + "\\BIM5D.xml";
		QFile file(fileName);
		if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
		{
			file.close();
			JKString error = "can not find" + fileName.toStdString();
			throw std::exception(error.c_str());
		}

		QDomDocument doc;
		if (!doc.setContent(&file))
		{
			file.close();
			throw std::exception("can not open file.");
		}
		file.close();
		QDomNodeList lists = doc.elementsByTagName("CloudService_v1");
		if (lists.count() == 0)
		{
			JKString error = "can not find CloudService_v1 ";
			throw std::exception(error.c_str());
		}
		QDomElement ele = lists.at(0).toElement();

		cloudStr = ele.toElement().firstChild().nodeValue().toStdString();
	}
	
	return cloudStr == "https://bim5d.glodon.com/api/v1/" || cloudStr == "https://bim5d.glodon.com/api/v3/";

}

void JKFileData::updateXMLNode(const QString & fileName, const QString & opt, const QString & fieldName, const QString & value)
{
	QFile file(fileName);
	if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
	{
		file.close();
		JKString error = "can not find" + fileName.toStdString();
		throw std::exception(error.c_str());
	}

	QDomDocument doc;
	if (!doc.setContent(&file))
	{
		file.close();
		throw std::exception("can not open file.");
	}
	file.close();
	QDomNodeList lists = doc.elementsByTagName(fieldName);
	if (lists.count() == 0)
	{
		JKString error = "can not find" + fieldName.toStdString();
		throw std::exception(error.c_str());
	}
	if ("update" == opt)
	{
		for (int i = 0; i < lists.size(); i++)
		{
			QDomElement ele = lists.at(i).toElement();
			ele.toElement().firstChild().setNodeValue(value);
		}
		if (!file.open(QIODevice::WriteOnly | QIODevice::Text))
		{
		}
		QTextStream out(&file);
		doc.save(out, 4);
		file.close();

		cloudStr = value.toStdString();
	}
}

bool JKFileData::upgrade(const unsigned int &dataVersion, Json::Value& result)
{
	if (dataVersion < 1)
	{
		result["remark"] = result["versionNum"];
		result["version"] = "";
		result["versionNum"] = nullptr;
	}

	return true;
}

void JKFileData::serializable(Json::Value & result) noexcept
{
	result["name"] = fileName;
	result["path"] = fullPath;
	result["version"] = version;
	result["remark"] = remark;
}

void JKFileData::deSerializable(Json::Value & result) noexcept
{
	setFileName(result["name"].asString());
	std::string path = result["path"].asString();
	setFullPath(path);
	setRemark(result["remark"].asString());
	setVersion(result["version"].asString());
}
