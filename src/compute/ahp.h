#ifndef HIERARCHY_ALGORITHM_H_INCLUDED
#define HIERARCHY_ALGORITHM_H_INCLUDED

#include <QVector>
#include <initializer_list>


class Matrix;



template<typename T>
struct TreeNode
{
    T data;
    QList<TreeNode<T>*> childs;

    explicit TreeNode(const T& value = T()) : data(value)
    {}

    ~TreeNode()
    {
        qDeleteAll(childs);
    }
};




/*!
  \class ahp::AlghorithmAHP
  \brief Метод анализа иерархий
  \property levels               кол-во уровней иерархии отчет с 0
*/
class AlghorithmAHP
{
private:
    uint _alternatives;
    TreeNode<double>* mConsistencyRatio;
    TreeNode<double>* mTreeWeights;


public:
    AlghorithmAHP(const TreeNode<Matrix>* tree, uint alternatives);
    ~AlghorithmAHP();

    uint alternatives() const { return _alternatives; }

    static double getCR(const Matrix& m);  //AHP Consistency Ratio

    QPair<uint, QVector<double>> answer();


private:
    QVector<double> weightForEachAlternative();
    static double combinedWeighting(const TreeNode<double>* weights, uint alt);
    static double calcConsistencyRatio(const Matrix& m, const QVector<double>& weights);
};





class Matrix
{
private:
    const uint mRows;
    const uint mCols;
    QVector<double> mtx;

public:
    Matrix(const Matrix&) = default;
    Matrix& operator=(const Matrix&) = default;


    Matrix(uint rows, uint cols);
    Matrix(const std::initializer_list<std::initializer_list<double>>& list);


    uint rows() const { return mRows; }
    uint cols() const { return mCols; }

    const double& operator()(uint x, uint y) const;
    double& operator()(uint x, uint y);

    Matrix normalize() const;
    QVector<double> avrRows() const;

    QVector<double> operator*(const QVector<double>& vec) const;
};




#endif // HIERARCHY_ALGORITHM_H_INCLUDED
