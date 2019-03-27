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

    QVBoxLayout* mainLayout = new QVBoxLayout(ui->scrollAreaWidgetContents);
    QScrollArea* scroll = new QScrollArea(ui->scrollAreaWidgetContents);
    scroll->setWidgetResizable(true);


    QStandardItemModel* model = new QStandardItemModel();
    QTableView* table = new QTableView();
    table->setModel(model);


    QWidget* widget = new QWidget();
    QHBoxLayout* hlayout = new QHBoxLayout(widget);
    hlayout->addWidget(table);
    scroll->setWidget(widget);
    mainLayout->addWidget(scroll);



    for (int i = 1; i < level; i++)
    {
        scroll = new QScrollArea(ui->scrollAreaWidgetContents);
        scroll->setWidgetResizable(true);
        widget = new QWidget();
        hlayout = new QHBoxLayout(widget);
        scroll->setWidget(widget);
        mainLayout->addWidget(scroll);


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
            table->setMinimumWidth(250);
            hlayout->addWidget(table);
        }
    }


}


Dialog::~Dialog()
{
    qDebug() << "Dialog destroyed";
    delete ui;
}
