#include "JKMainWin.h"
#include "JKTableModel.h"
#include <QFileDialog>
#include "BLL/JKFileData.h"
#include "BLL/JKFilesData.h"
#include "JKTableDelegate.h"
#include <Windows.h>
#include "JKUtil/JKStringUtil.h"
#include <QFileInfo>
#include <QKeyEvent>
#include <QMessageBox>
#include <QXmlStreamReader>
#include <QDomDocument>
#include "JKOtherEnvrionmentDlg.h"

USING_JK_NAMESPACE;

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
	this->addFile(fileName);
	//this->onSave();
}
void JKMainWin::onBatchAdded()
{
	QString path = QFileDialog::getExistingDirectory(this, QStringLiteral("批量添加"), "/");
	QDir dir(path);
	if (!dir.exists())
	{
		return;
	}

	QList<QString> listPath;
	listPath.push_back(path);

	while (listPath.count() > 0)
	{
		QString tempPath = listPath[0];
		listPath.pop_front();
		QDir dir(tempPath);
		dir.setFilter(QDir::Dirs | QDir::Files | QDir::NoSymLinks);
		dir.setSorting(QDir::DirsFirst);
		QFileInfoList list = dir.entryInfoList();

		for (int i = 0 ;i<list.count(); ++i)
		{
			QFileInfo file_info = list.at(i);
			if (file_info.fileName() == "." | file_info.fileName() == "..")
				continue;
			if (file_info.isDir())
				listPath.push_back(file_info.filePath());
			else
			{
				QString baseName = file_info.fileName();
				if (baseName == "BIM5D.exe")
					this->addFile(file_info.filePath());
			}
		}
	}

}

void JKMainWin::onDeleteFile()
{
	const QModelIndexList& selList = m_ui.m_pTableView->selectionModel()->selectedIndexes();
	for each (const QModelIndex& var in selList)
	{
		JKFileData* pFileData = m_pFilesData->getB5DFile(var.row());
		if (pFileData == nullptr)
			return;
		m_pTableModel->delFileData(pFileData);
	}
}

void JKMainWin::onUninstall()
{
	QModelIndex curIdx = m_ui.m_pTableView->currentIndex();
	JKFileData* pFileData = m_pFilesData->getB5DFile(curIdx.row());
	if (pFileData == nullptr)
		return;

	try
	{
		std::string str = pFileData->getFullPath();
		QFile f(QString::fromStdString(str));
		if (!f.exists())
		{
			m_pTableModel->delFileData(pFileData);
			return;
		}
		QDir dir(QString::fromStdString(str));
		dir.cdUp(); dir.cdUp();

		QFile f1(dir.path() + "\\_unins000.exe");
		if (f1.exists())
		{
			f1.rename(dir.path() + "unins000.exe");
			QFile f(dir.path() + "\\_unins000.dat");
			if (f.exists())
			{
				f.rename(dir.path() + "unins000.dat");

				std::string command = "\"" + f.fileName().toStdString() + "\"";

				ShellExecute(NULL, L"open", JKStringUtil::UTF8ToUnicode(command).c_str(), NULL, NULL, SW_SHOWDEFAULT);
			}
		}
		else
		{
			if (!this->deleteDir(dir.path()))
				return;
		}

		m_pTableModel->delFileData(pFileData);

	}
	catch(std::exception & e)
	{
		QMessageBox::information(this, QStringLiteral("提示！"), QStringLiteral("部分文件没有权限删除！请手动删除！"));
	}
	
}

void JKMainWin::onSave()
{
	m_pFilesData->saveB5DFile();
}

void JKMainWin::onOpenDir()
{
	QModelIndex curIdx = m_ui.m_pTableView->currentIndex();
	JKFileData* pFileData = m_pFilesData->getB5DFile(curIdx.row());
	if (pFileData == nullptr)
		return;
	std::string str = pFileData->getFullPath();
	QFileInfo fileInfo(QString(str.c_str()));


	std::string command = fileInfo.absolutePath().toStdString();

	ShellExecute(nullptr, L"open", nullptr, nullptr, JKStringUtil::UTF8ToUnicode(command).c_str(), SW_SHOWNORMAL);
}

void JKMainWin::onDelUnis()
{
	int fileCount = m_pFilesData->fileCount();
	for (int i = 0; i< fileCount; ++i)
	{
		JKFileData* pFileData = m_pFilesData->getB5DFile(i);
		if (pFileData == nullptr)
			return;
		std::string str = pFileData->getFullPath();
		QDir dir(QString::fromStdString(str));
		dir.cdUp(); dir.cdUp();

		QFile f(dir.path()+"\\unins000.dat");
		if (f.exists())
		{
			if (!f.rename(dir.path()+"_unins000.dat"))
			{

			}
		}
		QFile f1(dir.path() + "\\unins000.exe");
		if (f1.exists())
		{
			if (f1.rename(dir.path()+ "_unins000.exe"))
			{

			}
		}
	}
}

