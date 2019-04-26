#ifndef DIALOG_H_INCLUDED
#define DIALOG_H_INCLUDED

#include <QDialog>
#include <QVector>
#include <QList>
#include <QTableView>
#include <QLabel>



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
    QList<QList<QTableView*>> tableHierarchy;
    QList<QList<QStringList>> names;     //имена критериев и альтернатив


public:
    Dialog(const QStringList& list, int trueIndex, const QVector<double>& vals,
           const QVector<QVector<double>>& CR, QWidget* parent = nullptr);

    Dialog(const QList<QList<QStringList>>& names,
           int alternatives, QWidget* parent = nullptr);

    ~Dialog();

    void calculate();


private:
    void setDefaultValues();
    QLabel* getLabel(const QString& file, const QRect& rect,
                     QWidget* parent) const;
};



#endif // DIALOG_H_INCLUDED
