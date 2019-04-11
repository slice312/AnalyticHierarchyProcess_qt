#include <iostream>
#include <vector>
#include <iomanip>
#include <sstream>
#include <ostream>

#include "alg_ahp.h"


using std::cout, std::endl;

void test1();


Matrix& fill(Matrix& m, std::istream& istream)
{
	for (uint row = 0; row < m.size1(); row++)
		for (uint col = 0; col < m.size2(); col++)
			istream >> m(row, col);
	return m;
}


std::ostream& operator<<(std::ostream& os, const Matrix& m)
{
	for (uint i = 0; i < m.size1(); i++)
	{
		for (uint j = 0; j < m.size2(); j++)
			std::cout << std::setw(3) << m(i, j) << "  ";
		std::cout << std::endl;
	}
	return os;
}


int maint()
{
	test1();


	return 0;
}


//задача 14_1_2_3
void test1()
{
	std::istringstream stream;
	AlghorithmAHP ahp(2);

	Matrix mx(3, 3);
	std::string str = "1      1     0.25 "
		"1      1      0.2 "
		"4      5        1 ";
	stream.str(str);
	ahp.addLevel({ fill(mx, stream) });


	Matrix lvl2_1(2, 2);
	str = "1      2 "
		"0.5    1 ";
	stream.str(str);
	fill(lvl2_1, stream);


	Matrix lvl2_2(2, 2);
	str = "1       0.5 "
		"2     1 ";
	stream.str(str);
	fill(lvl2_2, stream);


	Matrix lvl2_3(2, 2);
	str = "1       1 "
		"1     1 ";
	stream.str(str);
	fill(lvl2_3, stream);


	std::vector<Matrix> vec;
	
	vec.push_back(lvl2_1);
	vec.push_back(lvl2_2);
	vec.push_back(lvl2_3);

	ahp.addLevel(vec);

	auto pair = ahp.answer();
	for (double wght : pair.second)
	{
		cout << wght << endl;
	}
	cout << "INDEX = " << pair.first << endl;

	// правильный ответ index = 0, [0.501916; 0,498084]
}
