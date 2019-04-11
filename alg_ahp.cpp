#include <cassert>
#include <stdexcept>

#include "alg_ahp.h"



AlghorithmAHP::AlghorithmAHP(uint alternatives)
{
	this->levels = 0;
	this->_alternatives = alternatives;
}



void AlghorithmAHP::addLevel(const std::vector<Matrix>& mtxs)
{
	std::vector<Matrix> matricesOnLevel;
	weights.resize(levels + 1);

	for (uint i = 0; i < mtxs.size(); i++)
	{
		Matrix m = mtxs[i];
		matricesOnLevel.emplace_back(m.normalize());
		weights[levels].emplace_back(m.avrRows());
	}

	list.emplace_back(matricesOnLevel);
	levels++;
}



// level с нуля
bool AlghorithmAHP::addMatrix(uint onLevel, const Matrix& mtxs)
{
	if (onLevel < levels)
	{
		Matrix m = mtxs;
		list[onLevel].emplace_back(m.normalize());
		weights[onLevel].emplace_back(m.avrRows());
		return true;
	}
	return false;
}



std::pair<int, std::vector<double>> AlghorithmAHP::answer()
{
	std::vector<double> result = weightForEachAlternative();
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



std::vector<double> AlghorithmAHP::weightForEachAlternative()
{
	std::vector<double> result;

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
	mtx.resize(rows * cols);
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



Matrix& Matrix::normalize()
{
	for (uint col = 0; col < cols; col++)
	{
		double sum = 0.0;
		for (uint row = 0; row < rows; row++)
			sum += (*this)(row, col);

		for (uint row = 0; row < rows; row++)
			(*this)(row, col) /= sum;
	}
	return *this;
}



std::vector<double> Matrix::avrRows() const
{
	std::vector<double> vec(rows);
	for (uint row = 0; row < rows; row++)
	{
		double sum = 0.0;
		for (uint col = 0; col < cols; col++)
			sum += (*this)(row, col);
		vec[row] = sum / cols;
	}
	return vec;
}
