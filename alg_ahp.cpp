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


/*!
    \brief AlghorithmAHP::addLevel
            Добавляет уровень иерархии с матрицами.
            При добавлении нормализует матрицы, высчитывает весовые коэф. и
            коэф. согласованности.
    \return коэффициенты согласованности для каждой добавленной матрицы
*/
vector<double> AlghorithmAHP::addLevel(const vector<Matrix>& mtxs)
{
    weights.resize(levels + 1);
    CR.resize(levels + 1);
    vector<Matrix> matricesOnLevel;

    for (uint i = 0; i < mtxs.size(); i++)
    {
        Matrix normalized = mtxs[i].normalize();
        matricesOnLevel.push_back(normalized);

        vector<double> avr = normalized.avrRows();
        weights[levels].push_back(avr);

        double CR = calcConsistencyRatio(mtxs[i], avr);
        this->CR[levels].push_back(CR);
    }

    list.push_back(matricesOnLevel);
    return CR[levels++];
}



/*!
 * \brief AlghorithmAHP::addMatrix
 * \param onLevel       уровень иерархии, отчет с 0
 * \param m             матрица сравнений
 * \return коэффициент согласованности добавленной матрицы
 */
double AlghorithmAHP::addMatrix(uint onLevel, const Matrix& m)
{
    if (onLevel >= levels || onLevel < 0)
        throw std::invalid_argument("invalid level");

    Matrix normalized = m.normalize();
    list[onLevel].push_back(normalized);

    vector<double> avr = normalized.avrRows();
    weights[onLevel].push_back(avr);

    double CR = calcConsistencyRatio(m, avr);
    this->CR[onLevel].push_back(CR);
    return CR;
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



//--------------------------------PRIVATE------------------------------------
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



/*!
 * \brief AlghorithmAHP::calcConsRatio
 * \param m                       ненормализованная матрица
 * \param avrRows                 относительные весовые коэффициенты
 * \return индекс согласованности CR
 */
double AlghorithmAHP::calcConsistencyRatio(const Matrix& m, const vector<double>& weights)
{
    std::vector<double> vec = m * weights;
    double max = std::accumulate(vec.begin(), vec.end(), 0.0);
    int n = vec.size();

    double CI = (max - n) / (n - 1);
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
