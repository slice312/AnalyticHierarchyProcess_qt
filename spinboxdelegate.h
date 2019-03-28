#ifndef SPINBOX_DELEGATE_H_INCLUDED
#define SPINBOX_DELEGATE_H_INCLUDED

#include <QItemDelegate>
#include <QModelIndex>
#include <QVector>


class SpinBoxDelegate : public QItemDelegate
{
    Q_OBJECT

private:
    QVector<QModelIndex> lockIndexes;

public:
    explicit SpinBoxDelegate(QObject* parent = nullptr);

    void lockIndex(const QModelIndex& index);


protected:
    QWidget* createEditor(QWidget* parent, const QStyleOptionViewItem& option,
                          const QModelIndex &index) const override;

    void setEditorData(QWidget* editor, const QModelIndex& index) const override;

    void setModelData(QWidget* editor, QAbstractItemModel* model,
                      const QModelIndex& index) const override;

    void updateEditorGeometry(QWidget* editor, const QStyleOptionViewItem& option,
                              const QModelIndex& index) const override;
};





#endif // SPINBOX_DELEGATE_H_INCLUDED
