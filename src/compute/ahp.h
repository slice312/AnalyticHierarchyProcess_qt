#ifndef AHP_ALGORITHM_H_INCLUDED
#define AHP_ALGORITHM_H_INCLUDED

#include <QVector>
#include <QDataStream>

#include <initializer_list>



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

    TreeNode(const TreeNode<T>&) = delete;
    TreeNode& operator=(const TreeNode<T>&) = delete;

    friend QDataStream& operator<<(QDataStream& out, const TreeNode<T>& obj)
    {
        out << obj.data << obj.childs.size();

        for (auto ptr : obj.childs)
            operator<<(out, *ptr);
        return out;
    }


    friend QDataStream& operator>>(QDataStream& in, TreeNode<T>& obj)
    {
        int count;
        in >> obj.data >> count;

        for (int i = 0; i < count; ++i)
        {
            TreeNode<T>* node = new TreeNode<T>();
            operator>>(in, *node);
            obj.childs.append(node);
        }
        return in;
    }
};



class Matrix;


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

#endif // AHP_ALGORITHM_H_INCLUDED
