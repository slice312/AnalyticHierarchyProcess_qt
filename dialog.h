#ifndef DIALOG_H_INCLUDED
#define DIALOG_H_INCLUDED

#include <QDialog>
#include <QVector>
#include <QList>
#include <QTableView>
#include <vector>


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
    QList<QList<QStringList>> slist;     //имена критерив и альтернатив


public:
    Dialog(QVector<double> vals, QStringList& list, int index,
            std::vector<std::vector<double>> CR, QWidget* parent = nullptr);
    Dialog(const QList<QList<QStringList>>& names, QVector<int> nums,
           int alternatives, QWidget* parent = nullptr);
    ~Dialog();

    void setTitles(int prev, int level, const QList<QStringList>& list);
    void defaultValue();
    QList<double> calculate();

};

#endif // DIALOG_H_INCLUDED
