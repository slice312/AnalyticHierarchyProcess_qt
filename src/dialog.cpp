#include <QMessageBox>
#include <QStandardItemModel>
#include <QPushButton>
#include <QHeaderView>
#include <QQueue>
#include <QDebug>

#include <functional>

#include "dialog.h"



typedef TreeNode<QTableView*> Tree;



Dialog::Dialog(const QAbstractItemModel* tree, const QStringList& alternatives, QWidget* parent) :
    QDialog(parent, Qt::Window)
{
    setupUi(this);
    this->mAlternatives = alternatives;
    setWindowTitle("Матрицы парных сравнений");

    buildTree(tree);
    setCriteriasOnLayout();
    setAlternativesOnLayout();

    setDefaultValues(mTree);

    connect(mCalcButton, &QPushButton::clicked, this, &Dialog::calculate);
}



Dialog::~Dialog()
{
    //    dumpObjectTree();
    delete mTree;
}



/*!
 * \brief Dialog::fillCriterias
 * \param tree          дерево со строками, названиями критериев.
 */
void Dialog::buildTree(const QAbstractItemModel* tree)
{
    std::function<QStringList (const QModelIndex&)> header;
    std::function<Tree* (const QStringList&, const QString&)> getNewNode;
    std::function<Tree* (const QModelIndex&, QString)> create;


    header = [tree](const QModelIndex& index)
    {
        QStringList names;
        for (int i = 0; i < tree->rowCount(index); i++)
        {
            const QModelIndex ix = tree->index(i, 0, index);
            names.append(tree->data(ix).toString());
        }
        return names;
    };


    getNewNode = [](const QStringList& header, const QString& name)
    {
        QTableView* table = new QTableView();
        QStandardItemModel* model = new QStandardItemModel(table);
        //Матрицы всегда квадратные
        int rowcol = header.size();
        model->setRowCount(rowcol);
        model->setColumnCount(rowcol);

        model->setHorizontalHeaderLabels(header);
        model->setVerticalHeaderLabels(header);
        model->setObjectName(name);
        table->setModel(model);
        table->setObjectName("tv_" + name);
        return new TreeNode(table);
    };


    create = [&](const QModelIndex& index, QString name)
    {
        name += "\n" + tree->data(index).toString();
        const int count = tree->rowCount(index);

        if (count == 0)
        {
            //матрица с альтернативами
            return getNewNode(mAlternatives, name);
        }

        Tree* newNode = getNewNode(header(index), name);

        for (int i = 0; i < count; i++)
        {
            const QModelIndex ix = tree->index(i, 0, index);
            newNode->childs.append(create(ix, name));
        }
        return newNode;
    };

    mTree = create(tree->index(0, 0).parent(), "");
}



/*!
 * \brief Dialog::setCriteriasOnLayout (BFS)
 *      Проход дерева mTree в ширину, чтобы на виджете разместить матрицы критериев,
 *      каждую на своем уровне.
 */
void Dialog::setCriteriasOnLayout()
{
    QQueue<Tree*> que;
    que.enqueue(mTree); //root

    while (!que.empty())
    {
        QWidget* widget = new QWidget();
        QScrollArea* scroll = new QScrollArea();
        scroll->setWidgetResizable(true);
        scroll->setWidget(widget);
        mScrollLayout->addWidget(scroll);
        QHBoxLayout* hlayout = new QHBoxLayout();
        widget->setLayout(hlayout);

        int level_size = que.size();
        while (level_size-- > 0)
        {
            Tree* node = que.dequeue();
            for (int i = 0; i < node->childs.size(); i++)
            {
                if (node->childs[i]->childs.isEmpty()) continue; //без матриц альтернатив
                que.enqueue(node->childs[i]);
            }
            putsOnLayout(hlayout, node->data);
        }
    }
}



/*!
 * \brief Dialog::setAlternativesOnLayout (DFS)
 *      Проход дерева mTree в глубину, чтобы на виджете на последнем уровне (layout)
 *      разместить матрицы альтернатив.
 *      В mTree альтернативы могут быть на разных уровнях, если например некоторые критерии
 *      не будут раскладываться, а другие будут.
 */
void Dialog::setAlternativesOnLayout()
{
    QWidget* widget = new QWidget();
    QScrollArea* scroll = new QScrollArea();
    scroll->setWidgetResizable(true);
    scroll->setWidget(widget);
    mScrollLayout->addWidget(scroll);
    QHBoxLayout* hlayout = new QHBoxLayout();
    widget->setLayout(hlayout);


    std::function<void (Tree*)> set = [&](Tree* node)
    {
        if (node->childs.isEmpty())
        {
            putsOnLayout(hlayout, node->data);
        }
        else
        {
            for (Tree* next : node->childs)
                set(next);
        }
    };
    set(mTree);
}



