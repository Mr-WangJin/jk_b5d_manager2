#include "JKTableDelegate.h"
#include <QLineEdit>

JKTableDelegate::JKTableDelegate(QObject *parent)
    : QStyledItemDelegate(parent)
{
}

QWidget *JKTableDelegate::createEditor(QWidget *parent, const QStyleOptionViewItem &/* option */, const QModelIndex &/* index */) const
{
    QLineEdit *editor = new QLineEdit(parent);
    editor->setFrame(false);

    return editor;
}

void JKTableDelegate::setEditorData(QWidget *editor, const QModelIndex &index) const
{
    QString value = index.model()->data(index, Qt::EditRole).toString();

	QLineEdit *pLineEdit = static_cast<QLineEdit*>(editor);

	pLineEdit->setText(value);
}

void JKTableDelegate::setModelData(QWidget *editor, QAbstractItemModel *model,
                                   const QModelIndex &index) const
{
	QLineEdit *pLineEdit = static_cast<QLineEdit*>(editor);

    model->setData(index, pLineEdit->text(), Qt::EditRole);
}

void JKTableDelegate::updateEditorGeometry(QWidget *editor,
    const QStyleOptionViewItem &option, const QModelIndex &/* index */) const
{
    editor->setGeometry(option.rect);
}
