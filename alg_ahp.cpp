#include <cassert>
#include <stdexcept>
#include <numeric>
#include <cmath>
#include "alg_ahp.h"


using namespace ahp;



AlghorithmAHP::AlghorithmAHP(uint alternatives)
{
    this->levels = 0;
    this->_alternatives = alternatives;
}




pair<int, vector<double>> AlghorithmAHP::answer()
{
    vector<double> result = weightForEachAlternative();
    double max = 0.0;
    int index = -1;

    for (uint i = 0; i < result.size(); i++)
    {
        if (result[i] > max)
        {
            max = result[i];
            index = i;
        }
    }
    return std::make_pair(index, result);
}



double AlghorithmAHP::getCR(const Matrix& m)
{
    Matrix normalized = m.normalize();
    vector<double> avr = normalized.avrRows();
    return calcConsistencyRatio(m, avr);
}



void AlghorithmAHP::setTree(TreeNode<Matrix>* tree)
{
    std::function<TreeNode<double>* (TreeNode<Matrix>*, TreeNode<double>*)> clone;
    clone = [&clone](TreeNode<Matrix>* mxs, TreeNode<double>* weights)
    {
        Matrix normalized = mxs->data.normalize();
        vector<double> avr = normalized.avrRows();


        double CR = calcConsistencyRatio(mxs->data, avr);
        qDebug() << CR;
        TreeNode<double>* cr = new TreeNode(CR);

        for (double num : avr)
        {
            weights->childs.append(new TreeNode(num));
        }

        for (int i = 0; i < mxs->childs.size(); i++)
        {
            cr->childs.append(clone(mxs->childs[i], weights->childs[i]));
        }

        return cr;
    };

    mTreeMatrix = tree;
    mTreeWeights = new TreeNode(1.0);
    mConsistencyRatio = clone(mTreeMatrix, mTreeWeights);

    std::function<double (TreeNode<double>*, int)> alg;
    alg = [&alg](TreeNode<double>* root, int alt)
    {
        //остановить не предпоследнем уровне
        if (root->childs[0]->childs.size() == 0)
            return root->childs[alt]->data;

        double sum = 0.0;
        for (int i = 0; i < root->childs.size(); i++)
        {
            sum += root->childs[i]->data * alg(root->childs[i], alt);
        }
        return sum;
    };

    double a1 = alg(mTreeWeights, 0);
    double a2 = alg(mTreeWeights, 1);
    double a3 = alg(mTreeWeights, 2);

    Q_UNUSED(a1);
    Q_UNUSED(a2);
    Q_UNUSED(a3);
    qDebug() << "_------------------";
}



//--------------------------------PRIVATE------------------------------------
vector<double> AlghorithmAHP::weightForEachAlternative()
{
    vector<double> result;
    for (uint i = 0; i < _alternatives; i++)
    {
        double res = combinedWeighting(mTreeWeights, i);
        result.push_back(res);
    }
    return result;
}



double AlghorithmAHP::combinedWeighting(TreeNode<double>* weights, uint alt)
{
    //остановить не предпоследнем уровне
    if (weights->childs[0]->childs.size() == 0)
        return weights->childs[alt]->data;

    double sum = 0.0;
    for (uint i = 0; i < weights->childs.size(); i++)
    {
        sum += weights->childs[i]->data * combinedWeighting(weights->childs[i], alt);
    }
    return sum;
}



/*!
 * \brief AlghorithmAHP::calcConsistencyRatio
 * \param m                       ненормализованная матрица
 * \param avrRows                 относительные весовые коэффициенты
 * \return индекс согласованности CR
 */
double AlghorithmAHP::calcConsistencyRatio(const Matrix& m, const vector<double>& weights)
{
    std::vector<double> vec = m * weights;
    double nmax = std::accumulate(vec.begin(), vec.end(), 0.0);
    int n = vec.size();

    double CI = (nmax - n) / (n - 1);
    double RI = (1.98 * (n - 2)) / n;
    double CR = CI / RI;

    if (std::isinf(CR) || std::isnan(CR))
        CR = 0;
    return CR;
}





//--------------------------------------------MATRIX-----------------------------------------
Matrix::Matrix(uint rows, uint cols) : rows(rows), cols(cols)
{
    this->mtx.resize(rows * cols);
}



Matrix::Matrix(initializer_list<initializer_list<double>> list) :
    rows(list.size()), cols(list.begin()->size())
{
    this->mtx.resize(rows * cols);
    uint i = 0;
    for (auto it1 = list.begin(); it1 != list.end(); ++it1, ++i)
    {
        uint j = 0;
        for (auto it2 = it1->begin(); it2 != it1->end(); ++it2, ++j)
        {
            (*this)(i, j) = *it2;
        }
    }
}



const double& Matrix::operator()(uint x, uint y) const
{
    if (x >= rows || y >= cols)
        throw std::out_of_range("matrix indices out of range");
    return mtx[x * cols + y];
}



double& Matrix::operator()(uint x, uint y)
{
    if (x >= rows || y >= cols)
        throw std::out_of_range("matrix indices out of range");
    return mtx[x * cols + y];
}



Matrix Matrix::normalize() const
{
    Matrix copy(*this);
    for (uint col = 0; col < cols; col++)
    {
        double sum = 0.0;
        for (uint row = 0; row < rows; row++)
            sum += copy(row, col);

        for (uint row = 0; row < rows; row++)
            copy(row, col) /= sum;
    }
    return copy;
}



vector<double> Matrix::avrRows() const
{
    vector<double> vec(rows);
    for (uint row = 0; row < rows; row++)
    {
        double sum = 0.0;
        for (uint col = 0; col < cols; col++)
            sum += (*this)(row, col);
        vec[row] = sum / cols;
    }
    return vec;
}



vector<double> Matrix::operator*(const vector<double>& vec) const
{
    if (cols != vec.size())
        throw std::invalid_argument("matrix columns not equals vector size");

    vector<double> result(vec.size());

    for (uint i = 0; i < vec.size(); i++)
    {
        for (uint j = 0; j < cols ; j++)
        {
            result[i] += (*this)(i, j) * vec[j];
        }
    }
    return result;
}
