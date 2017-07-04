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

private:
	void initClass();

private:
	Ui::JKMainWin m_ui;

	JKTableModel* m_pTableModel;
	JKFilesData* m_pFilesData;
};

#endif // JKMAINWIN_H
