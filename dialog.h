#ifndef DIALOG_H_INCLUDED
#define DIALOG_H_INCLUDED

#include <QDialog>
#include <QVector>
#include <QList>
#include <QTableView>


namespace Ui {
class Dialog;
}


class Dialog : public QDialog
{
    Q_OBJECT

private:
    Ui::Dialog* ui;

    int levels;
    int alternatives;
    QVector<QVector<QTableView*>> vecTables;
    static QList<QList<QStringList>> slist;


public:
    Dialog(QVector<double> vals, QStringList& list, int index = 0, QWidget* parent = nullptr);
    Dialog(QVector<int> nums, int alternatives, QWidget* parent = nullptr);
    ~Dialog();

    void setTitles(int prev, int level, const QList<QStringList>& list);
    void defaultValue();
    QList<double> calculate();

    static void fill(int level, const QList<QStringList>& list)
    {
        slist.push_back(list);
    }

};

#endif // DIALOG_H_INCLUDED
