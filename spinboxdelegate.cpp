#include <QDoubleSpinBox>
#include <QTableView>
#include <QDebug>

#include "spinboxdelegate.h"


SpinBoxDelegate::SpinBoxDelegate(QObject* parent) :
    QItemDelegate(parent)
{}


QWidget* SpinBoxDelegate::createEditor(QWidget* parent,
    const QStyleOptionViewItem&, const QModelIndex&) const
{
    QDoubleSpinBox* doubleSpinBox = new QDoubleSpinBox(parent);
    doubleSpinBox->setMinimum(-1000.0001);
    doubleSpinBox->setMaximum(+1000.0001);

    qDebug() << parent;

    QTableView* table = dynamic_cast<QTableView*>(parent);

    qDebug() << __func__;
    qDebug() << " AFTER CAST " <<  (table == nullptr);

    return doubleSpinBox;
}


void SpinBoxDelegate::setEditorData(QWidget* editor, const QModelIndex& index) const
{
    QDoubleSpinBox* doubleSpinBox = dynamic_cast<QDoubleSpinBox*>(editor);
    doubleSpinBox->setValue(index.data().toDouble());
    qDebug() << __func__;
}


void SpinBoxDelegate::setModelData(QWidget* editor,
    QAbstractItemModel* model, const QModelIndex& index) const
{
    QDoubleSpinBox* doubleSpinBox = dynamic_cast<QDoubleSpinBox*>(editor);
    model->setData(index, doubleSpinBox->value());
    qDebug() << __func__;
}


void SpinBoxDelegate::updateEditorGeometry(QWidget* editor,
    const QStyleOptionViewItem& option, const QModelIndex&) const
{
    editor->setGeometry(option.rect);
    qDebug() << __func__;
}
