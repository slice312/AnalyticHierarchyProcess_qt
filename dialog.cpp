#include <QTextEdit>
#include <QStandardItemModel>
#include <QPushButton>
#include <QHeaderView>
#include <QDebug>

#include "dialog.h"
#include "ui_dialog.h"
#include "spinboxdelegate.h"
#include "alg_ahp.h"



Dialog::Dialog(const QStringList& list, int trueIndex, const QVector<double>& vals,
               const QVector<QVector<double>>& CR, QWidget* parent) :
    QDialog(parent), ui(new Ui::Dialog())
{
    ui->setupUi(this);
    this->setMinimumSize(500, 500);

    QVBoxLayout* mainLayout = new QVBoxLayout(ui->scrollAreaWidgetContents);
    mainLayout->setMargin(20);

    QTextEdit* text = new QTextEdit("\n");
    text->setReadOnly(true);

    text->append("Комбинированные весовые коэффициенты");
    for (int i = 0; i < vals.size(); i++)
    {
        text->append(list[i] + "\t" + QString::number(vals[i]));
    }

    if (trueIndex >= 0)
    {
        text->append("Лучший вариант: \"" + list[trueIndex] + "\" \t" +
                   QString::number(vals[trueIndex]));
    }
    mainLayout->addWidget(text);


    text->append("\n\n\nКоэффициенты согласованности (CR) для каждой матрицы");
    int count = 1;
    for (int i = 0; i < CR.size(); i++)
    {
        for (int j = 0; j < CR[i].size(); j++)
        {
            text->append(QVariant(count++).toString() + ") " + QVariant(CR[i][j]).toString());
        }
    }
}



Dialog::Dialog(const QList<QList<QStringList>>& names, int alternatives, QWidget* parent) :
    QDialog(parent, Qt::Window), ui(new Ui::Dialog())
{
    ui->setupUi(this);
    this->setMinimumSize(680, 450);
    this->names = names;
    this->levels = names.size();
    this->alternatives = alternatives;

    QVBoxLayout* mainLayout = new QVBoxLayout(ui->scrollAreaWidgetContents);
    mainLayout->setAlignment(Qt::AlignCenter);
    ui->scrollAreaWidgetContents->setLayout(mainLayout);


    // Построение TableView для каждого уровня декомпозиции.
    QList<QTableView*> tablesOnLevel;
    QStringList chained;

    for (int lvl = 0; lvl < levels; lvl++)
    {
        QScrollArea* scroll = new QScrollArea(ui->scrollAreaWidgetContents);
        scroll->setWidgetResizable(true);
        mainLayout->addWidget(scroll);
        QWidget* widget = new QWidget();
        scroll->setWidget(widget);
        QHBoxLayout* hlayout = new QHBoxLayout(widget);

        chained.clear();
        if (lvl > 0)
        {
            for (int i = 0; i < names[lvl - 1].size(); i++)
            {
                for (int j = 0; j < names[lvl - 1][i].size(); j++)
                {
                    chained.push_back(names[lvl - 1][i][j]);
                }
            }
        }
        else
            chained.push_back("");


        for (int t = 0, tables = names[lvl].size(); t < tables; t++) // tables - кол-во таблиц на уровне
        {
            QTableView* table = new QTableView();
            tablesOnLevel.push_back(table);
            QStandardItemModel* model = new QStandardItemModel(table);

            //Матрицы всегда квадратные.
            //уровень -> таблица-> кол-во строк.
            int rowcol = names[lvl][t].size();

            model->setRowCount(rowcol);
            model->setColumnCount(rowcol);

            model->setHorizontalHeaderLabels(names[lvl][t]);
            model->setVerticalHeaderLabels(names[lvl][t]);

            table->setModel(model);
            table->setMinimumWidth(250);  //TODO размеры
            table->setMinimumHeight(200);

            SpinBoxDelegate* delegate =
                    new SpinBoxDelegate(rowcol, rowcol, this);
            table->setItemDelegate(delegate);

            QLabel* label = getLabel(":/pics/indicator_gray.svg", QRect(10, 10, 25, 25), this);
            connect(delegate, &SpinBoxDelegate::indicate, [label](bool state)
            {
                if (state == true)
                    label->setPixmap(QIcon(":/pics/indicator_green.svg").pixmap(label->size()));
                else
                    label->setPixmap(QIcon(":/pics/indicator_red.svg").pixmap(label->size()));
            });

            QWidget* info = new QWidget();
            QVBoxLayout* v = new QVBoxLayout();
            v->setContentsMargins(0, 0, 0, info->height() * 0.25);
            qDebug() << info->height();

            v->addWidget(new QLabel(chained[t]));
            v->addWidget(label);
            info->setLayout(v);

            hlayout->addWidget(info);
            hlayout->addWidget(table);
        }
        this->tableHierarchy.push_back(tablesOnLevel);
        tablesOnLevel.clear();
    }

    QPushButton* calcButton = new QPushButton("Вычислить", this);
    mainLayout->addWidget(calcButton, Qt::AlignCenter);
    calcButton->setMaximumWidth(300);

    connect(calcButton, &QPushButton::clicked, this, &Dialog::calculate);
    setDefaultValues();
}



Dialog::~Dialog()
{
    dumpObjectTree();
    delete ui;
}




void Dialog::calculate()
{
    qDebug() << "clicked CALCULATE";

    AlghorithmAHP ahp(alternatives);
    QVector<QVector<double>> indexCR;

    std::vector<Matrix> vec;
    for (QList<QTableView*> tablesOnLevel : tableHierarchy)
    {
        vec.clear();
        for (QTableView* table : tablesOnLevel)
        {
            QAbstractItemModel* model = table->model();
            Matrix mtx(model->rowCount(), model->columnCount());

            for (int row = 0; row < model->rowCount(); row++)
            {
                for (int col = 0; col < model->columnCount(); col++)
                {
                    QModelIndex index = model->index(row, col);
                    QVariant var = model->data(index);
                    mtx(row, col) = var.toDouble();
                }
            }
            vec.push_back(mtx);
        }
        indexCR.push_back(QVector<double>::fromStdVector(ahp.addLevel(vec)));
    }

    auto pair = ahp.answer();
    QStringList altsNames = names.back().back();
    Dialog wgt(altsNames, pair.first,
               QVector<double>::fromStdVector(pair.second),
               indexCR, this);

    wgt.setModal(true);
    wgt.setWindowTitle("Ответ");
    wgt.setMinimumSize(300, 250);
    wgt.exec();
}



void Dialog::setDefaultValues()
{
    for (QList<QTableView*> tablesOnLevel : tableHierarchy)
    {
        for (QTableView* table : tablesOnLevel)
        {
            SpinBoxDelegate* delegate = qobject_cast<SpinBoxDelegate*>(table->itemDelegate());
            QAbstractItemModel* model = table->model();

            for (int col = 0; col < model->columnCount(); col++)
            {
                QModelIndex index = model->index(col, col);
                model->setData(index, 1);
                delegate->lockIndex(index);
                table->horizontalHeader()->resizeSection(col, 60); //TODO размеры
                table->verticalHeader()->resizeSection(col, 25);
            }
        }
    }
}



QLabel* Dialog::getLabel(const QString& file, const QRect& rect,
                         QWidget* parent) const
{
    QLabel* label = new QLabel(parent);
    label->setGeometry(rect);
    label->setFixedSize(rect.size());
    label->setAlignment(Qt::AlignCenter);

    QPixmap pix = QIcon(file).pixmap(label->size());
    label->setPixmap(pix);
    return label;
}
