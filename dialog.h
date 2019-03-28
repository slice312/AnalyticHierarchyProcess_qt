#ifndef DIALOG_H_INCLUDED
#define DIALOG_H_INCLUDED

#include <QDialog>
#include <QVector>
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


public:
    Dialog(QVector<int> nums, int alternatives,
           QWidget* parent = nullptr);
    ~Dialog();

    void setTitles(int level, const QString& str);
    void defaultValue();





};

#endif // DIALOG_H_INCLUDED
