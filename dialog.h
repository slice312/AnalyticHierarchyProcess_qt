#ifndef DIALOG_H
#define DIALOG_H

#include <QDialog>
#include <QVector>



namespace Ui {
class Dialog;
}

class Dialog : public QDialog
{
    Q_OBJECT

private:
    Ui::Dialog* ui;


public:
    Dialog(int level, QVector<int> nums, int alternatives,
            QWidget* parent = nullptr);
    ~Dialog();


};

#endif // DIALOG_H
