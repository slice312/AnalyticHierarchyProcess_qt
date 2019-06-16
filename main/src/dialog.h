#ifndef DIALOG_H_INCLUDED
#define DIALOG_H_INCLUDED

#include <QDialog>
#include <QVector>
#include <QTableView>
#include <QLabel>

#include "ui_dialog.h"
#include "delegate/spinboxdelegate.h"
#include "compute/ahp.h"



class Dialog : public QDialog, private Ui::Dialog
{
    Q_OBJECT

private:
    int mLevels;
    QStringList mAlternatives;
    TreeNode<QTableView*>* mTree;


public:
    Dialog(const QAbstractItemModel* tree, const QStringList& alternatives,
           QWidget* parent = nullptr);

    ~Dialog();

private:
    void buildTree(const QAbstractItemModel* tree);
    void setCriteriasOnLayout();
    void setAlternativesOnLayout();

    void putsOnLayout(QLayout* layout, QTableView* table);

    void setDefaultValues(TreeNode<QTableView*>* node);

    QLabel* createIndicator(const QString& file, const QRect& rect, QWidget* parent) const;


private slots:
    void calculate();
};



#endif // DIALOG_H_INCLUDED