void Dialog::putsOnLayout(QLayout* layout, QTableView* table)
{
    table->setHorizontalScrollMode(QAbstractItemView::ScrollPerPixel);
    table->setVerticalScrollMode(QAbstractItemView::ScrollPerPixel);
    table->setMinimumWidth(400);
    table->setMinimumHeight(100);

    SpinBoxDelegate* delegate = new SpinBoxDelegate(table->model()->rowCount(), this);

    table->setItemDelegate(delegate);
    QLabel* led = createIndicator(":res/pics/indicator_gray.svg", QRect(10, 10, 25, 25), this);
    QLabel* indexCR = new QLabel("-");


    connect(delegate, &SpinBoxDelegate::indicate, [led, indexCR](double val)
    {
        indexCR->setText(QString::number(val,'g', 5));
        if (val > 0.1)
            led->setPixmap(QIcon(":res/pics/indicator_red.svg").pixmap(led->size()));
        else
            led->setPixmap(QIcon(":res/pics/indicator_green.svg").pixmap(led->size()));
    });


    QLabel* title = new QLabel(table->model()->objectName());
    title->setMinimumHeight(50);

    QFont font(title->fontInfo().family(), 8);
    table->horizontalHeader()->setFont(font);
    table->verticalHeader()->setFont(font);
    font.setPointSize(7);
    title->setFont(font);

    QWidget* info = new QWidget();
    QVBoxLayout* vbl = new QVBoxLayout();
    vbl->setContentsMargins(0, 0, 0, info->height() * 0.25);
    vbl->addWidget(title);
    vbl->addWidget(led);
    vbl->addWidget(indexCR);
    info->setLayout(vbl);

    layout->addWidget(info);
    layout->addWidget(table);
}



void Dialog::setDefaultValues(TreeNode<QTableView*>* node)
{
    QTableView* table = node->data;
    SpinBoxDelegate* delegate = qobject_cast<SpinBoxDelegate*>(table->itemDelegate());
    QAbstractItemModel* model = table->model();

    for (int row = 0; row < model->rowCount(); row++)
    {
        const QModelIndex index = model->index(row, row);
        model->setData(index, 1);
        delegate->lockIndex(index);
        table->horizontalHeader()->resizeSection(row, 70); //TODO размеры хедеров
        table->verticalHeader()->resizeSection(row, 25);
    }

    for (int i = 0; i < node->childs.size(); i++)
        setDefaultValues(node->childs[i]);
}



QLabel* Dialog::createIndicator(const QString& file, const QRect& rect, QWidget* parent) const
{
    QLabel* label = new QLabel(parent);
    label->setGeometry(rect);
    label->setFixedSize(rect.size());
    label->setAlignment(Qt::AlignCenter);
    QPixmap pix = QIcon(file).pixmap(label->size());
    label->setPixmap(pix);
    return label;
}



void Dialog::calculate()
{
    qDebug() << "clicked CALCULATE";

    std::function<TreeNode<Matrix>* (Tree*)> fill;
    fill = [&fill](Tree* node)
    {
        QTableView* table = node->data;
        QAbstractItemModel* model = table->model();

        int rows = model->rowCount();
        int cols = model->columnCount();
        Matrix mx(rows, cols);

        for (int row = 0; row < rows; row++)
        {
            for (int col = 0; col < cols; col++)
            {
                const QModelIndex index = model->index(row, col);
                mx(row, col) = model->data(index).toDouble();
            }
        }

        TreeNode<Matrix>* newNode = new TreeNode<Matrix>(mx);

        for (int i = 0; i < node->childs.size(); i++)
        {
            newNode->childs.append(fill(node->childs[i]));
        }
        return newNode;
    };

    TreeNode<Matrix>* tree = fill(mTree);
    AlghorithmAHP ahp(tree, mAlternatives.size());

    QPair<int, QVector<double>> pair = ahp.answer();

    QString text;
    text.append("Комбинированные весовые коэффициенты\n");
    for (int i = 0; i < mAlternatives.size(); i++)
        text.append(mAlternatives[i] + "\t" + QString::number(pair.second[i]) + "\n");

    text.append("\nЛучший вариант: \"" + mAlternatives[pair.first] + "\" \t" +
            QString::number(pair.second[pair.first]));

    QMessageBox::information(this, "Ответ", text, QMessageBox::Ok);
    delete tree;
}
