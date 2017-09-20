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

public slots:
	void onAddFile();
	void onDeleteFile();
	void onSave();
	void onOpenDir();
	void onDelUnis();
	void onDelUnableFiles();

	void onSetOffice();
	void onSetHuNan();

	void onRunExe();
	void onRunTool();
	void onRunCraft();

protected:
	void keyPressEvent(QKeyEvent *event) override;

private:
	void initClass();

	void tempWriteXML();
private:
	Ui::JKMainWin m_ui;

	JKTableModel* m_pTableModel;
	JKFilesData* m_pFilesData;
};

#endif // JKMAINWIN_H
