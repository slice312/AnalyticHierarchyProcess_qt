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
    Dialog(const QStringList& list, int trueIndex, const QVector<double>& vals,
           const QVector<QVector<double>>& CR, QWidget* parent = nullptr);

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
    void connectIndicator(QLabel* receiver, const SpinBoxDelegate* sender);


private slots:
    void calculate();
};



#endif // DIALOG_H_INCLUDED
