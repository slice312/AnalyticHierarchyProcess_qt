#include <QProcess>
#include <QDebug>

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

    this->spins.push_back(ui->spin);
    this->attachments.push_back(ui->toolbut);
}



MainWindow::~MainWindow()
{
    delete ui;
}



//FIXME перезагружает приложение
void MainWindow::on_resetButton_clicked()
{
    qApp->quit();
    QProcess::startDetached(qApp->arguments()[0], qApp->arguments());
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
    connect(toolbut, SIGNAL(clicked()), this, SLOT(on_toolbut_clicked()));

    this->spins.push_back(newSpin);
    this->attachments.push_back(toolbut);
}



void MainWindow::on_toolbut_clicked()
{
    int index = -1;
    for (int i = 0; i < attachments.size(); i++)
    {
        if (sender() == attachments[i])
        {
            index = i;
            break;
        }
    }

    input form((index > 0 ) ? spins[index - 1]->value() : 1, spins[index]->value(), this);
    form.setModal(true);
    form.exec();


    if (critNames.size() > index)
        critNames[index] = form.getNames();
    else
        critNames.push_back(form.getNames());

    qDebug() << critNames[index];
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



void MainWindow::on_okButton_clicked()
{
    //добавить название альтернатив
    QVBoxLayout* layout = ui->altsNameVLayout;
    QList<QStringList> altsLevel;
    QStringList list;
        for (int i = 0; i < layout->count(); i++)
    {
        auto edit = qobject_cast<QLineEdit*>(layout->itemAt(i)->widget());
        list.push_back(edit->text());
    }
    altsLevel.push_back(list);
    critNames.push_back(altsLevel);


    QVector<int> vec;
    for (QSpinBox* spin : spins)
        vec.push_back(spin->value());



    Dialog wgt(critNames, vec, ui->altsSpin->value());
    wgt.defaultValue();

    QList<QStringList> tmp;

    if (!this->critNames.isEmpty())
        for (int i = 0; i < vec.size() + 1; ++i)
            wgt.setTitles(critNames[i].size(), i, critNames[i]);


    wgt.setModal(true);
    wgt.setMinimumSize(800, 600);
    wgt.exec();
}
