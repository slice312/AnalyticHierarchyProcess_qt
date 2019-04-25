#include <QDoubleSpinBox>
#include <QDebug>
#include "spinboxdelegate.h"




SpinBoxDelegate::SpinBoxDelegate(int rows, int cols, QObject* parent) :
    QItemDelegate(parent), marked(rows, cols)
{
    qDebug() << "DELEGATE CREATED";
}


void SpinBoxDelegate::lockIndex(const QModelIndex& index)
{
    lockIndexes.push_back(index);
    int row = index.row();
    int col = index.column();
    marked(row, col) = 1.0;
    marked(col, row) = 1.0;
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
    qDebug() << __func__;
    QDoubleSpinBox* doubleSpinBox = qobject_cast<QDoubleSpinBox*>(editor);
    doubleSpinBox->setValue(1.00);
}




void SpinBoxDelegate::setModelData(QWidget* editor, QAbstractItemModel* model,
                                   const QModelIndex& index) const
{
    qDebug() << __func__;
    QDoubleSpinBox* doubleSpinBox = qobject_cast<QDoubleSpinBox*>(editor);
    double value = doubleSpinBox->value();
    int col = index.column();
    int row = index.row();

    QModelIndex chainedIndex = model->index(col, row);
    model->setData(index, value);
    model->setData(chainedIndex, QString::number(1.0 / value, 'f', 3));

    marked(col, row) = 1.0;
    marked(row, col) = 1.0;

    bool flag = true;
    for (int i = 0; i < marked.size1(); i++)
    {
        for (int j = 0; j < marked.size2(); j++)
        {
            if (marked(i, j) == 0.0)
            {
                flag = false;
                break;
            }
        }
    }

    if (flag == true)
    {
        emit indicate(true);
        qDebug() << "SpinBoxDelegate EMIT indicate";
    }
}



void SpinBoxDelegate::updateEditorGeometry(QWidget* editor, const QStyleOptionViewItem& option,
                                           const QModelIndex&) const
{
    editor->setGeometry(option.rect);
    qDebug() << __func__;
}
