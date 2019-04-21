#include <iostream>
#include <iomanip>
#include <vector>
#include <numeric>
#include <QDebug>
#include "alg_ahp.h"


using std::cout, std::endl;
using ahp::AlghorithmAHP, ahp::Matrix;

void test1();
void test2();
void test3();



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


int Mmain()
{




    return 0;
}