void JKMainWin::onDelUnableFiles()
{
	std::vector<JKFileData*> vecDelPtr;
	int fileCount = m_pFilesData->fileCount();
	for (int i = 0; i < fileCount; ++i)
	{
		JKFileData* pFileData = m_pFilesData->getB5DFile(i);
		if (pFileData == nullptr)
			return;
		std::string str = pFileData->getFullPath();
		QFile f(QString::fromStdString(str));
		if (!f.exists())
			vecDelPtr.push_back(pFileData);
	}
	
	for (int i = 0; i< vecDelPtr.size(); ++i)
	{
		m_pFilesData->deleteFile(vecDelPtr[i]);
	}
}

void JKMainWin::onSetOffice()
{
	QModelIndex curIdx = m_ui.m_pTableView->currentIndex();
	JKFileData* pFileData = m_pFilesData->getB5DFile(curIdx.row());
	if (pFileData == nullptr)
		return;

	std::string str = pFileData->getFullPath();
	QDir dir(QString::fromStdString(str));
	dir.cdUp(); 

	try {
		pFileData->updateXMLNode(dir.path() + "\\BIM5D.xml", "update", "CloudType", QString("Public"));
		pFileData->updateXMLNode(dir.path() + "\\BIM5D.xml", "update", "CloudService_v1", QString("https://bim5d.glodon.com/api/v1/"));
		pFileData->updateXMLNode(dir.path() + "\\BIM5D.xml", "update", "CloudService_v3", QString("https://bim5d.glodon.com/api/v3/"));
		/*pFileData->updateXMLNode(dir.path() + "\\config.xml", "update", "CloudService_v1", QString("https://bim5d.glodon.com/api/v1/"));
		pFileData->updateXMLNode(dir.path() + "\\config.xml", "update", "CloudService_v3", QString("https://bim5d.glodon.com/api/v3/"));*/
		QMessageBox::information(this, QStringLiteral("提示！"), QStringLiteral("设置成功！"));
	}
	catch (std::exception &e)
	{
		QMessageBox::information(this, QStringLiteral("提示！"), QString("%1").arg(e.what()));
	}

}
void JKMainWin::onSetHuNan()
{
	QModelIndex curIdx = m_ui.m_pTableView->currentIndex();
	JKFileData* pFileData = m_pFilesData->getB5DFile(curIdx.row());
	if (pFileData == nullptr)
		return;

	std::string str = pFileData->getFullPath();
	QDir dir(QString::fromStdString(str));
	dir.cdUp();

	try {
		pFileData->updateXMLNode(dir.path() + "\\BIM5D.xml", "update", "CloudType", QString("Public"));
		pFileData->updateXMLNode(dir.path() + "\\BIM5D.xml", "update", "CloudService_v1", QString("http://bim5d-hunan.glodon.com/api/v1/"));
		pFileData->updateXMLNode(dir.path() + "\\BIM5D.xml", "update", "CloudService_v3", QString("http://bim5d-hunan.glodon.com/api/v3/"));
		/*pFileData->updateXMLNode(dir.path() + "\\config.xml", "update", "CloudService_v1", QString("http://bim5d-hunan.glodon.com/api/v1/"));
		pFileData->updateXMLNode(dir.path() + "\\config.xml", "update", "CloudService_v3", QString("http://bim5d-hunan.glodon.com/api/v3/"));*/
		QMessageBox::information(this, QStringLiteral("提示！"), QStringLiteral("设置成功！"));
	}
	catch (std::exception &e)
	{
		QMessageBox::information(this, QStringLiteral("提示！"), QString("%1").arg(e.what()));
	}
}

