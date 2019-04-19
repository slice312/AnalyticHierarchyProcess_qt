#ifndef HIERARCHY_ALGORITHM_H_INCLUDED
#define HIERARCHY_ALGORITHM_H_INCLUDED

#include <vector>
#include <initializer_list>

typedef unsigned int uint;


namespace ahp {

using std::vector,
std::pair,
std::initializer_list;

class AlghorithmAHP;
class Matrix;
}

using ahp::AlghorithmAHP;
using ahp::Matrix;



/*!
  \class ahp::AlghorithmAHP
  \brief Метод анализа иерархий
  \property levels отчет с 0
*/
class ahp::AlghorithmAHP
{
private:
    uint levels;
    uint _alternatives;

    vector<vector<Matrix>> list;             // матрицы на уровне иерархии
    vector<vector<vector<double>>> weights;  // весовые коэффициенты
    vector<vector<double>> CR;               // коэффициенты согласованности


public:
    AlghorithmAHP(uint alternatives);

    int alternatives() const { return _alternatives; }

    vector<double> addLevel(const vector<Matrix>& mtxs);
    double addMatrix(uint onLevel, const Matrix& mx);

    pair<int, vector<double>> answer();


private:
    vector<double> weightForEachAlternative();
    double combinedWeighting(uint level, uint onlvl, uint alt);
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


#endif // HIERARCHY_ALGORITHM_H_INCLUDED
