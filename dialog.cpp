#include <QStandardItemModel>
#include <QTableView>
#include <QDebug>
#include <QScrollArea>


#include "dialog.h"
#include "ui_dialog.h"




Dialog::Dialog(int level, QVector<int> nums, int alternatives, QWidget* parent) :
    QDialog(parent, Qt::Window), ui(new Ui::Dialog)
{
    ui->setupUi(this);
    qDebug() << nums;


    QWidget* widget = new QWidget();
    ui->mainLayout->addWidget(widget);
    QHBoxLayout* hlayout = new QHBoxLayout(widget);

    QStandardItemModel* model = new QStandardItemModel();
    QTableView* table = new QTableView();
    table->setModel(model);
    hlayout->addWidget(table);





    for (int i = 1; i < level; i++)
    {
        widget = new QWidget();
        ui->mainLayout->addWidget(widget);
        hlayout = new QHBoxLayout(widget);

        int tables = 1;
        for (int n = i - 1; n >= 0; n--)
        {
            tables *= nums[n];
        }

        for (int j = 0; j < tables; j++)
        {
            model = new QStandardItemModel();
            table = new QTableView();
            table->setModel(model);
            hlayout->addWidget(table);
        }
    }


}


Dialog::~Dialog()
{
    qDebug() << "Dialog destroyed";
    delete ui;
}
