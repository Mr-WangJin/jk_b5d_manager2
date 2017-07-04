#include <QtWidgets/QApplication>
#include "Ui/JKMainWin.h"

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);
	JKMainWin w;
	w.show();
	return a.exec();
}

