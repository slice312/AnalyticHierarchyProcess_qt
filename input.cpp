#include <cassert>

#include <QPushButton>
#include <QDialogButtonBox>

#include "input.h"
#include "ui_input.h"



input::input(int prev, int current, QWidget* parent) :
    QDialog(parent),
    ui(new Ui::input)
{
    ui->setupUi(this);
    this->count = prev;


    for (int i = 0; i < current * prev; i++)
    {
        QLineEdit* line = new QLineEdit(this);
        this->lines.push_back(line);
        ui->vLayout->addWidget(line);
    }

    QPushButton* buttonOk = ui->buttonBox->button(QDialogButtonBox::Ok);
    QPushButton* cancel = ui->buttonBox->button(QDialogButtonBox::Cancel);
    connect(buttonOk, SIGNAL(clicked()), this, SLOT(readAll()));
    connect(cancel, SIGNAL(clicked()), this, SLOT(cancel()));
}



input::~input()
{
    delete ui;
}



QList<QStringList> input::getNames()
{
    QList<QStringList> strs;
    auto iterator = lines.begin();

    for (int i = 0; i < count; i++)
    {
        QStringList list;
        for (int j = 0; j < lines.size() / count; j++)
        {
            assert(iterator != lines.end() && "iterator out of range, input::getNames()");
            list.push_back((*iterator)->text());
            ++iterator;
        }
        strs.push_back(list);
    }
    return strs;
}
