#include "JKTableModel.h"
#include "BLL/JKFilesData.h"
#include "BLL/JKFileData.h"
#include <QColor>

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
	return 4;
}

QVariant JKTableModel::data(const QModelIndex & index, int role) const
{
	if (!index.isValid() || (role != Qt::DisplayRole && role != Qt::EditRole && role != Qt::BackgroundColorRole))
		return QVariant();
	JKFileData* pFileData = m_pFilesData->getFileData(index.row());
	if (role == Qt::DisplayRole || role == Qt::EditRole)
	{
		switch (index.column())
		{
		case 0:
		{
			QString tempStr(pFileData->getFileName().c_str());
			return QVariant(tempStr);
		}
		case 1:
		{
			QString tempStr(pFileData->getVersion().c_str());
			return QVariant(tempStr);
		}
		case 2:
		{
			QString tempStr(pFileData->getRemark().c_str());
			return QVariant(tempStr);
		}
		case 3:
		{
			QString tempStr(pFileData->getFullPath().c_str());
			return QVariant(tempStr);
		}
		default:
			break;
		}
	}
	else if (role == Qt::BackgroundColorRole)
	{
		try {
			if (pFileData->isOffice())
				return QColor(Qt::green);
		}
		catch (std::exception &e)
		{

		}
	}
	
	return QVariant();
}

QVariant JKTableModel::headerData(int section, Qt::Orientation orientation, int role) const
{
	if (orientation != Qt::Horizontal)
		return QVariant();

	if (role == Qt::DisplayRole)
	{
		switch (section)
		{
		case 0:
		{
			return QVariant(QStringLiteral("Ãû³Æ"));
		}
		case 1:
		{
			return QVariant(QStringLiteral("°æ±¾ºÅ"));
		}
		case 2:
		{
			return QVariant(QStringLiteral("×¢ÊÍ"));
		}
		case 3:
		{
			return QVariant(QStringLiteral("Â·¾¶"));
		}
		default:
			break;
		}
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
		break;
	}
	case 1:
	{
		pFileData->setVersion(value.toString().toStdString());
		break;
	}
	case 2:
	{
		pFileData->setRemark(value.toString().toStdString());
		break;
	}
	case 3:
	{
		pFileData->setFullPath(value.toString().toStdString());
		break;
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

