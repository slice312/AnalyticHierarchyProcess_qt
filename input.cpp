#include "input.h"
#include "ui_input.h"
#include "mainwindow.h"
#include <QPushButton>
#include  <QDialogButtonBox>


input::input(QSpinBox* spin, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::input)
{
    ui->setupUi(this);
    for (int i = 0; i < spin->value(); ++i) {
        QLineEdit* line = new QLineEdit(this);
        this->lines.push_back(line);
        ui->vLayout->addWidget(line);
    }

    QPushButton* buttonOk = ui->buttonBox->button(QDialogButtonBox::Ok);
    connect(buttonOk, SIGNAL(clicked()), this, SLOT(readAll()));
}

input::~input()
{
    delete ui;
}


QStringList input::give()
{
    return strs;
}


void input::readAll()
{
    for (QLineEdit* line : lines)
    {
        strs.push_back(line->text());
    }

}
