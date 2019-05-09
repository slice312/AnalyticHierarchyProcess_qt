#include <QDoubleSpinBox>
#include <QDebug>

#include "spinboxdelegate.h"



SpinBoxDelegate::SpinBoxDelegate(int rowcol, QObject* parent) :
    QItemDelegate(parent),
    marked(rowcol, rowcol), mx(rowcol, rowcol)
{}



void SpinBoxDelegate::lockIndex(const QModelIndex& index)
{
    lockIndexes.push_back(index);
    int row = index.row();
    int col = index.column();

    mx(row, col) = 1.0;
    mx(col, row) = 1.0;

    marked(row, col) = 1.0;
    marked(col, row) = 1.0;

    if (mx.rows() == 1 && mx.cols() == 1)
        emit indicate(0.0);
}



QWidget* SpinBoxDelegate::createEditor(QWidget* parent, const QStyleOptionViewItem&,
                                       const QModelIndex& index) const
{
    qDebug() << __func__;
    QDoubleSpinBox* doubleSpinBox = new QDoubleSpinBox(parent);
    doubleSpinBox->setMinimum(0.0);
    doubleSpinBox->setMaximum(99.0);
    doubleSpinBox->setSingleStep(0.5);
    doubleSpinBox->setDecimals(3);

    for (QModelIndex ix : lockIndexes)
    {
        if (ix == index)
        {
            doubleSpinBox->setReadOnly(true);
            break;
        }
    }
    return doubleSpinBox;
}



void SpinBoxDelegate::setEditorData(QWidget* editor, const QModelIndex& index) const
{
    qDebug() << __func__;
    QDoubleSpinBox* doubleSpinBox = qobject_cast<QDoubleSpinBox*>(editor);
    int row = index.row();
    int col = index.column();

    if (marked(row, col) > 0.0)
        doubleSpinBox->setValue(mx(row, col));
    else
        doubleSpinBox->setValue(1.0);
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

    mx(row, col) = value;
    mx(col, row) = 1.0 / value;

    marked(col, row) = 1.0;
    marked(row, col) = 1.0;


    for (uint i = 0; i < marked.rows(); i++)
    {
        for (uint j = 0; j < marked.cols(); j++)
        {
            if (marked(i, j) == 0.0)
            {
                //матрица не заполнена
                return;
            }
        }
    }

    //Если матрица заполнена.
    qDebug() << "SpinBoxDelegate EMIT indicate" << endl;
    emit indicate(AlghorithmAHP::getCR(mx));
}



void SpinBoxDelegate::updateEditorGeometry(QWidget* editor, const QStyleOptionViewItem& option,
                                           const QModelIndex&) const
{
    qDebug() << __func__;
    editor->setGeometry(option.rect);
}
