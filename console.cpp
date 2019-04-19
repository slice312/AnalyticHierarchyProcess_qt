#include <iostream>
#include <iomanip>
#include <vector>
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


int maint()
{
    //    Matrix m = {
    //        {1, 2},
    //        {0.5, 1}
    //    };

    //    std::vector<double> avr1 = m.normalize().avrRows();
    //    std::vector<double> nvl = m * avr1;

    //    double nMax = std::accumulate(nvl.begin(), nvl.end(), 0.0);

    //    double CI = (nMax - nvl.size()) / (nvl.size() - 1);
    //    double RI = (1.98 * (nvl.size() - 2)) / nvl.size();
    //    double CR = CI / RI;


    //    cout << "\n\n";

    //    Matrix m2 = {
    //        {1, 0.5, 0.2},
    //        {2,  1, 0.5},
    //        {5,  2, 1}
    //    };

    //    Matrix normalized = m2.normalize();
    //    std::vector<double> avr = normalized.avrRows();
    //    std::vector<double> nVal = m2 * avr;

    //    nMax = std::accumulate(nVal.begin(), nVal.end(), 0.0);

    //    CI = (nMax - nVal.size()) / (nVal.size() - 1);
    //    RI = (1.98 * (nVal.size() - 2)) / nVal.size();
    //    CR = CI / RI;





//    test1();
    //    test2();
        test3();


    return 0;
}


//задача 14_1_2_3
void test1()
{
    AlghorithmAHP ahp(2);

    std::vector<int> ves;

    Matrix mx = {
        { 1, 1, 0.25 },
        { 1, 1,  0.2 },
        { 4, 5,    1 }
    };

    auto CRs = ahp.addLevel({mx});


    Matrix lvl2_1 = {
        {   1, 2 },
        { 0.5, 1 }
    };


    Matrix lvl2_2 = {
        { 1, 0.5 },
        { 2,   1 }
    };

    Matrix lvl2_3 = {
        { 1, 1 },
        { 1, 1 }
    };


    ahp.addLevel({lvl2_1, lvl2_2, lvl2_3});

    auto pair = ahp.answer();
    for (double wght : pair.second)
    {
        cout << wght << endl;
    }
    cout << "INDEX = " << pair.first << endl;
    cout << CRs[0] << endl;

    // правильный ответ index = 0, [0.501916; 0,498084]
    // CR = 0.0066
}



//задача 14_1_2_2
void test2()
{
    AlghorithmAHP ahp(3);

    //----------------------level 1--------------------------
    Matrix mx = {
        {   1, 2 },
        { 0.5, 1 }
    };
    ahp.addLevel({mx});


    //----------------------level 2--------------------------

    Matrix lvl2_1 = {
        { 1, 0.333 },
        { 3,     1 }
    };

    Matrix lvl2_2 = {
        {    1, 4 },
        { 0.25, 1 }
    };

    ahp.addLevel({lvl2_1, lvl2_2});

    //----------------------level 3--------------------------

    Matrix lvl3_1 = {
        {     1,   2, 3 },
        {   0.5,   1, 2 },
        { 0.333, 0.5, 1 }
    };

    Matrix lvl3_2 = {
        {   1, 2,   0.5 },
        { 0.5, 1, 0.333 },
        {   2, 3,     1 }
    };

    Matrix lvl3_3 = {
        {    1,     4, 2 },
        { 0.25,     1, 3 },
        {  0.5, 0.333, 1 }
    };

    Matrix lvl3_4 = {
        {    1,   0.5, 4 },
        {    2,     1, 3 },
        { 0.25, 0.333, 1 }
    };

    ahp.addLevel({lvl3_1, lvl3_2, lvl3_3, lvl3_4});


    auto pair = ahp.answer();
    for (double wght : pair.second)
    {
        cout << wght << endl;
    }
    cout << "INDEX = " << pair.first << endl;
}


//задача 14_1_1  Выбор универа Мартин и Джейн
void test3()
{
    AlghorithmAHP ahp(3);

    //----------------------level 1--------------------------
    Matrix mx = {
        { 1, 1 },
        { 1, 1 }
    };
    ahp.addLevel({mx});


    //----------------------level 2--------------------------

    Matrix lvl2_1 = {
        { 1, 0.2 },
        { 5,   1 }
    };

    Matrix lvl2_2 = {
        {    1, 0.42 },
        { 2.33,    1 },
    };

    ahp.addLevel({lvl2_1, lvl2_2});

    //----------------------level 3--------------------------

    Matrix lvl3_1 = {
        { 1, .5, .2 },
        { 2,  1, .5 },
        { 5,  2,  1 }
    };

    Matrix lvl3_2 = {
        {    1,    2,   3 },
        {   .5,    1, 1.5 },
        { .333, .666,   1 }
    };

    Matrix lvl3_3 = {
        {    1,   .66, .4 },
        {  1.5,     1, .6 },
        {  2.5,  1.66,  1 }
    };

    Matrix lvl3_4 = {
        {  1, 2.5, 1.66 },
        { .4,   1,  .66 },
        { .6, 1.5,    1 }
    };


    //    Matrix lvl3_5 = {
    //        {  1,  .2, 1.66 },
    //        {  5,   1, .333 },
    //        { .6,   3,    1 }
    //    };

    std::vector<double>  CR =  ahp.addLevel({lvl3_1, lvl3_2, lvl3_3, lvl3_4});

    for (double coef : CR) {
        cout << "coef: " << coef << endl;
    }


    auto pair = ahp.answer();
    for (double wght : pair.second)
    {
        cout << wght << endl;
    }
    cout << "INDEX = " << pair.first << endl;

    // правильный ответ index = 0, [0.44214, 0.25184, 0.30602]
}
