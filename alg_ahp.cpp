#include <cassert>
#include <stdexcept>
#include <numeric>
#include "alg_ahp.h"
#include <QDebug>

using namespace ahp;



AlghorithmAHP::AlghorithmAHP(uint alternatives)
{
    this->levels = 0;
    this->_alternatives = alternatives;
}


/*!
    \property AlghorithmAHP::addLevel
    \brief Добавляет уровень иерархии с матрицами.
        При добавлении нормализует матрицы, высчитывает весовые коэф. и
        коэф. согласованности.
    \return возвращает коэффициенты согласованности для каждой добавленной матрицы

    \sa addMatrix()
*/
vector<double> AlghorithmAHP::addLevel(const vector<Matrix>& mtxs)
{
    weights.resize(levels + 1);
    CR.resize(levels + 1);
    vector<Matrix> matricesOnLevel;

    for (uint i = 0; i < mtxs.size(); i++)
    {
        Matrix m = mtxs[i];
        Matrix normalized = m.normalize();
        matricesOnLevel.emplace_back(normalized);

        vector<double> avr = normalized.avrRows();
        weights[levels].emplace_back(avr);

        vector<double> nVal = m * avr;
        double nMax = std::accumulate(nVal.begin(), nVal.end(), 0.0);

        double CI = (nMax - nVal.size()) / (nVal.size() - 1);
        double RI = (1.98 * (nVal.size() - 2)) / nVal.size();
        double CR = CI / RI;
        CR = (RI == 0) ? std::numeric_limits<double>::min() : CR;
        this->CR[levels].push_back(CR);
    }

    list.emplace_back(matricesOnLevel);
    return CR[levels++];
}



/*!
 * \brief AlghorithmAHP::addMatrix
 * \param onLevel уровень иерархии, отчет с 0
 * \param mx матрица сравнений
 * \return коэффициент согласованности добавленной матрицы
 */
double AlghorithmAHP::addMatrix(uint onLevel, const Matrix& mx)
{
    if (onLevel < levels)
    {
        Matrix m = mx;
        list[onLevel].emplace_back(m.normalize());

        vector<double> avr = m.avrRows();
        weights[onLevel].emplace_back(avr);

        vector<double> nVal = m * avr;
        double nMax = std::accumulate(nVal.begin(), nVal.end(), 0);


        double CI = (nMax - nVal.size()) / (nVal.size() - 1);
        double RI = (1.98 * (nVal.size() - 2)) / nVal.size();

        double _CR = CI / RI;
        CR[onLevel].push_back(_CR);
        return _CR;
    }
    return INT_MIN;
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
    std::vector<double> avr1 = m.normalize().avrRows();
    std::vector<double> nvl = m * avr1;

    double nMax = std::accumulate(nvl.begin(), nvl.end(), 0.0);


    double CI = (nMax - nvl.size()) / (nvl.size() - 1);
    // numeri_limits::min добавил чтобы не было деления на 0
    double RI = (1.98 * (nvl.size() - 2)) / nvl.size();
    double CR = CI / RI;
    CR = (RI == 0) ? std::numeric_limits<double>::min() : CR;

    return CR;
}



vector<double> AlghorithmAHP::weightForEachAlternative()
{
    vector<double> result;

    for (uint i = 0; i < _alternatives; i++)
    {
        double res = combinedWeighting(0, 0, i);
        assert(res > 0.0 && "wrong weight");
        result.push_back(res);
    }
    return result;
}



double AlghorithmAHP::combinedWeighting(uint lvl, uint onlvl, uint alt)
{
    if (lvl == this->levels - 1)
        return weights[lvl][onlvl][alt];

    for (uint i = lvl; i <= this->levels; i++)
    {
        for (uint vecOnLevel = onlvl; vecOnLevel < list[lvl].size(); vecOnLevel++)
        {
            double sum = 0.0;
            for (uint i = 0; i < weights[lvl][vecOnLevel].size(); i++)
            {
                sum += weights[lvl][vecOnLevel][i] *
                        combinedWeighting(lvl + 1, vecOnLevel * 2 + i, alt);
            }
            return sum;
        }
    }
    return -1.0;
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
            auto num = *it2;
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

    for (int i = 0; i < vec.size(); i++)
    {
        for (int j = 0; j < cols ; j++)
        {
            result[i] += (*this)(i, j) * vec[j];
        }
    }
    return result;
}
