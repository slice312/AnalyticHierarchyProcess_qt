#ifndef SPINBOX_DELEGATE_H_INCLUDED
#define SPINBOX_DELEGATE_H_INCLUDED

#include <QItemDelegate>
#include <QModelIndex>
#include <QVector>
#include "alg_ahp.h"


class SpinBoxDelegate : public QItemDelegate
{
    Q_OBJECT

private:
    QVector<QModelIndex> lockIndexes;
    mutable Matrix marked;

public:
    SpinBoxDelegate(int rows, int cols, QObject* parent = nullptr);

    void lockIndex(const QModelIndex& index);


protected:
    QWidget* createEditor(QWidget* parent, const QStyleOptionViewItem& option,
                          const QModelIndex &index) const override;

    void setEditorData(QWidget* editor, const QModelIndex& index) const override;

    void setModelData(QWidget* editor, QAbstractItemModel* model,
                      const QModelIndex& index) const override;

    void updateEditorGeometry(QWidget* editor, const QStyleOptionViewItem& option,
                              const QModelIndex& index) const override;

signals:
    void indicate(bool state) const;
};





#endif // SPINBOX_DELEGATE_H_INCLUDED
