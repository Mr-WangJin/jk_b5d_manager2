#include "JKMainWin.h"
#include "JKTableModel.h"
#include <QFileDialog>
#include "BLL/JKFileData.h"
#include "BLL/JKFilesData.h"

JKMainWin::JKMainWin(QWidget *parent)
	: QMainWindow(parent)
{
	m_ui.setupUi(this);

	this->initClass();
}

JKMainWin::~JKMainWin()
{

}

void JKMainWin::onAddFile()
{
	QString fileName = QFileDialog::getOpenFileName(this, tr("Open File"), "/",	tr("Exe (*.exe)"));
	if (fileName.size() == 0)
		return;
	QFileInfo fileInfo(fileName);
	JKFileData* pFileData = new JKFileData;
	pFileData->setFileName(fileInfo.fileName().toStdString());
	m_pFilesData->addFile(pFileData);
}
void JKMainWin::onDeleteFile()
{

}
void JKMainWin::onSave()
{

}

void JKMainWin::initClass()
{
	m_pTableModel = new JKTableModel(this);
	m_pFilesData = new JKFilesData();
	m_pTableModel->setFilesData(m_pFilesData);

	m_ui.m_pTableView->setModel(m_pTableModel);

	connect(m_ui.m_pActAdd, SIGNAL(triggered()), this, SLOT(onAddFile()));
}
