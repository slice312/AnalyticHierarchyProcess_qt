#include <QProcess>
#include <QDebug>
#include <QLineEdit>
#include <QSpinBox>

#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "dialog.h"
#include "input.h"


MainWindow::MainWindow(QWidget* parent) : QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->scrollArea->setMinimumWidth(250);
    ui->scrollArea->setMinimumHeight(150);
    ui->label1->setMinimumWidth(150);

    spins.push_back(ui->spin1);
    attachments.push_back(ui->toolbut1);

    connect(ui->toolbut1, SIGNAL(clicked()), this, SLOT(toolButton()));
}



MainWindow::~MainWindow()
{
    delete ui;
}



void MainWindow::on_addButton_clicked()
{
    QString str = "Ур.";
    int row = ui->criteriaLayout->rowCount();
    str += QString::number(row + 1);
    str += "  критерии";

    QSpinBox* newSpin = new QSpinBox();
    QToolButton* toolbut = new QToolButton();

    ui->criteriaLayout->addWidget(new QLabel(str), row, 0, 1, 1);
    ui->criteriaLayout->addWidget(newSpin, row, 1, 1, 1);
    ui->criteriaLayout->addWidget(toolbut, row, 2, 1, 1);

    toolbut->setText("...");
    connect(toolbut, SIGNAL(clicked()), this, SLOT(toolButton()));

    spins.push_back(newSpin);
    attachments.push_back(toolbut);
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


    if (!this->critNames.isEmpty())
        for (int i = 0; i < vec.size(); ++i)
            wgt.setTitles(i, this->critNames[i]);

    if (!list.isEmpty())
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



void MainWindow::toolButton()
{
    int index = -1;
    for (int i = 0; i < attachments.size(); ++i) {
        if (sender() == attachments[i]) {
            index = i;
            break;
        }
    }

    input wgt(spins[index], this);
    wgt.setModal(true);
    wgt.exec();


    if (this->critNames.size() > index)
        this->critNames[index] = wgt.give();
    else
        this->critNames.push_back(wgt.give());

    qDebug() << critNames[index];

}
