#ifndef JKOTHERENVRIONMENTDLG_H
#define JKOTHERENVRIONMENTDLG_H

#include <QDialog>
#include "ui_JKOtherEnvrionmentDlg.h"

class JKOtherEnvrionmentDlg : public QDialog
{
	Q_OBJECT

public:
	JKOtherEnvrionmentDlg(QWidget *parent = 0);
	~JKOtherEnvrionmentDlg();

	QString GetInputUrl();
	bool	IsPrivate();

private:
	Ui::JKOtherEnvrionmentDlg ui;
};

#endif // JKOTHERENVRIONMENTDLG_H
