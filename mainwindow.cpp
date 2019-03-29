#include <QProcess>
#include <QDebug>
#include <QLineEdit>
#include <QSpinBox>

#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "dialog.h"


MainWindow::MainWindow(QWidget* parent) : QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->scrollArea->setMinimumWidth(250);
    ui->scrollArea->setMinimumHeight(150);
    ui->label1->setMinimumWidth(150);

    spins.push_back(ui->spin1);
}



MainWindow::~MainWindow()
{
    delete ui;
}



void MainWindow::on_addButton_clicked()
{
    QString str = "Ур.";
    str += QString::number(ui->column->rowCount() + 1);
    str += "  критерии";

    QSpinBox* newSpin = new QSpinBox();
    ui->column->addRow(new QLabel(str), newSpin);
    spins.push_back(newSpin);
}



// TODO переписать этот метод, чтобы без перезагрузки
void MainWindow::on_resetButton_clicked()
{
    qApp->quit();
    QProcess::startDetached(qApp->arguments()[0], qApp->arguments());
}



void MainWindow::on_okButton_clicked()
{
    QVector<int> vec;
    for (QSpinBox* spin : spins)
        vec.push_back(spin->value());

    QStringList list;
    QVBoxLayout* layout = ui->altsNameVLayout;

    for (int i = 0; i < layout->count(); i++)
    {
        auto edit = dynamic_cast<QLineEdit*>(layout->itemAt(i)->widget());
        list.push_back(edit->text());
    }


    Dialog wgt(vec, ui->altsSpin->value());
    wgt.defaultValue();
    wgt.setTitles(vec.size(), list);

    wgt.setModal(true);
    wgt.setMinimumSize(800, 600);
    wgt.exec();
}



void MainWindow::on_altsSpin_valueChanged(int value)
{
    QVBoxLayout* layout = ui->altsNameVLayout;

    while (value < layout->count())
    {
        QLayoutItem* item = layout->takeAt(layout->count() - 1);
        QWidget* wgt = item->widget();
        wgt->hide();
        layout->removeWidget(wgt);
        delete wgt;
    }

    for (int i = layout->count(); i < value; i++)
    {
        QLineEdit* lineEdit = new QLineEdit();
        layout->addWidget(lineEdit);
    }
}
