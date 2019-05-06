#ifndef DIALOG_H_INCLUDED
#define DIALOG_H_INCLUDED

#include <QDialog>
#include <QVector>
#include <QList>
#include <QTableView>
#include <QStandardItemModel>
#include <QVBoxLayout>
#include <QLabel>
#include <QPair>

#include "ui_dialog.h"
#include "spinboxdelegate.h"
#include "editableTreeView/treeitem.h"
#include "alg_ahp.h"




class Dialog : public QDialog, private Ui::Dialog
{
    Q_OBJECT

private:
    int mLevels;
    QStringList mAlternatives;
    TreeNode<QTableView*>* mTree;




public:
    Dialog(const QStringList& list, int trueIndex, const QVector<double>& vals,
           const QVector<QVector<double>>& CR, QWidget* parent = nullptr);

    Dialog(QAbstractItemModel* tree, const QStringList& alternatives,
           QWidget* parent = nullptr);
    ~Dialog();

    void calculate();


private:
    void fillCriterias(QAbstractItemModel* tree);
    void BFS();
    void DFS();

    void setDefaultValues();

    QLabel* createIndicator(const QString& file, const QRect& rect,
                     QWidget* parent) const;
    void connectIndicator(QLabel* receiver, const SpinBoxDelegate* sender);
};



#endif // DIALOG_H_INCLUDED
