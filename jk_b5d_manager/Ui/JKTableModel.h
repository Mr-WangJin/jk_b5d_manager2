#pragma once

#include <QAbstractTableModel>

class JKFilesData;

class JKTableModel : public QAbstractTableModel
{
	Q_OBJECT

public:
	JKTableModel(QObject* parent = nullptr);
	virtual ~JKTableModel();

	void setFilesData(JKFilesData* ptr);

	int rowCount(const QModelIndex &parent = QModelIndex()) const override;
	int columnCount(const QModelIndex &parent = QModelIndex()) const override;

	QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;
	QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const override;

private:
	JKFilesData* m_pFilesData;

};