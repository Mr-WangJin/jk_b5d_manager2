#ifndef JK_B5D_MANAGER_H
#define JK_B5D_MANAGER_H

#include <QtWidgets/QMainWindow>
#include "ui_jk_b5d_manager.h"

class jk_b5d_manager : public QMainWindow
{
	Q_OBJECT

public:
	jk_b5d_manager(QWidget *parent = 0);
	~jk_b5d_manager();

private:
	Ui::jk_b5d_managerClass ui;
};

#endif // JK_B5D_MANAGER_H
