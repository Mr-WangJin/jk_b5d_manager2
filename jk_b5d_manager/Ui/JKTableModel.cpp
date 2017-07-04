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
	if (!index.isValid() || role != Qt::DisplayRole)
		return QVariant();
	JKFileData* pFileData = m_pFilesData->getFileData(index.row());
	
	switch (index.column())
	{
	case 0:
	{
		QString tempStr(pFileData->getFileName().c_str());
		return QVariant(tempStr);
	}
	default:
		break;
	}
	return QVariant();

}

QVariant JKTableModel::headerData(int section, Qt::Orientation orientation, int role) const
{
	return QVariant();
}
