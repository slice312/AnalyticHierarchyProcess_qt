#include <QProcess>
#include <QDebug>

#include <memory>

#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "dialog.h"


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->scrollArea->setMinimumWidth(250);
    ui->scrollArea->setMinimumHeight(150);
    ui->label1->setMinimumWidth(150);

    spins.push_back(QPair<QLabel*, QSpinBox*>(ui->label1, ui->spin1));
    this->level = 1;
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

    spins.push_back(QPair<QLabel*, QSpinBox*>(new QLabel(str), new QSpinBox()));
    ui->column->addRow(spins.back().first, spins.back().second);
    this->level++;
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
    for (auto pair : spins)
    {
        vec.push_back(pair.second->value());
    }

    Dialog wgt(this->level, vec, ui->altsSpin->value());
    wgt.setModal(true);
//    wgt.setFixedSize(500, 500);
    wgt.exec();
}