void JKMainWin::onSetOther()
{
	QModelIndex curIdx = m_ui.m_pTableView->currentIndex();
	JKFileData* pFileData = m_pFilesData->getB5DFile(curIdx.row());
	if (pFileData == nullptr)
		return;

	std::string str = pFileData->getFullPath();
	QDir dir(QString::fromStdString(str));
	dir.cdUp();

	JKOtherEnvrionmentDlg* otherEnvironMentDlg = new JKOtherEnvrionmentDlg(this);
	if (otherEnvironMentDlg->exec())
	{
		try
		{
			QString url = otherEnvironMentDlg->GetInputUrl();
			if (otherEnvironMentDlg->IsPrivate())
			{
				 pFileData->updateXMLNode(dir.path() + "\\BIM5D.xml", "update", "CloudType", QString("Private"));
				 pFileData->updateXMLNode(dir.path() + "\\BIM5D.xml", "update", "PrivateCloudDocUrl", url + QString("/cloud"));
			}
			else
			{
				pFileData->updateXMLNode(dir.path() + "\\BIM5D.xml", "update", "CloudService_v1", url + QString("/api/v1/"));
				pFileData->updateXMLNode(dir.path() + "\\BIM5D.xml", "update", "CloudService_v3", url + QString("/api/v3/"));
			}
			QMessageBox::information(this, QStringLiteral("提示！"), QStringLiteral("设置成功！"));
		}
		catch (std::exception& e)
		{
			QMessageBox::information(this, QStringLiteral("提示！"), QString("%1").arg(e.what()));
		}
	}
}

void JKMainWin::onRunExe()
{
	QModelIndex curIdx = m_ui.m_pTableView->currentIndex();
	JKFileData* pFileData = m_pFilesData->getB5DFile(curIdx.row());
	if (pFileData == nullptr)
		return;
	std::string str = pFileData->getFullPath();

	std::string command = "\"" + str + "\"";

	ShellExecute(NULL, L"open", JKStringUtil::UTF8ToUnicode(command).c_str(), NULL, NULL, SW_SHOWDEFAULT);
}

void JKMainWin::onRunTool()
{
	QModelIndex curIdx = m_ui.m_pTableView->currentIndex();
	JKFileData* pFileData = m_pFilesData->getB5DFile(curIdx.row());
	if (pFileData == nullptr)
		return;
	std::string str = pFileData->getFullPath();
	QFileInfo fileInfo(QString(str.c_str()));
	QString toolName = QString("%1/%2").arg(fileInfo.path()).arg("AdminManageTool.exe");
	QFileInfo toolFileInfo(toolName);
	if (toolFileInfo.exists() == false)
		return;

	std::string command = "\"" + toolName.toStdString() + "\"";

	ShellExecute(NULL, L"open", JKStringUtil::UTF8ToUnicode(command).c_str(), NULL, NULL, SW_SHOWDEFAULT);
}

void JKMainWin::onRunCraft()
{
	QModelIndex curIdx = m_ui.m_pTableView->currentIndex();
	JKFileData* pFileData = m_pFilesData->getB5DFile(curIdx.row());
	if (pFileData == nullptr)
		return;
	std::string str = pFileData->getFullPath();
	QFileInfo fileInfo(QString(str.c_str()));
	QString toolName = QString("%1/%2").arg(fileInfo.path()).arg("CraftLibrary.exe");
	QFileInfo toolFileInfo(toolName);
	if (toolFileInfo.exists() == false)
		return;

	std::string command = "\"" + toolName.toStdString() + "\"";

	ShellExecute(NULL, L"open", JKStringUtil::UTF8ToUnicode(command).c_str(), NULL, NULL, SW_SHOWDEFAULT);
}

void JKMainWin::onTableViewContextMenuClicked(QPoint p)
{
	tableViewMenu->exec(QCursor::pos());
}

void JKMainWin::keyPressEvent(QKeyEvent * event)
{
	if (event->key() == Qt::Key_S && event->modifiers() & Qt::ControlModifier)
	{
		m_pFilesData->saveB5DFile();
	}

	QMainWindow::keyPressEvent(event);
}

void JKMainWin::initClass()
{
	this->initContextMenu();

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
	connect(m_ui.actBatchAdded, SIGNAL(triggered()), this, SLOT(onBatchAdded()));
	connect(m_ui.m_pActDelete, SIGNAL(triggered()), this, SLOT(onDeleteFile()));
	connect(m_ui.actUninstall, SIGNAL(triggered()), this, SLOT(onUninstall()));
	connect(m_ui.m_pActSave, SIGNAL(triggered()), this, SLOT(onSave()));
	connect(m_ui.m_pActOpenFiePath, SIGNAL(triggered()), this, SLOT(onOpenDir()));
	connect(m_ui.m_pPBtnRunner, SIGNAL(clicked()), this, SLOT(onRunExe()));
	connect(m_ui.m_pPBtnToolRunner, SIGNAL(clicked()), this, SLOT(onRunTool()));
	connect(m_ui.m_pPBtnRunCraft, SIGNAL(clicked()), this, SLOT(onRunCraft()));
	connect(m_ui.actDelUnis, SIGNAL(triggered()), this, SLOT(onDelUnis()));
	connect(m_ui.actDelUnableFile, SIGNAL(triggered()), this, SLOT(onDelUnableFiles()));

	connect(m_ui.actSetOffice, SIGNAL(triggered()), this, SLOT(onSetOffice()));
	connect(m_ui.actSetHuNan, SIGNAL(triggered()), this, SLOT(onSetHuNan()));
	connect(m_ui.actSetOther, &QAction::triggered, this, &JKMainWin::onSetOther);
}

