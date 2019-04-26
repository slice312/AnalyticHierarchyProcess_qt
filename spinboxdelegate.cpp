#include <QDoubleSpinBox>
#include <QDebug>
#include "spinboxdelegate.h"



SpinBoxDelegate::SpinBoxDelegate(int rows, int cols, QObject* parent) :
    QItemDelegate(parent), marked(rows, cols), mx(rows, cols)
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

    mx(row, col) = 1.0;
    mx(col, row) = 1.0;
}



QWidget* SpinBoxDelegate::createEditor(QWidget* parent, const QStyleOptionViewItem&,
                                       const QModelIndex& index) const
{
    qDebug() << __func__;
    QDoubleSpinBox* doubleSpinBox = new QDoubleSpinBox(parent);
    doubleSpinBox->setMinimum(-1000.0001);
    doubleSpinBox->setMaximum(+1000.0001);
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

    mx(row, col) = value;
    mx(col, row) = 1.0 / value;


    for (uint i = 0; i < marked.size1(); i++)
    {
        for (uint j = 0; j < marked.size2(); j++)
        {
            if (marked(i, j) == 0.0)
            {
                //матрица не заполнена
                return;
            }
        }
    }


    qDebug() << "SpinBoxDelegate EMIT indicate";

    //Если матрица заполнена.
    //Проверка индекса (CR) согласованности.
    double indexCR = AlghorithmAHP::getCR(mx);
    if (indexCR > 0.1)
        emit indicate(false);
    else
        emit indicate(true);
}



void SpinBoxDelegate::updateEditorGeometry(QWidget* editor, const QStyleOptionViewItem& option,
                                           const QModelIndex&) const
{
    qDebug() << __func__;
    editor->setGeometry(option.rect);
}
