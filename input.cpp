#include <cassert>
#include "input.h"
#include "ui_input.h"



Input::Input(int count, int groups, QWidget* parent) :
    QDialog(parent), ui(new Ui::Input)
{
    ui->setupUi(this);
    this->groups = groups;

    for (int i = 0; i < count; i++)
    {
        QLineEdit* line = new QLineEdit(this);
        this->qlines.push_back(line);
        ui->vLayout->addWidget(line);
    }
}



Input::~Input()
{
    delete ui;
}



QList<QStringList> Input::getNames() const
{
    QList<QStringList> strs;
    auto iterator = qlines.begin();

    for (int i = 0; i < groups; i++)
    {
        QStringList list;
        for (int j = 0; j < qlines.size() / groups; j++)
        {
            assert(iterator != qlines.end() && "iterator out of range, input::getNames()");
            list.push_back((*iterator)->text());
            ++iterator;
        }
        strs.push_back(list);
    }
    return strs;
}
