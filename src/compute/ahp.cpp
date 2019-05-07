#include <stdexcept>
#include <cmath>
#include <functional>
#include <QDebug>
#include "ahp.h"





AlghorithmAHP::AlghorithmAHP(const TreeNode<Matrix>* tree, uint alternatives)
{
    this->_alternatives = alternatives;


    std::function<TreeNode<double>* (const TreeNode<Matrix>*, TreeNode<double>*)> calc;
    calc = [&calc](const TreeNode<Matrix>* mxs, TreeNode<double>* weights)
    {
        Matrix normalized = mxs->data.normalize();
        QVector<double> avr = normalized.avrRows();

        for (double num : avr)
            weights->childs.append(new TreeNode(num));

        double CR = calcConsistencyRatio(mxs->data, avr);
        TreeNode<double>* nodeCR = new TreeNode(CR);

        for (uint i = 0; i < mxs->childs.size(); i++)
            nodeCR->childs.append(calc(mxs->childs[i], weights->childs[i]));

        return nodeCR;
    };

    mTreeWeights = new TreeNode(1.0);
    mConsistencyRatio = calc(tree, mTreeWeights);
}



AlghorithmAHP::~AlghorithmAHP()
{
    delete mConsistencyRatio;
    delete mTreeWeights;
}



QPair<uint, QVector<double>> AlghorithmAHP::answer()
{
    QVector<double> result = weightForEachAlternative();
    double max = 0.0;
    uint index = -1;


    for (uint i = 0; i < result.size(); i++)
    {
        if (result[i] > max)
        {
            max = result[i];
            index = i;
        }
    }
    return qMakePair(index, result);
}



double AlghorithmAHP::getCR(const Matrix& m)
{
    Matrix normalized = m.normalize();
    QVector<double> avr = normalized.avrRows();
    return calcConsistencyRatio(m, avr);
}





//--------------------------------PRIVATE------------------------------------
QVector<double> AlghorithmAHP::weightForEachAlternative()
{
    QVector<double> result;
    for (uint i = 0; i < _alternatives; i++)
        result.push_back(combinedWeighting(mTreeWeights, i));
    return result;
}



double AlghorithmAHP::combinedWeighting(const TreeNode<double>* weights, uint alt)
{
    //остановить не предпоследнем уровне
    if (weights->childs[0]->childs.isEmpty())
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
double AlghorithmAHP::calcConsistencyRatio(const Matrix& m, const QVector<double>& weights)
{
    QVector<double> vec = m * weights;
    double nmax = std::accumulate(vec.begin(), vec.end(), 0.0);
    uint n = vec.size();

    double CI = (nmax - n) / (n - 1);
    double RI = (1.98 * (n - 2)) / n;
    double CR = CI / RI;

    if (std::isinf(CR) || std::isnan(CR))
        CR = 0;
    return CR;
}





//--------------------------------------------MATRIX-----------------------------------------
Matrix::Matrix(uint rows, uint cols) : mRows(rows), mCols(cols)
{
    this->mtx.resize(rows * cols);
}



Matrix::Matrix(const std::initializer_list<std::initializer_list<double>>& list) :
    mRows(list.size()), mCols(list.begin()->size())
{
    this->mtx.resize(mRows * mCols);
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
    if (x >= mRows || y >= mCols)
        throw std::out_of_range("matrix indices out of range");
    return mtx[x * mCols + y];
}



double& Matrix::operator()(uint x, uint y)
{
    if (x >= mRows || y >= mCols)
        throw std::out_of_range("matrix indices out of range");
    return mtx[x * mCols + y];
}



Matrix Matrix::normalize() const
{
    Matrix copy(*this);
    for (uint col = 0; col < mCols; col++)
    {
        double sum = 0.0;
        for (uint row = 0; row < mRows; row++)
            sum += copy(row, col);

        for (uint row = 0; row < mRows; row++)
            copy(row, col) /= sum;
    }
    return copy;
}



QVector<double> Matrix::avrRows() const
{
    QVector<double> vec(mRows);
    for (uint row = 0; row < mRows; row++)
    {
        double sum = 0.0;
        for (uint col = 0; col < mCols; col++)
            sum += (*this)(row, col);
        vec[row] = sum / mCols;
    }
    return vec;
}



QVector<double> Matrix::operator*(const QVector<double>& vec) const
{
    if (mCols != vec.size())
        throw std::invalid_argument("matrix columns not equals vector size");

    QVector<double> result(vec.size());

    for (uint i = 0; i < vec.size(); i++)
    {
        for (uint j = 0; j < mCols ; j++)
        {
            result[i] += (*this)(i, j) * vec[j];
        }
    }
    return result;
}
