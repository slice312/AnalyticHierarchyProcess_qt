#include "treeitem.h"



TreeItem::TreeItem(const QVector<QVariant>& data, TreeItem* parent)
{
    mParentItem = parent;
    mItemData = data;
}



TreeItem::~TreeItem()
{
    qDeleteAll(mChildItems);
}



void TreeItem::appendChild(TreeItem* child)
{
    mChildItems.append(child);
}



TreeItem* TreeItem::child(int row) const
{
    //тут не оператор[],потому что value возвращает значение по умолчанию
    //для типа, если элемента нет.
    return mChildItems.value(row);
}



TreeItem* TreeItem::parent() const
{
    return mParentItem;
}



int TreeItem::childCount() const
{
    return mChildItems.size();
}



int TreeItem::columnCount() const
{
    return mItemData.size();
}



int TreeItem::childNumber() const
{
    if (mParentItem != nullptr)
        return mParentItem->mChildItems.indexOf(const_cast<TreeItem*>(this));
    return 0;
}



QVariant TreeItem::data(int column) const
{
    return mItemData.value(column);
}



bool TreeItem::setData(int column, const QVariant& value)
{
    if (column < 0 || column >= mItemData.size())
        return false;
    mItemData[column] = value;
    return true;
}



bool TreeItem::insertChildren(int position, int count, int columns)
{
    if (position < 0 || position > mChildItems.size())
        return false;

    for (int row = 0; row < count; row++)
    {
        QVector<QVariant> data(columns);
        TreeItem* item = new TreeItem(data, this);
        mChildItems.insert(position, item);
    }
    return true;
}



bool TreeItem::removeChildren(int position, int count)
{
    if (position < 0 || position + count > mChildItems.size())
        return false;

    for (int row = 0; row < count; row++)
        delete mChildItems.takeAt(position);

    return true;
}



bool TreeItem::insertColumns(int position, int columns)
{
    if (position < 0 || position > mItemData.size())
        return false;

    for (int column = 0; column < columns; column++)
        mItemData.insert(position, QVariant());

    for (TreeItem* child : mChildItems)
        child->insertColumns(position, columns);
    return true;
}



bool TreeItem::removeColumns(int position, int columns)
{
    if (position < 0 || position + columns > mItemData.size())
        return false;

    for (int column = 0; column < columns; column++)
        mItemData.remove(position);

    for (TreeItem* child : mChildItems)
        child->removeColumns(position, columns);

    return true;
}
