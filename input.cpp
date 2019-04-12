#include "input.h"
#include "ui_input.h"
#include "mainwindow.h"
#include <QPushButton>
#include  <QDialogButtonBox>


input::input(int prev, QSpinBox* spin, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::input)
{
    ui->setupUi(this);
    this->count = prev;



    for (int i = 0; i < spin->value() * prev; ++i) {
        QLineEdit* line = new QLineEdit(this);
        this->lines.push_back(line);
        ui->vLayout->addWidget(line);
    }

    QPushButton* buttonOk = ui->buttonBox->button(QDialogButtonBox::Ok);
    QPushButton* cancel = ui->buttonBox->button(QDialogButtonBox::Cancel);
    connect(buttonOk, SIGNAL(clicked()), this, SLOT(readAll()));
    connect(cancel, SIGNAL(clicked()), this, SLOT(cance()));
}

input::~input()
{
    delete ui;
}


QList<QStringList> input::give()
{
    return strs;
}


void input::readAll()
{
    for (int i = 0; i < count; ++i)
    {
        QStringList ls;
        for (int c = 0, n = i; c < lines.size() / count; ++n, ++c)
        {
            ls.push_back((lines[i + n]->text()));
        }
        strs.push_back(ls);
    }
}
