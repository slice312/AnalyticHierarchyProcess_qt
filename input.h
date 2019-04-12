#ifndef INPUT_H
#define INPUT_H

#include <QDialog>
#include <QSpinBox>
#include <QList>
#include <QStringList>
#include <QLineEdit>
#include <QWidget>



namespace Ui {
class input;
}

class input : public QDialog
{
    Q_OBJECT

public:
    explicit input(int prev, QSpinBox* spin, QWidget *parent = nullptr);
    ~input();

private:
    int count;
    Ui::input* ui;
    QList<QStringList> strs;
    QList<QLineEdit*> lines;

public:
    QList<QStringList> give();

private slots:
    void readAll();

    void cance()
    {
        readAll();
    }

};

#endif // INPUT_H
