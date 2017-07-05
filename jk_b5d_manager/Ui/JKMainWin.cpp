#include "JKMainWin.h"
#include "JKTableModel.h"
#include <QFileDialog>
#include "BLL/JKFileData.h"
#include "BLL/JKFilesData.h"
#include "JKTableDelegate.h"
#include <Windows.h>
#include "JKUtil/JKStringUtil.h"

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
	pFileData->setFullPath(fileName.toStdString());

	m_pTableModel->addFileData(pFileData);
}

void JKMainWin::onDeleteFile()
{
	QModelIndex curIdx = m_ui.m_pTableView->currentIndex();
	JKFileData* pFileData = m_pFilesData->getB5DFile(curIdx.row());
	if (pFileData == nullptr)
		return;
	m_pTableModel->delFileData(pFileData);
}

void JKMainWin::onSave()
{
	m_pFilesData->saveB5DFile();
}

void JKMainWin::onRunExe()
{
	QModelIndex curIdx = m_ui.m_pTableView->currentIndex();
	JKFileData* pFileData = m_pFilesData->getB5DFile(curIdx.row());
	if (pFileData == nullptr)
		return;
	std::string str = pFileData->getFullPath();
	std::string command = "\"" + str + "\"";

	ShellExecute(NULL, L"open", JK_NAMESPACE::JKStringUtil::to_wstring(command).c_str(), NULL, NULL, SW_SHOWDEFAULT);
}

void JKMainWin::onRunTool()
{
}

void JKMainWin::initClass()
{
	m_pTableModel = new JKTableModel(this);
	m_pFilesData = new JKFilesData();
	m_pTableModel->setFilesData(m_pFilesData);

	m_ui.m_pTableView->setModel(m_pTableModel);
	//m_ui.m_pTableView->setEditTriggers(QAbstractItemView::DoubleClicked);
	m_ui.m_pTableView->horizontalHeader()->setStretchLastSection(true);
	m_ui.m_pTableView->verticalHeader()->setDefaultSectionSize(20);

	JKTableDelegate* pTableDelegate = new JKTableDelegate;
	m_ui.m_pTableView->setItemDelegate(pTableDelegate);

	QItemSelectionModel *selectionModel = new QItemSelectionModel(m_pTableModel);
	m_ui.m_pTableView->setSelectionModel(selectionModel);

	connect(m_ui.m_pActAdd, SIGNAL(triggered()), this, SLOT(onAddFile()));
	connect(m_ui.m_pActDelete, SIGNAL(triggered()), this, SLOT(onDeleteFile()));
	connect(m_ui.m_pActSave, SIGNAL(triggered()), this, SLOT(onSave()));
	connect(m_ui.m_pPBtnRunner, SIGNAL(triggered()), this, SLOT(onRunExe()));
	connect(m_ui.m_pPBtnToolRunner, SIGNAL(triggered()), this, SLOT(onRunTool()));
}
