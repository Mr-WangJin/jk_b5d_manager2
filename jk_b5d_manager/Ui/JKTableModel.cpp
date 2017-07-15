#include "JKTableModel.h"
#include "BLL/JKFilesData.h"
#include "BLL/JKFileData.h"

JKTableModel::JKTableModel(QObject* parent /* = nullptr */)
	: QAbstractTableModel(parent)
{

}

JKTableModel::~JKTableModel()
{
}

void JKTableModel::addFileData(JKFileData * pFileData)
{
	int fileCount = m_pFilesData->fileCount();
	beginInsertRows(QModelIndex(), fileCount, fileCount);
	m_pFilesData->addFile(pFileData);
	endInsertRows();
}

void JKTableModel::delFileData(JKFileData * pFileData)
{
	beginResetModel();
	m_pFilesData->deleteFile(pFileData);
	endResetModel();
}

void JKTableModel::setFilesData(JKFilesData * ptr)
{
	beginResetModel();
	m_pFilesData = ptr;
	endResetModel();
}

int JKTableModel::rowCount(const QModelIndex & parent) const
{
	if (m_pFilesData == nullptr)
		return 0;
	else
		return m_pFilesData->fileCount();
}

int JKTableModel::columnCount(const QModelIndex & parent) const
{
	return 3;
}

QVariant JKTableModel::data(const QModelIndex & index, int role) const
{
	if (!index.isValid() || (role != Qt::DisplayRole && role != Qt::EditRole))
		return QVariant();
	JKFileData* pFileData = m_pFilesData->getFileData(index.row());
	
	switch (index.column())
	{
	case 0:
	{
		QString tempStr(pFileData->getFileName().c_str());
		return QVariant(tempStr);
	}
	case 1:
	{
		QString tempStr(pFileData->getVersionNum().c_str());
		return QVariant(tempStr);
	}
	case 2:
	{
		QString tempStr(pFileData->getFullPath().c_str());
		return QVariant(tempStr);
	}
	default:
		break;
	}
	return QVariant();

}

QVariant JKTableModel::headerData(int section, Qt::Orientation orientation, int role) const
{
	if (orientation != Qt::Horizontal)
		return QVariant();

	if (role != Qt::DisplayRole)
		return QVariant();
	switch (section)
	{
	case 0:
	{
		return QVariant(QStringLiteral("Ãû³Æ"));
	}
	case 1:
	{
		return QVariant(QStringLiteral("×¢ÊÍ"));
	}
	case 2:
	{
		return QVariant(QStringLiteral("Â·¾¶"));
	}
	default:
		break;
	}
	return QVariant();
}

bool JKTableModel::setData(const QModelIndex & index, const QVariant & value, int role)
{
	JKFileData* pFileData = m_pFilesData->getFileData(index.row());
	if (pFileData == nullptr)
		return false;
	switch (index.column())
	{
	case 0:
	{
		pFileData->setFileName(value.toString().toStdString());
	}
	case 1:
	{
		pFileData->setVersionNum(value.toString().toStdString());
	}
	default:
		break;
	}
	return true;
}

Qt::ItemFlags JKTableModel::flags(const QModelIndex & index) const
{
	if (!index.isValid())
		return Qt::NoItemFlags;

	return  QAbstractTableModel::flags(index) | Qt::ItemIsEditable;
}
