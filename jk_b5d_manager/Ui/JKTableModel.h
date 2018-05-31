#pragma once

#include <QAbstractTableModel>

class JKFilesData;
class JKFileData;

class JKTableModel : public QAbstractTableModel
{
	Q_OBJECT

public:
	JKTableModel(QObject* parent = nullptr);
	virtual ~JKTableModel();

	void addFileData(JKFileData* pFileData);
	void delFileData(JKFileData* pFileData);
	void setFilesData(JKFilesData* ptr);

	int rowCount(const QModelIndex &parent = QModelIndex()) const override;
	int columnCount(const QModelIndex &parent = QModelIndex()) const override;

	QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;
	QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const override;

	virtual bool setData(const QModelIndex &index, const QVariant &value, int role = Qt::EditRole) override;

	Qt::ItemFlags flags(const QModelIndex &index) const override;

private:
	JKFilesData* m_pFilesData;

};