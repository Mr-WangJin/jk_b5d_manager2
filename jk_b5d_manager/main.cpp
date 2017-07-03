#include "jk_b5d_manager.h"
#include <QtWidgets/QApplication>

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);
	jk_b5d_manager w;
	w.show();
	return a.exec();
}
