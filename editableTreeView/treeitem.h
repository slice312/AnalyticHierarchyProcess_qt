#ifndef TREE_ITEM_H_INCLUDED
#define TREE_ITEM_H_INCLUDED

#include <QList>
#include <QVector>
#include <QVariant>



class TreeItem
{
private:
    TreeItem* mParentItem;
    QList<TreeItem*> mChildItems;
    QVector<QVariant> mItemData;


public:
    explicit TreeItem(const QVector<QVariant>& data, TreeItem* parent = nullptr);
     ~TreeItem();

    void appendChild(TreeItem* child);

    TreeItem* child(int row) const;
    TreeItem* parent() const;

    int childCount() const;
    int columnCount() const;
    int childNumber() const;

    QVariant data(int column) const;
    bool setData(int column, const QVariant& value);

    bool insertChildren(int position, int count, int columns);
    bool removeChildren(int position, int count);

    bool insertColumns(int position, int columns);
    bool removeColumns(int position, int columns);
};

#endif // TREE_ITEM_H_INCLUDED
