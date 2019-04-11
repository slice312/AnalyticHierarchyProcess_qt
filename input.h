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
    explicit input(QSpinBox* spin, QWidget *parent = nullptr);
    ~input();

private:
    Ui::input* ui;
    QStringList strs;
    QList<QLineEdit*> lines;

public:
    QStringList give();

private slots:
    void readAll();

};

#endif // INPUT_H
