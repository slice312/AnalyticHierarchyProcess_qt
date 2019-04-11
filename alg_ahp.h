#ifndef HIERARCHY_ALGORITHM_H_INCLUDED
#define HIERARCHY_ALGORITHM_H_INCLUDED

#include <vector>

typedef unsigned int uint;

class Matrix;



class AlghorithmAHP
{
private:
	uint levels;
	uint _alternatives;

	std::vector<std::vector<Matrix>> list;
	std::vector<std::vector<std::vector<double>>> weights;


public:
	AlghorithmAHP(uint alternatives);

	int alternatives() const { return _alternatives; }

	void addLevel(const std::vector<Matrix>& mtxs);
	bool addMatrix(uint onLevel, const Matrix& mtxs);

	std::pair<int, std::vector<double>> answer();


private:
	std::vector<double> weightForEachAlternative();
	double combinedWeighting(uint level, uint onlvl, uint alt);
};





class Matrix
{
private:
	const uint rows;
	const uint cols;
	std::vector<double> mtx;

public:
	Matrix(uint rows, uint cols);


	uint size1() const { return rows; }
	uint size2() const { return cols; }

	const double& operator()(uint x, uint y) const;
	double& operator()(uint x, uint y);

	Matrix& normalize();
	std::vector<double> avrRows() const;
};


#endif // HIERARCHY_ALGORITHM_H_INCLUDED
