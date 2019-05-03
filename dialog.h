#ifndef DIALOG_H_INCLUDED
#define DIALOG_H_INCLUDED

#include <QDialog>
#include <QVector>
#include <QList>
#include <QTableView>
#include <QVBoxLayout>
#include <QLabel>
#include "ui_dialog.h"
#include "spinboxdelegate.h"




class Dialog : public QDialog, private Ui::Dialog
{
    Q_OBJECT

private:
    int levels;
    QStringList mAlternatives;
    QList<QList<QTableView*>> tableHierarchy;
    QList<QList<QStringList>> names;     //имена критериев и альтернатив




public:
    Dialog(const QStringList& list, int trueIndex, const QVector<double>& vals,
           const QVector<QVector<double>>& CR, QWidget* parent = nullptr);

    Dialog(QAbstractItemModel* tree, const QStringList& alternatives,
           QWidget* parent = nullptr);
    ~Dialog();

    void calculate();


private:
    void bfs(QAbstractItemModel* tree);
    void dfs(QAbstractItemModel* tree);
    void setDefaultValues();
    QLabel* getLabel(const QString& file, const QRect& rect,
                     QWidget* parent) const;
    void connectIndicator(QLabel* receiver, const SpinBoxDelegate* sender);
};



#endif // DIALOG_H_INCLUDED
