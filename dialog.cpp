#include <QStandardItemModel>
#include <QDebug>
#include <QScrollArea>
#include <QPushButton>
#include <QHeaderView>
#include <QScrollBar>
#include <QLabel>
#include <QTextEdit>

#include "dialog.h"
#include "ui_dialog.h"
#include "spinboxdelegate.h"
#include "alg_ahp.h"



Dialog::Dialog(QVector<double> vals, QStringList& list, int index, QWidget* parent):
    QDialog(parent),
    ui(new Ui::Dialog)
{
    qDebug() << "output";
    qDebug() << vals.size();
    qDebug() << list.size();
    ui->setupUi(this);
    QVBoxLayout* mainLayout = new QVBoxLayout(ui->scrollAreaWidgetContents);
    mainLayout->setMargin(20);
    mainLayout->addWidget(new QLabel("Ответ"));

    QTextEdit* text = new QTextEdit("\n");
    text->setReadOnly(true);

    text->append("Комбинированные весовые коэффициенты");
    for (int i = 0; i < vals.size(); i++)
    {
        text->append(list[i] + "\t" + QString::number(vals[i]));
    }

    if (index > 0)
        text->append("Лучший вариант: \"" + list[index - 1] + "\" \t\t" + QString::number(vals[index]));
    mainLayout->addWidget(text);


}



Dialog::Dialog(QVector<int> nums, int alternatives, QWidget* parent) :
    QDialog(parent, Qt::Window), ui(new Ui::Dialog)
{
    ui->setupUi(this);
    this->levels = nums.size();
    this->alternatives = alternatives;
    qDebug() << nums;

    QVBoxLayout* mainLayout = new QVBoxLayout(ui->scrollAreaWidgetContents);
    ui->scrollAreaWidgetContents->setLayout(mainLayout);

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
            table->setModel(model);
            table->setMinimumWidth(250);
            table->setMinimumHeight(200);
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
        //        table->setMinimumHeight(300);
        table->setMinimumWidth(300);
        table->setItemDelegate(new SpinBoxDelegate(this));
        hlayout->addWidget(table);
        vector.push_back(table);
    }

    this->vecTables.push_back(vector);

    // TODO переписать
    mainLayout->setStretch(3, 20);
    mainLayout->setStretch(0, 11);
    mainLayout->setStretch(1, 16);
    mainLayout->setStretch(2, 16);



    QPushButton* calcButton = new QPushButton("Расчитать", this);
    mainLayout->addWidget(calcButton);
    //    calcButton->setChecked(false);
    calcButton->setDefault(true);
    connect(calcButton, &QPushButton::clicked, this, &Dialog::calculate);
}





Dialog::~Dialog()
{
    dumpObjectTree();
    qDebug() << "Dialog destroyed";
    delete ui;
}


// TODO дописать
void Dialog::setTitles(int level, const QStringList& list)
{
    this->slist = QStringList(list);
    for (QTableView* table : vecTables[level])
    {
        QAbstractItemModel* model = table->model();
        QStandardItemModel* mod = dynamic_cast<QStandardItemModel*>(model);
        mod->setHorizontalHeaderLabels(list);
        mod->setVerticalHeaderLabels(list);
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

            for (int col = 0; col < model->columnCount(); col++)
            {
                QModelIndex index = model->index(col, col);
                model->setData(index, 1);
                delegate->lockIndex(index);
                table->horizontalHeader()->resizeSection(col, 60);
                table->verticalHeader()->resizeSection(col, 25);
            }
        }
    }
}



QList<double> Dialog::calculate()
{
    qDebug() << "clicked CALCULATE";
    AlghorithmAHP ahp(this->alternatives);

    std::vector<Matrix> list;
    for (QVector<QTableView*> vector : vecTables)
    {
        list.clear();
        for (QTableView* table : vector)
        {
            QAbstractItemModel* model = table->model();
            Matrix mtx(model->rowCount(), model->columnCount());

            for (int row = 0; row < model->rowCount(); row++)
            {
                for (int col = 0; col < model->columnCount(); col++)
                {
                    QModelIndex index = model->index(row, col);
                    QVariant var = model->data(index);
                    qDebug() << var.toDouble();
                    mtx(row, col) = var.toDouble();
                }
            }
            list.push_back(mtx);
        }
        ahp.addLevel(list);
    }


    auto pair = ahp.answer();

    Dialog wgt(QVector<double>::fromStdVector(pair.second), this->slist, pair.first, this);
    wgt.defaultValue();

    wgt.setModal(true);
    wgt.setMinimumSize(300, 200);
    wgt.exec();


    return QList<double>();
}




