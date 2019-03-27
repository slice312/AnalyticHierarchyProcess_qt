#include <QStandardItemModel>
#include <QTableView>
#include <QDebug>
#include <QScrollArea>
#include <QPushButton>


#include "dialog.h"
#include "ui_dialog.h"




Dialog::Dialog(int level, QVector<int> nums, int alternatives, QWidget* parent) :
    QDialog(parent, Qt::Window), ui(new Ui::Dialog)
{
    ui->setupUi(this);
    qDebug() << nums;

    QVBoxLayout* mainLayout = new QVBoxLayout(ui->scrollAreaWidgetContents);

    mainLayout->setObjectName("mainLayout");


    // Построение TableView для каждого уровня декомпозиции.
    for (int i = 0; i < level; i++)
    {
        QScrollArea* scroll = new QScrollArea(ui->scrollAreaWidgetContents);
        scroll->setWidgetResizable(true);
        QWidget* widget = new QWidget();
        QHBoxLayout* hlayout = new QHBoxLayout(widget);
        scroll->setWidget(widget);
        mainLayout->addWidget(scroll);

        //DEBUG:
        scroll->setObjectName("scroll_" + QString::number(i));
        widget->setObjectName("widget_" + QString::number(i));
        hlayout->setObjectName("hlayout_" + QString::number(i));


        int tables = 1;
        for (int n = i - 1; n >= 0; n--)
            tables *= nums[n];

        for (int j = 0; j < tables; j++)
        {
            QTableView* table = new QTableView();
            QStandardItemModel* model = new QStandardItemModel(table);
            model->setRowCount(nums[i]);
            model->setColumnCount(nums[i]);
            table->setModel(model);
            table->setMinimumWidth(250);
            hlayout->addWidget(table);


            //DEBUG:
            model->setObjectName("model_" + tr("%1_%2").arg(i).arg(j));
            table->setObjectName("table_" + tr("%1_%2").arg(i).arg(j));
        }
    }



    // TableView c матрицами для альтернатив
    QScrollArea* scroll = new QScrollArea(ui->scrollAreaWidgetContents);
    scroll->setWidgetResizable(true);
    QWidget* widget = new QWidget();
    QHBoxLayout* hlayout = new QHBoxLayout(widget);
    scroll->setWidget(widget);
    mainLayout->addWidget(scroll);

    int tables = 1;
    for (int n : nums)
        tables *= n;

    for (int i = 0; i < tables; i++)
    {
        QTableView* table = new QTableView();
        QStandardItemModel* model = new QStandardItemModel(table);
        model->setRowCount(alternatives);
        model->setColumnCount(alternatives);
        table->setModel(model);
        table->setMinimumWidth(250);
        hlayout->addWidget(table);
    }


    mainLayout->addWidget(new QPushButton("Расчитать", this));
}



Dialog::~Dialog()
{
    dumpObjectTree();
    qDebug() << "Dialog destroyed";
    delete ui;
}
