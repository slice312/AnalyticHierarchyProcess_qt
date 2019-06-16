#include <QLineEdit>
#include <QFile>

#include "mainwindow.h"
#include "dialog.h"
#include "editableTreeView/treemodel.h"



MainWindow::MainWindow(QWidget* parent) :
    QMainWindow(parent)
{
    setupUi(this);
    __toolbar->setMovable(false);

    QWidget* empty = new QWidget(__toolbar);
    empty->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);
    __toolbar->addWidget(empty);
    __toolbar->addAction(mResetAction);
    resize(600, 500);

    mTreeView->setAlternatingRowColors(true);
    mTreeView->setSelectionBehavior(QAbstractItemView::SelectItems);
    mTreeView->setHorizontalScrollMode(QAbstractItemView::ScrollPerPixel);
    mTreeView->setVerticalScrollMode(QAbstractItemView::ScrollPerPixel);
    mTreeView->setAnimated(true);
    mTreeView->setAllColumnsShowFocus(true);

    QFile file(":res/default.txt");
    file.open(QIODevice::ReadOnly);
    TreeModel* model = new TreeModel({"Критерии"}, file.readAll(), mTreeView);
    file.close();
    mTreeView->setModel(model);


    connect(mTreeView->selectionModel(), &QItemSelectionModel::selectionChanged,
            this, &MainWindow::updateActions);
    connect(mInsertRowAction, &QAction::triggered, this, &MainWindow::insertRow);
    connect(mRemoveRowAction, &QAction::triggered, this, &MainWindow::removeRow);
    connect(mInsertChildAction, &QAction::triggered, this, &MainWindow::insertChild);
    connect(mResetAction, &QAction::triggered, this, &MainWindow::reset);
    updateActions();
}



MainWindow::~MainWindow()
{
    //    dumpObjectTree();
}



void MainWindow::updateActions()
{
    bool hasCurrent = mTreeView->selectionModel()->currentIndex().isValid();
    mRemoveRowAction->setEnabled(hasCurrent);
    mInsertChildAction->setEnabled(hasCurrent);
}



void MainWindow::on_mAltsSpin_valueChanged(int value)
{
    QVBoxLayout* layout = mAltsNameLayout;

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
    QStringList altNames;
    for (int i = 0; i < mAltsNameLayout->count(); i++)
    {
        altNames.append(qobject_cast<QLineEdit*>(mAltsNameLayout->itemAt(i)->widget())->text());
    }

    Dialog wgt(mTreeView->model(), altNames, this);
    wgt.setModal(true);
    wgt.setWindowState(Qt::WindowMaximized);
    wgt.exec();
}



void MainWindow::reset()
{
    QAbstractItemModel* model = mTreeView->model();
    const QModelIndex index = model->index(0, 0).parent();
    while (model->rowCount(index) > 0)
        model->removeRow(0, index);
    updateActions();
    mAltsSpin->setValue(0);
}



void MainWindow::insertRow()
{
    const QModelIndex index = mTreeView->selectionModel()->currentIndex();
    QAbstractItemModel* model = mTreeView->model();

    if (!model->insertRow(index.row() + 1, index.parent()))
        return;

    for (int column = 0; column < model->columnCount(index.parent()); column++)
    {
        const QModelIndex child = model->index(index.row() + 1, column, index.parent());
        model->setData(child, QVariant("[No data]"), Qt::EditRole);
    }

    const QModelIndex curIx = model->index(index.row() + 1, 0, index.parent());

    mTreeView->selectionModel()->setCurrentIndex(curIx, QItemSelectionModel::ClearAndSelect);
    emit mTreeView->edit(curIx);
    updateActions();
}



void MainWindow::insertChild()
{
    const QModelIndex index = mTreeView->selectionModel()->currentIndex();
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
        const QModelIndex child = model->index(0, column, index);
        model->setData(child, QVariant("[No data]"), Qt::EditRole);
        if (!model->headerData(column, Qt::Horizontal).isValid())
        {
            model->setHeaderData(column, Qt::Horizontal, QVariant("[No header]"),
                                 Qt::EditRole);
        }
    }

    const QModelIndex curIx = model->index(0, 0, index);
    mTreeView->selectionModel()->setCurrentIndex(curIx, QItemSelectionModel::ClearAndSelect);
    emit mTreeView->edit(curIx);
    updateActions();
}



void MainWindow::removeRow()
{
    const QModelIndex index = mTreeView->selectionModel()->currentIndex();
    if (!index.isValid())
        return;
    QAbstractItemModel* model = mTreeView->model();
    if (model->removeRow(index.row(), index.parent()))
        updateActions();
}