void JKMainWin::initContextMenu()
{
	m_ui.m_pTableView->setContextMenuPolicy(Qt::CustomContextMenu);
	connect(m_ui.m_pTableView, SIGNAL(customContextMenuRequested(QPoint)), this, SLOT(onTableViewContextMenuClicked(QPoint)));

	tableViewMenu = new QMenu(m_ui.m_pTableView);

	tableViewMenu->addAction(m_ui.m_pActDelete);
	tableViewMenu->addAction(m_ui.actUninstall);
	tableViewMenu->addAction(m_ui.m_pActOpenFiePath);
	tableViewMenu->addAction(m_ui.actDelUnableFile);
	tableViewMenu->addAction(m_ui.actSetOffice);
	tableViewMenu->addAction(m_ui.actSetHuNan);
	tableViewMenu->addAction(m_ui.actSetOther);
}

void JKMainWin::addFile(QString fullFileName)
{
	QFile f(fullFileName);
	if (!f.exists())
		return;

	QFileInfo fileInfo(fullFileName);
	JKFileData* pFileData = new JKFileData;
	pFileData->setFileName(fileInfo.fileName().toStdString());
	pFileData->setFullPath(fullFileName.toStdString());
	
	try {
		QDir dir(fullFileName);
		dir.cdUp();

		QString fileName = dir.path() + "\\BIM5D.xml";
		QFile file(fileName);
		if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
		{
			file.close();
			JKString error = "can not find" + fileName.toStdString();
			throw std::exception(error.c_str());
		}

		QDomDocument doc;
		if (!doc.setContent(&file))
		{
			file.close();
			throw std::exception("can not open file.");
		}
		file.close();
		QDomNodeList lists = doc.elementsByTagName("Version");
		if (lists.count() == 0)
		{
			JKString error = "can not find Version in xml ";
			throw std::exception(error.c_str());
		}
		QDomElement ele = lists.at(0).toElement();

		QString version = ele.toElement().firstChild().nodeValue();
		pFileData->setVersion(version.toStdString());
	}
	catch (std::exception& e)
	{
		QMessageBox::information(this, QStringLiteral("提示！"), e.what());
	}


	m_pTableModel->addFileData(pFileData);

	this->onDelUnis();
}

void JKMainWin::tempWriteXML()
{
// 	QFile file(filename);
// 	file.open(QIODevice::ReadWrite);
// 	QDomDocument doc;
// 	QDomProcessingInstruction instruction;
// 	instruction = doc.createProcessingInstruction("xml", "version=\"1.0\" encoding=\"GB2312\"");
// 	doc.appendChild(instruction);
// 	QDomElement root = doc.createElement("ipconfig");
// 
// 	doc.appendChild(root);
// 	QDomText text = doc.createTextNode("");
// 	root.appendChild(text);
// 	QTextStream out(&file);
// 	doc.save(out, 4);
// 	file.close();
}

bool JKMainWin::deleteDir(const QString &dirName)
{
	QDir directory(dirName);
	if (!directory.exists())
	{
		return true;
	}


	QString srcPath = QDir::toNativeSeparators(dirName);
	if (!srcPath.endsWith(QDir::separator()))
		srcPath += QDir::separator();


	QStringList fileNames = directory.entryList(QDir::AllEntries | QDir::NoDotAndDotDot | QDir::Hidden);
	bool error = false;
	for (QStringList::size_type i = 0; i != fileNames.size(); ++i)
	{
		QString filePath = srcPath + fileNames.at(i);
		QFileInfo fileInfo(filePath);
		if (fileInfo.isFile() || fileInfo.isSymLink())
		{
			QFile::setPermissions(filePath, QFile::WriteOwner);
			if (!QFile::remove(filePath))
			{
				JKString info = "remove file" + filePath.toStdString() + "faild!";
				throw std::exception(info.c_str());
			}
		}
		else if (fileInfo.isDir())
		{
			if (!deleteDir(filePath))
			{
				error = true;
			}
		}
	}


	if (!directory.rmdir(QDir::toNativeSeparators(directory.path())))
	{
		JKString info = "remove dir" + directory.path().toStdString() + "faild!";
		throw std::exception(info.c_str());
		//qDebug() << "remove dir" << directory.path() << " faild!";
	}
	return !error;
}