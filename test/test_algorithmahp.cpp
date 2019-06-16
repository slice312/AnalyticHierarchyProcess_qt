#include <QtTest>
#include <memory>
#include "../main/src/compute/ahp.h"

// add necessary includes here


class AlgorithmAHP_test : public QObject
{
    Q_OBJECT

public:
    AlgorithmAHP_test() = default;

private slots:
    void taskMartinAndUniversity();
    void task_14_1_2_3();

};


void AlgorithmAHP_test::taskMartinAndUniversity()
{
    uint alternatives = 3;

    Matrix m1_level1 = {
        {1, 1},
        {1, 1}
    };

    Matrix m1_level2 = {
        {1, 0.2},
        {5,   1}
    };

    Matrix m2_level2 = {
        {   1, 0.42},
        {2.33,    1}
    };


    Matrix m1_level3 = {
        {1, 0.5, 0.2},
        {2,   1, 0.5},
        {5,   2,   1}
    };


    Matrix m2_level3 = {
        {   1,      2,   3},
        {  0.5,     1, 1.5},
        {0.333, 0.666,   1}
    };

    Matrix m3_level3 = {
        {  1, 0.66, 0.4},
        {1.5,    1, 0.6},
        {2.5, 1.66,   1}
    };

    Matrix m4_level3 = {
        {  1, 2.5, 1.66},
        {0.4,   1, 0.66},
        {0.6, 1.5,    1}
    };

    std::unique_ptr<TreeNode<Matrix>> tree(new TreeNode<Matrix>(m1_level1));
    tree->childs.append({new TreeNode<Matrix>(m1_level2), new TreeNode<Matrix>(m2_level2)});
    tree->childs[0]->childs.append({new TreeNode<Matrix>(m1_level3), new TreeNode<Matrix>(m2_level3)});
    tree->childs[1]->childs.append({new TreeNode<Matrix>(m3_level3), new TreeNode<Matrix>(m4_level3)});

    AlgorithmAHP ahp(tree.get(), alternatives);
    QVector<double> vec = ahp.answer().second;

    vec[0] = static_cast<double>(static_cast<int>(vec[0] * 1000 + 0.5)) / 1000.0;
    vec[1] = static_cast<double>(static_cast<int>(vec[1] * 1000 + 0.5)) / 1000.0;
    vec[2] = static_cast<double>(static_cast<int>(vec[2] * 1000 + 0.5)) / 1000.0;

    QCOMPARE(vec[0], 0.443);
    QCOMPARE(vec[1], 0.252);
    QCOMPARE(vec[2], 0.305);
}


void AlgorithmAHP_test::task_14_1_2_3()
{
    Matrix m1_level1 = {
        {1, 1, 0.25},
        {1, 1,  0.2},
        {4, 5,    1}
    };

    Matrix m1_level2 = {
        {  1, 2},
        {0.5, 1}
    };

    Matrix m2_level2 = {
        {1, 0.5},
        {2,   1}
    };

    Matrix m3_level2 = {
        {1, 1},
        {1, 1}
    };

    std::unique_ptr<TreeNode<Matrix>> tree(new TreeNode<Matrix>(m1_level1));
    tree->childs.append({new TreeNode<Matrix>(m1_level2), new TreeNode<Matrix>(m2_level2),
                        new TreeNode<Matrix>(m3_level2)});

    AlgorithmAHP ahp(tree.get(), 2);
    double ratio = AlgorithmAHP::getCR(m1_level1);
    QVector<double> vec = ahp.answer().second;

    vec[0] = static_cast<double>(static_cast<int>(vec[0] * 1000 + 0.5)) / 1000.0;
    vec[1] = static_cast<double>(static_cast<int>(vec[1] * 1000 + 0.5)) / 1000.0;
    ratio = static_cast<double>(static_cast<int>(ratio * 10000 + 0.5)) / 10000.0;


    QCOMPARE(vec[0], 0.502);
    QCOMPARE(vec[1], 0.498);
    QCOMPARE(ratio, 0.0066);
}



QTEST_APPLESS_MAIN(AlgorithmAHP_test)
#include "test_algorithmahp.moc"
