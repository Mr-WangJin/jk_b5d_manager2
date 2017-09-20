#include "JKFileData.h"
#include <QFile>
#include <QDir>
#include <QDomDocument>
#include <QTextStream>

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

bool JKFileData::isOffice()
{
	if (cloudStr.empty())
	{
		std::string str = this->getFullPath();
		QDir dir(QString::fromStdString(str));
		dir.cdUp();

		QString fileName = dir.path() + "\\BIM5D.xml";
		QFile file(fileName);
		if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
		{
			file.close();
			std::string error = "can not find" + fileName.toStdString();
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
			std::string error = "can not find CloudService_v1 ";
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
		std::string error = "can not find" + fileName.toStdString();
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
		std::string error = "can not find" + fieldName.toStdString();
		throw std::exception(error.c_str());
	}
	QDomElement ele = lists.at(0).toElement();

	if ("update" == opt)
	{
		ele.toElement().firstChild().setNodeValue(value);
		if (!file.open(QIODevice::WriteOnly | QIODevice::Text))
		{
		}
		QTextStream out(&file);
		doc.save(out, 4);
		file.close();

		cloudStr = value.toStdString();
	}
}