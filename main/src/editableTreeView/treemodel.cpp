#include <QDebug>
#include "treemodel.h"
#include "treeitem.h"



TreeModel::TreeModel(const QStringList& headers, QObject* parent) :
    QAbstractItemModel(parent)
{
    QVector<QVariant> rootData;
    for (QString header : headers)
        rootData << header;
    mRootItem = new TreeItem(rootData);
}



TreeModel::TreeModel(const QStringList& headers, const QString& data, QObject* parent) :
    QAbstractItemModel(parent)
{
    QVector<QVariant> rootData;
    for (QString header : headers)
        rootData << header;
    mRootItem = new TreeItem(rootData);
    setupModelData(data.split(QString("\n")), mRootItem);
}



TreeModel::~TreeModel()
{
    //    qDebug() << "deleted TreeModel";
    delete mRootItem;
}



QVariant TreeModel::data(const QModelIndex& index, int role) const
{
    if (!index.isValid())
        return QVariant();

    if (role != Qt::DisplayRole && role != Qt::EditRole)
        return QVariant();

    TreeItem* item = getItem(index);
    return item->data(index.column());
}



QVariant TreeModel::headerData(int section, Qt::Orientation orientation,
                               int role) const
{
    if (orientation == Qt::Horizontal && role == Qt::DisplayRole)
        return mRootItem->data(section);
    return QVariant();
}



bool TreeModel::setData(const QModelIndex& index, const QVariant& value, int role)
{
    if (role != Qt::EditRole)
        return false;

    TreeItem* item = getItem(index);
    bool result = item->setData(index.column(), value);
    if (result)
        emit dataChanged(index, index, QVector<int>(1, role));

    return result;
}



bool TreeModel::setHeaderData(int section, Qt::Orientation orientation,
                              const QVariant& value, int role)
{
    if (role != Qt::EditRole || orientation != Qt::Horizontal)
        return false;

    bool result = mRootItem->setData(section, value);
    if (result)
        emit headerDataChanged(orientation, section, section);

    return result;
}



QModelIndex TreeModel::index(int row, int column, const QModelIndex& parent) const
{
    if (parent.isValid() && parent.column() != 0)
        return QModelIndex();

    TreeItem* parentItem = getItem(parent);
    TreeItem* childItem = parentItem->child(row);

    if (childItem != nullptr)
        return createIndex(row, column, childItem);
    else
        return QModelIndex();
}



QModelIndex TreeModel::parent(const QModelIndex& index) const
{
    if (!index.isValid())
        return QModelIndex();

    TreeItem* childItem = getItem(index);
    TreeItem* parentItem = childItem->parent();

    if (parentItem == mRootItem)
        return QModelIndex();

    return createIndex(parentItem->childNumber(), 0, parentItem);
}



Qt::ItemFlags TreeModel::flags(const QModelIndex& index) const
{
    if (!index.isValid())
        return 0;
    return Qt::ItemIsEditable | QAbstractItemModel::flags(index);
}



int TreeModel::rowCount(const QModelIndex& parent) const
{
    TreeItem* parentItem = getItem(parent);
    return parentItem->childCount();
}



int TreeModel::columnCount(const QModelIndex& parent) const
{
    return mRootItem->columnCount();
}



bool TreeModel::insertRows(int position, int rows, const QModelIndex& parent)
{
    TreeItem* parentItem = getItem(parent);
    bool success;

    beginInsertRows(parent, position, position + rows - 1);
    success = parentItem->insertChildren(position, rows, mRootItem->columnCount());
    endInsertRows();

    return success;
}



bool TreeModel::removeRows(int position, int rows, const QModelIndex& parent)
{
    TreeItem* parentItem = getItem(parent);
    bool success = true;

    beginRemoveRows(parent, position, position + rows - 1);
    success = parentItem->removeChildren(position, rows);
    endRemoveRows();

    return success;
}



bool TreeModel::insertColumns(int position, int columns, const QModelIndex& parent)
{
    bool success;

    beginInsertColumns(parent, position, position + columns - 1);
    success = mRootItem->insertColumns(position, columns);
    endInsertColumns();

    return success;
}



bool TreeModel::removeColumns(int position, int columns, const QModelIndex& parent)
{
    bool success;

    beginRemoveColumns(parent, position, position + columns - 1);
    success = mRootItem->removeColumns(position, columns);
    endRemoveColumns();

    if (mRootItem->columnCount() == 0)
        removeRows(0, rowCount());
    return success;
}



/*!
 * \brief TreeModel::setupModelData
 *      Строит дерево по текстовому файлу.
 * \param lines            строки из файла.
 */
void TreeModel::setupModelData(const QStringList& lines, TreeItem* parent)
{
    QList<TreeItem*> parents;
    QList<int> indentations;
    parents << parent;
    indentations << 0;

    int number = 0;

    while (number < lines.count())
    {
        int position = 0;
        while (position < lines[number].length())
        {
            if (lines[number].at(position) != ' ')
                break;
            position++;
        }

        QString lineData = lines[number].mid(position).trimmed();

        if (!lineData.isEmpty())
        {
            // Read the column data from the rest of the line.
            QStringList columnStrings = lineData.split("\t", QString::SkipEmptyParts);
            QVector<QVariant> columnData;
            for (int column = 0; column < columnStrings.size(); column++)
                columnData << columnStrings[column];

            if (position > indentations.last())
            {
                // The last child of the current parent is now the new parent
                // unless the current parent has no children.

                if (parents.last()->childCount() > 0)
                {
                    parents << parents.last()->child(parents.last()->childCount() - 1);
                    indentations << position;
                }
            }
            else
            {
                while (position < indentations.last() && parents.size() > 0)
                {
                    parents.pop_back();
                    indentations.pop_back();
                }
            }

            // Append a new item to the current parent's list of children.
            TreeItem *parent = parents.last();
            parent->insertChildren(parent->childCount(), 1, mRootItem->columnCount());
            for (int column = 0; column < columnData.size(); column++)
                parent->child(parent->childCount() - 1)->setData(column, columnData[column]);
        }
        number++;
    }
}



TreeItem *TreeModel::getItem(const QModelIndex& index) const
{
    if (index.isValid())
    {
        TreeItem* item = static_cast<TreeItem*>(index.internalPointer());
        if (item != nullptr)
            return item;
    }
    return mRootItem;
}
