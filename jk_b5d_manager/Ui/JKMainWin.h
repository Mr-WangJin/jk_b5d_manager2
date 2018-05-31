#ifndef JKMAINWIN_H
#define JKMAINWIN_H

#include <QMainWindow>
#include "ui_JKMainWin.h"

class JKTableModel;
class JKFilesData;

class JKMainWin : public QMainWindow
{
	Q_OBJECT

public:
	JKMainWin(QWidget *parent = 0);
	~JKMainWin();

	static bool deleteDir(const QString &dirName);

public slots:
	void onAddFile();
	void onBatchAdded();
	void onDeleteFile();
	void onUninstall();
	void onSave();
	void onOpenDir();
	void onDelUnis();
	void onDelUnableFiles();

	void onSetOffice();
	void onSetHuNan();

	void onRunExe();
	void onRunTool();
	void onRunCraft();

	void onTableViewContextMenuClicked(QPoint);

protected:
	void keyPressEvent(QKeyEvent *event) override;

private:
	void initClass();
	void initContextMenu();
	void addFile(QString fullFileName);

	void tempWriteXML();
private:
	Ui::JKMainWin m_ui;
	QMenu* tableViewMenu;

	JKTableModel* m_pTableModel;
	JKFilesData* m_pFilesData;
};

#endif // JKMAINWIN_H
