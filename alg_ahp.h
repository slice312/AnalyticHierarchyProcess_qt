#ifndef HIERARCHY_ALGORITHM_H_INCLUDED
#define HIERARCHY_ALGORITHM_H_INCLUDED

#include <vector>
#include <initializer_list>
#include <QDebug>
#include <functional>


typedef unsigned int uint;


namespace ahp {

using std::vector,
std::pair,
std::initializer_list;

class AlghorithmAHP;
class Matrix;

template<typename T>
struct TreeNode;
}

using ahp::AlghorithmAHP;
using ahp::Matrix;
using ahp::TreeNode;



/*!
  \class ahp::AlghorithmAHP
  \brief Метод анализа иерархий
  \property levels               кол-во уровней иерархии отчет с 0
*/
class ahp::AlghorithmAHP
{
private:
    uint levels;
    uint _alternatives;


    TreeNode<Matrix>* mTreeMatrix;
    TreeNode<double>* mConsistencyRatio;
    TreeNode<double>* mTreeWeights;



public:
    AlghorithmAHP(uint alternatives);

    int alternatives() const { return _alternatives; }


    pair<int, vector<double>> answer();
    static double getCR(const Matrix& m);  //AHP Consistency Ratio
    void setTree(TreeNode<Matrix>* tree);


//    double alg(Tree* root, int alt)
//    {
//        //остановить не предпоследнем уровне
//        if (root->childs[0]->childs.size() == 0)
//            return root->childs[alt]->value;


//        double sum = 0.0;
//        for (int i = 0; i < root->childs.size(); i++)
//        {
//            sum += root->childs[i]->value * alg(root->childs[i], alt);
//        }
//        return sum;
//    }


    /*
     * child* addChild(treenode*)
     *
     */




private:
    vector<double> weightForEachAlternative();
    double combinedWeighting(TreeNode<double>* weights, uint alt);
    static double calcConsistencyRatio(const Matrix& m, const vector<double>& weights);
};





class ahp::Matrix
{
private:
    const uint rows;
    const uint cols;
    vector<double> mtx;

public:
    Matrix(const Matrix&) = default;
    Matrix& operator=(const Matrix&) = default;


    Matrix(uint rows, uint cols);
    Matrix(initializer_list<initializer_list<double>> list);


    uint size1() const { return rows; }
    uint size2() const { return cols; }

    const double& operator()(uint x, uint y) const;
    double& operator()(uint x, uint y);

    Matrix normalize() const;
    vector<double> avrRows() const;

    vector<double> operator*(const vector<double>& vec) const;
};


template<typename T>
struct is_pointer { static const bool value = false; };

template<typename T>
struct is_pointer<T*> { static const bool value = true; };


template<typename T>
struct ahp::TreeNode
{
    T data;
    QList<TreeNode<T>*> childs;

    TreeNode(T val = T()) : data(val)
    {}

    ~TreeNode()
    {
        qDeleteAll(childs);
    }

//    static constexpr void print(TreeNode* node, const QString& prefix)
//    {
//        if (is_pointer<T>::value)
//            qDebug() << prefix <<  node->data->objectName();
//        else
//            qDebug() << prefix <<  node->data.objectName();

//        for (auto i : node->childs)
//        {
//            print(i, prefix + "   ");
//        }
//    }
};


#endif // HIERARCHY_ALGORITHM_H_INCLUDED
