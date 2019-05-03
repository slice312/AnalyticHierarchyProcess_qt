#include <QProcess>
#include <QDebug>
#include <QFile>
#include <QPair>
#include <QQueue>

#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "dialog.h"
#include "editableTreeView/treemodel.h"





MainWindow::MainWindow(QWidget* parent) :
    QMainWindow(parent)
{
    setupUi(this);
    toolbar->setMovable(false);
    resize(600, 500);

    mTreeView->setAlternatingRowColors(true);
    mTreeView->setSelectionBehavior(QAbstractItemView::SelectItems);
    mTreeView->setHorizontalScrollMode(QAbstractItemView::ScrollPerPixel);
    mTreeView->setAnimated(true);
    mTreeView->setAllColumnsShowFocus(true);

    QFile file(":/default.txt");
    file.open(QIODevice::ReadOnly);
    TreeModel* model = new TreeModel({"Критерии"}, file.readAll(), mTreeView);
    file.close();
    model->setObjectName("MY_MODEL");
    //    TreeModel* model = new TreeModel({"name", "state"});
    //    model->insertRow(0);
    //    auto index = model->index(0, 0);
    //    model->setData(index, "First");
    //    model->setData(model->index(0, 1), "[No data]");

    mTreeView->setObjectName("MY_TREE_VIEW");

    mTreeView->setModel(model);

    for (int column = 0; column < model->columnCount(); column++)
        mTreeView->resizeColumnToContents(column);


    connect(mTreeView->selectionModel(), &QItemSelectionModel::selectionChanged,
            this, &MainWindow::updateActions);
    connect(mInsertRowAction, &QAction::triggered, this, &MainWindow::insertRow);
    connect(mRemoveRowAction, &QAction::triggered, this, &MainWindow::removeRow);
    connect(mInsertChildAction, &QAction::triggered, this, &MainWindow::insertChild);
    updateActions();
//    on_mOkButton_clicked();
}



MainWindow::~MainWindow()
{
    //    dumpObjectTree();
}



void MainWindow::updateActions()
{
    //    bool hasSelection = !view->selectionModel()->selection().isEmpty();
    bool hasCurrent = mTreeView->selectionModel()->currentIndex().isValid();
    mRemoveRowAction->setEnabled(hasCurrent);
    mInsertChildAction->setEnabled(hasCurrent);

    if (hasCurrent)
    {
        mTreeView->closePersistentEditor(mTreeView->selectionModel()->currentIndex());
    }
}


void MainWindow::on_resetButton_clicked()
{
    qApp->quit();
    QProcess::startDetached(qApp->arguments()[0], qApp->arguments());
}







void MainWindow::on_altsSpin_valueChanged(int value)
{
    QVBoxLayout* layout = mAltsNameVLayout;

    while (value < layout->count())
    {
        QLayoutItem* item = layout->takeAt(layout->count() - 1);
        QWidget* wgt = item->widget();
        wgt->hide();
        layout->removeWidget(wgt);
        delete wgt;
    }

    for (int i = layout->count(); i < value; i++)
    {
        QLineEdit* lineEdit = new QLineEdit();
        layout->addWidget(lineEdit);
    }
}



void MainWindow::on_mOkButton_clicked()
{
    // QList<QList<QTableView*>> tableHierarchy;

    QAbstractItemModel* model = mTreeView->model();

    int rows = model->rowCount();
    //    for (int level = 0; level < model->rowCount(); level++)
    //    {
    //        QModelIndex index = model->index(level, 0);
    //        index.data();
    //        int ones = model->rowCount(index);
    //        for (int i = 0; i < 3; ++i) {
    //        }
    //    }

    // TODO обход дерева в ширину
    // сначала в корню -                   это критерии первого уровня
    // потом по всем детям, если есть -    это критерии второго уровня и т.д.

    QQueue<QPair<QModelIndex, bool>> que;


    QModelIndex index = model->index(0, 0).parent();
    bool check = index.isValid();
    que.enqueue(qMakePair(index, false));


//    int level = 0;
//    QVector<QStringList> levels;
//    levels.resize(3);


//    while (!que.empty())
//    {
//        int level_size = que.size();
//        while (level_size--)
//        {
//            auto pair = que.dequeue();
//            pair.second = true;

//            int count = model->rowCount(pair.first);
//            for (int i = 0; i < count; i++)
//            {
//                //-----------------------------
//                QModelIndex ix = model->index(i, 0, pair.first);
//                bool isValid = ix.isValid();
//                QVariant name = model->data(ix);

//                levels[level].append(name.toString());
//                //-----------------------------

//                if (isValid)
//                    que.enqueue(qMakePair(ix, false));
//            }
//        }
//        level++;
//    }

//    qDebug() << "LEVEL=" << level;
//    qDebug() << "LEVEL=" << level;

//    for (int level = 0; ; level++)
//    {
//        QStringList onLevel;
//        for (int i = 0; i < model->rowCount(); i++)
//        {
//            onLevel.append(model->data(model->index(i, 0)).toString());
//        }
//        levels.append(onLevel);

//    }

        Dialog wgt(mTreeView->model(), mAltsSpin->value(), this);
        wgt.setModal(true);
        wgt.setWindowState(Qt::WindowMaximized);
        wgt.exec();
}



void MainWindow::insertRow()
{
    QModelIndex index = mTreeView->selectionModel()->currentIndex();
    QAbstractItemModel* model = mTreeView->model();

    if (!model->insertRow(index.row() + 1, index.parent()))
        return;

    for (int column = 0; column < model->columnCount(index.parent()); column++)
    {
        QModelIndex child = model->index(index.row() + 1, column, index.parent());
        model->setData(child, QVariant("[No data]"), Qt::EditRole);
    }

    mTreeView->selectionModel()->
            setCurrentIndex(model->index(index.row() + 1, 0, index.parent()),
                            QItemSelectionModel::ClearAndSelect);
    updateActions();
}



void MainWindow::insertChild()
{
    QModelIndex index = mTreeView->selectionModel()->currentIndex();
    if (!index.isValid())
        return;
    QAbstractItemModel* model = mTreeView->model();
    if (model->columnCount(index) == 0)
    {
        if (!model->insertColumn(0, index))
            return;
    }

    if (!model->insertRow(0, index))
        return;

    for (int column = 0; column < model->columnCount(index); column++)
    {
        QModelIndex child = model->index(0, column, index);
        model->setData(child, QVariant("[No data]"), Qt::EditRole);
        if (!model->headerData(column, Qt::Horizontal).isValid())
        {
            model->setHeaderData(column, Qt::Horizontal, QVariant("[No header]"),
                                 Qt::EditRole);
        }
    }
    mTreeView->selectionModel()->setCurrentIndex(model->index(0, 0, index),
                                                 QItemSelectionModel::ClearAndSelect);
    updateActions();
}



void MainWindow::removeRow()
{
    QModelIndex index = mTreeView->selectionModel()->currentIndex();
    if (!index.isValid())
        return;
    QAbstractItemModel* model = mTreeView->model();
    if (model->removeRow(index.row(), index.parent()))
        updateActions();
}

