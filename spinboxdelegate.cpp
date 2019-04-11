#include <QDoubleSpinBox>
#include <QDebug>

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



QWidget* SpinBoxDelegate::createEditor(QWidget* parent, const QStyleOptionViewItem&,
                                       const QModelIndex& index) const
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
    QDoubleSpinBox* doubleSpinBox = qobject_cast<QDoubleSpinBox*>(editor);
    doubleSpinBox->setValue(1.00);
    qDebug() << __func__;
}



void SpinBoxDelegate::setModelData(QWidget* editor, QAbstractItemModel* model,
                                   const QModelIndex& index) const
{
    QDoubleSpinBox* doubleSpinBox = qobject_cast<QDoubleSpinBox*>(editor);
    double value = doubleSpinBox->value();

    QModelIndex chainedIndex = model->index(index.column(), index.row());
    model->setData(index, value);
    model->setData(chainedIndex, QString::number(1.0 / value, 'f', 3));
    qDebug() << __func__;
}



void SpinBoxDelegate::updateEditorGeometry(QWidget* editor, const QStyleOptionViewItem& option,
                                           const QModelIndex&) const
{
    editor->setGeometry(option.rect);
    qDebug() << __func__;
}
