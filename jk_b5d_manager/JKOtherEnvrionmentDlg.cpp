#include "JKOtherEnvrionmentDlg.h"

JKOtherEnvrionmentDlg::JKOtherEnvrionmentDlg(QWidget *parent)
	: QDialog(parent)
{
	ui.setupUi(this);
	/*connect(ui.okButton, &QPushButton::clicked, this, &QDialog::accepted);
	connect(ui.buttonBox)*/
	ui.buttonBox->addButton(QStringLiteral("确定"), QDialogButtonBox::AcceptRole);
	ui.buttonBox->addButton(QStringLiteral("取消"), QDialogButtonBox::RejectRole);
	connect(ui.buttonBox, &QDialogButtonBox::accepted, this, &JKOtherEnvrionmentDlg::accept);
	connect(ui.buttonBox, &QDialogButtonBox::rejected, this, &JKOtherEnvrionmentDlg::reject);
}

JKOtherEnvrionmentDlg::~JKOtherEnvrionmentDlg()
{

}

QString JKOtherEnvrionmentDlg::GetInputUrl()
{
	return ui.environmentText->toPlainText();
}

bool JKOtherEnvrionmentDlg::IsPrivate()
{
	return ui.isPrivateCheck->checkState();
}
