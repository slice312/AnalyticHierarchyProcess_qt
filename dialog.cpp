#include <QStandardItemModel>
#include <QDebug>
#include <QScrollArea>
#include <QPushButton>
#include <QHeaderView>

#include "dialog.h"
#include "ui_dialog.h"
#include "spinboxdelegate.h"




Dialog::Dialog(QVector<int> nums, int alternatives, QWidget* parent) :
    QDialog(parent, Qt::Window), ui(new Ui::Dialog)
{
    ui->setupUi(this);
    this->levels = nums.size();
    this->alternatives = alternatives;
    qDebug() << nums;

    QVBoxLayout* mainLayout = new QVBoxLayout(ui->scrollAreaWidgetContents);
    mainLayout->setObjectName("mainLayout");
    QVector<QTableView*> vector;


    // Построение TableView для каждого уровня декомпозиции.
    for (int i = 0; i < levels; i++)
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

        vector.clear();

        for (int j = 0; j < tables; j++)
        {
            QTableView* table = new QTableView();
            QStandardItemModel* model = new QStandardItemModel(table);
            model->setRowCount(nums[i]);
            model->setColumnCount(nums[i]);
//            model->set

            table->setModel(model);
            table->setMinimumWidth(250);
            table->setItemDelegate(new SpinBoxDelegate(this));
            hlayout->addWidget(table);
            vector.push_back(table);

            //DEBUG:
            model->setObjectName("model_" + tr("%1_%2").arg(i).arg(j));
            table->setObjectName("table_" + tr("%1_%2").arg(i).arg(j));
        }
        this->vecTables.push_back(vector);
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

    vector.clear();
    for (int i = 0; i < tables; i++)
    {
        QTableView* table = new QTableView();
        QStandardItemModel* model = new QStandardItemModel(table);
        model->setRowCount(alternatives);
        model->setColumnCount(alternatives);
        table->setModel(model);
        table->setMinimumWidth(250);
        table->setItemDelegate(new SpinBoxDelegate(this));
        hlayout->addWidget(table);
        vector.push_back(table);
    }

    this->vecTables.push_back(vector);



    mainLayout->addWidget(new QPushButton("Расчитать", this));
}



Dialog::~Dialog()
{
    dumpObjectTree();
    qDebug() << "Dialog destroyed";
    delete ui;
}



void Dialog::setTitles(int level, const QString&)
{
    for (auto i : vecTables[level])
    {
        dynamic_cast<QStandardItemModel*>(i->model())->
                setHorizontalHeaderLabels({"a", "b", "C"});
    }
}




void Dialog::defaultValue()
{
    for (QVector<QTableView*> vector : vecTables)
    {
        for (QTableView* table : vector)
        {
            SpinBoxDelegate* delegate = dynamic_cast<SpinBoxDelegate*>(table->itemDelegate());
            QAbstractItemModel* model = table->model();

            for (int col = 0; col < model->rowCount(); col++)
            {
                QModelIndex index = model->index(col, col);
                model->setData(index, 1);
                table->horizontalHeader()->resizeSection(col, 60);
                delegate->lockIndex(index);
            }
        }
    }
}





