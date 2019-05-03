#include <QTextEdit>
#include <QStandardItemModel>
#include <QPushButton>
#include <QHeaderView>
#include <QDebug>
#include <QQueue>

#include <functional>

#include "dialog.h"
#include "ui_dialog.h"
#include "spinboxdelegate.h"
#include "alg_ahp.h"



Dialog::Dialog(const QStringList& list, int trueIndex, const QVector<double>& vals,
               const QVector<QVector<double>>& CR, QWidget* parent) :
    QDialog(parent)
{
    QVBoxLayout* _lay = new QVBoxLayout(this);
    QScrollArea* scrollArea = new QScrollArea(this);
    QWidget* scrollAreaWidget = new QWidget();
    scrollArea->setWidgetResizable(true);
    scrollArea->setWidget(scrollAreaWidget);
    _lay->addWidget(scrollArea);
    _lay->setAlignment(Qt::AlignCenter);
    this->setMinimumSize(680, 450);

    QVBoxLayout* mainLayout = new QVBoxLayout(scrollAreaWidget);
    mainLayout->setAlignment(Qt::AlignCenter);
    mainLayout->setMargin(20);
    scrollAreaWidget->setLayout(mainLayout);


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



Dialog::Dialog(QAbstractItemModel* tree, const QStringList& alternatives, QWidget* parent) :
    QDialog(parent, Qt::Window)
{
    setupUi(this);
    this->mAlternatives = alternatives;
    /*
     * Сначала обход в ширину для всех уровней критериев.
     * Потом обход в глубину для уровня альтернатив.
     */
    bfs(tree);
    dfs(tree);
    //        QPushButton* calcButton = new QPushButton("Вычислить", this);
    //        __vLayout2->addWidget(calcButton, Qt::AlignCenter);
    //        //    calcButton->setMaximumWidth(300);
    connect(mCalcButton, &QPushButton::clicked, this, &Dialog::calculate);
    setDefaultValues();
}



void Dialog::bfs(QAbstractItemModel* tree)
{
    QList<QTableView*> tablesOnLevel;
    QQueue<QModelIndex> que;
    que.enqueue(tree->index(0, 0).parent()); //root

    while (!que.empty())
    {
        QWidget* widget = new QWidget();
        QScrollArea* scroll = new QScrollArea();
        scroll->setWidgetResizable(true);
        scroll->setWidget(widget);
        mScrollLayout->addWidget(scroll);
        QHBoxLayout* hlayout = new QHBoxLayout();
        widget->setLayout(hlayout);

        bool inserted = false;
        int level_size = que.size();
        while (level_size-- > 0)
        {
            QStringList names;
            const QModelIndex index = que.dequeue();

            for (int i = 0; i < tree->rowCount(index); i++)
            {
                QModelIndex ix = tree->index(i, 0, index);
                if (ix.isValid()) que.enqueue(ix);
                names.append(tree->data(ix).toString());
            }

            if (names.isEmpty()) continue;
            inserted = true;
            QTableView* table = new QTableView();
            table->setHorizontalScrollMode(QAbstractItemView::ScrollPerPixel);
            tablesOnLevel.append(table);
            QStandardItemModel* model = new QStandardItemModel(table);

            //Матрицы всегда квадратные
            int rowcol = names.size();
            model->setRowCount(rowcol);
            model->setColumnCount(rowcol);
            model->setHorizontalHeaderLabels(names);
            model->setVerticalHeaderLabels(names);

            table->setModel(model);
            table->setMinimumWidth(250);  //TODO размеры
            table->setMinimumHeight(200);

            SpinBoxDelegate* delegate = new SpinBoxDelegate(rowcol, rowcol, this);
            table->setItemDelegate(delegate);
            QLabel* label = getLabel(":/pics/indicator_gray.svg", QRect(10, 10, 25, 25), this);
            connectIndicator(label, delegate);


            QWidget* info = new QWidget();
            QVBoxLayout* vbl = new QVBoxLayout();
            vbl->setContentsMargins(0, 0, 0, info->height() * 0.25);

            QLabel* title = new QLabel(tree->data(index).toString());
            title->setMinimumHeight(50);
            vbl->addWidget(title);
            vbl->addWidget(label);
            info->setLayout(vbl);

            hlayout->addWidget(info);
            hlayout->addWidget(table);
            names.clear();
        }

        if (inserted)
            this->tableHierarchy.push_back(tablesOnLevel);
        else
            delete mScrollLayout->takeAt(mScrollLayout->count() - 1)->widget();
        tablesOnLevel.clear();
    }
}



void Dialog::dfs(QAbstractItemModel* tree)
{
    QWidget* widget = new QWidget();
    QScrollArea* scroll = new QScrollArea();
    scroll->setWidgetResizable(true);
    scroll->setWidget(widget);
    mScrollLayout->addWidget(scroll);
    QHBoxLayout* hlayout = new QHBoxLayout();
    widget->setLayout(hlayout);

    tableHierarchy.append(QList<QTableView*>());

    std::function<void (QAbstractItemModel*, const QModelIndex&)> func;

    func = [this, hlayout, &func](QAbstractItemModel* tree, const QModelIndex& index)
    {
        const int count = tree->rowCount(index);
        if (count == 0)
        {
            QTableView* table = new QTableView();
            QStandardItemModel* model = new QStandardItemModel(table);

            int rowcol = 3;  //из альтернаитв
            model->setRowCount(rowcol);
            model->setColumnCount(rowcol);
            model->setHorizontalHeaderLabels(mAlternatives);
            model->setVerticalHeaderLabels(mAlternatives);

            table->setModel(model);
            table->resize(200, 100);
            table->setMinimumWidth(350);
            table->setMinimumHeight(200);
            table->setHorizontalScrollMode(QAbstractItemView::ScrollPerPixel);
            table->setVerticalScrollMode(QAbstractItemView::ScrollPerPixel);
            //        table->setMinimumWidth(250);  //TODO размеры
            //        table->setMinimumHeight(r200);

            SpinBoxDelegate* delegate = new SpinBoxDelegate(rowcol, rowcol, table);
            table->setItemDelegate(delegate);
            QLabel* label = getLabel(":/pics/indicator_gray.svg", QRect(10, 10, 25, 25), this);
            connectIndicator(label, delegate);


            QWidget* info = new QWidget();
            QVBoxLayout* v = new QVBoxLayout();
            v->setContentsMargins(0, 0, 0, info->height() * 0.25);

            v->addWidget(new QLabel(tree->data(index).toString()));
            v->addWidget(label);
            info->setLayout(v);

            hlayout->addWidget(info);
            hlayout->addWidget(table);
            tableHierarchy.back().append(table);
        }

        for (int i = 0; i < count; i++)
        {
            QModelIndex ix = tree->index(i, 0, index);
            func(tree, ix);
        }
    };

    func(tree, tree->index(0, 0).parent());
}




Dialog::~Dialog()
{
    //    dumpObjectTree();
}




void Dialog::calculate()
{
    qDebug() << "clicked CALCULATE";

    AlghorithmAHP ahp(mAlternatives.size());
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
    Dialog wgt(mAlternatives, pair.first,
               QVector<double>::fromStdVector(pair.second),
               indexCR, this);

    wgt.setModal(true);
    wgt.setWindowTitle("Ответ");
    wgt.setMinimumSize(300, 250);
    wgt.exec();
}



//void Dialog::init()
//{
//    QVBoxLayout* _lay = new QVBoxLayout(this);
//    QScrollArea* scrollArea = new QScrollArea(this);
//    mScrollAreaWidget = new QWidget();
//    scrollArea->setWidgetResizable(true);
//    scrollArea->setWidget(mScrollAreaWidget);
//    _lay->addWidget(scrollArea);
//    _lay->setAlignment(Qt::AlignCenter);
//    this->setMinimumSize(680, 450);

//    mMainLayout = new QVBoxLayout(mScrollAreaWidget);
//    mMainLayout->setAlignment(Qt::AlignCenter);
//    mMainLayout->setMargin(20);
//    mScrollAreaWidget->setLayout(mMainLayout);
//}



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


void Dialog::connectIndicator(QLabel* receiver, const SpinBoxDelegate* sender)
{
    connect(sender, &SpinBoxDelegate::indicate, [receiver](bool state)
    {
        if (state == true)
            receiver->setPixmap(QIcon(":/pics/indicator_green.svg").pixmap(receiver->size()));
        else
            receiver->setPixmap(QIcon(":/pics/indicator_red.svg").pixmap(receiver->size()));
    });

}
