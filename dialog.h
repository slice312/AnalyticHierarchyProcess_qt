#ifndef DIALOG_H_INCLUDED
#define DIALOG_H_INCLUDED

#include <QDialog>
#include <QVector>
#include <QList>
#include <QTableView>
#include <QVBoxLayout>
#include <QLabel>




class Dialog : public QDialog
{
    Q_OBJECT

private:
    int levels;
    int alternatives;
    QList<QList<QTableView*>> tableHierarchy;
    QList<QList<QStringList>> names;     //имена критериев и альтернатив

    QVBoxLayout* mMainLayout;
    QWidget* mScrollAreaWidget;
    QHBoxLayout* tmplayout; //TODO  убрать


public:
    Dialog(const QStringList& list, int trueIndex, const QVector<double>& vals,
           const QVector<QVector<double>>& CR, QWidget* parent = nullptr);

    Dialog(QAbstractItemModel* tree,
           int alternatives, QWidget* parent = nullptr);
    ~Dialog();

    void calculate();


private:
    void init();
    void dfs(QAbstractItemModel* tree, const QModelIndex& index);
    void setDefaultValues();
    QLabel* getLabel(const QString& file, const QRect& rect,
                     QWidget* parent) const;
};



#endif // DIALOG_H_INCLUDED
