#include <QDoubleSpinBox>
#include <QTableView>
#include <QDebug>
#include <QStandardItemModel>

#include "spinboxdelegate.h"


SpinBoxDelegate::SpinBoxDelegate(QObject* parent) :
    QItemDelegate(parent)
{
    qDebug() << "DELEGATE CREATED";
}



void SpinBoxDelegate::lockIndex(const QModelIndex& index)
{
    lockIndexes.push_back(index);
}



QWidget* SpinBoxDelegate::createEditor(QWidget* parent,
    const QStyleOptionViewItem&, const QModelIndex& index) const
{
    QDoubleSpinBox* doubleSpinBox = new QDoubleSpinBox(parent);
    doubleSpinBox->setMinimum(-1000.0001);
    doubleSpinBox->setMaximum(+1000.0001);
    doubleSpinBox->setSingleStep(0.25);
    doubleSpinBox->setDecimals(3);

    for (QModelIndex ix : lockIndexes)
    {
        if (ix == index)
        {
            doubleSpinBox->setReadOnly(true);
            break;
        }
    }

    qDebug() << __func__;
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
