#include <QCoreApplication>
#include <QDebug>
#include <QGenericMatrix>
#include <QList>
#include <QtAlgorithms>

#include <functional>
#include <iostream>
#include <sstream>
#include <string>
#include <iomanip>
#include <algorithm>

#include <boost/numeric/ublas/matrix.hpp>
#include <boost/numeric/ublas/io.hpp>

#include "hierarchyalgorithm.h"


template<class T>
using matrix = boost::numeric::ublas::matrix<T>;


void showSample();
void showSolution_14_1_1();
void showSolution_14_1_2_2();


QVector<double> calc(QVector<QVector<QVector<double>>> all, int alternatives)
{
    int lastLevel = all.size() - 1;
    std::function<double (int, int, int)> combinedWeighting = [&](int level, int onlvl, int N) -> double
    {
        for (int i = level; i <= lastLevel; i++)
        {
            for (int vecOnLevel = onlvl; vecOnLevel < all[level].size(); vecOnLevel++)
            {
                if (level == lastLevel)
                    return all[level][vecOnLevel][N];

                double sum = 0.0;
                for (int i = 0; i < all[level][vecOnLevel].size(); i++)
                {
                    sum += all[level][vecOnLevel][i] * combinedWeighting(level + 1, vecOnLevel * 2 + i, N);
                }
                return sum;
            }
        }

        return -0.2;
    };

    QVector<double> result;
    for (int alt = 0; alt < alternatives; ++alt) {
        result.push_back(combinedWeighting(0, 0, alt));
    }
    //    result.push_back(combinedWeighting(0, 0, 2));
    qDebug() << "RESULT " << result;

    double max = *std::max_element(result.begin(), result.end());

    return result;
}









template<typename T>
void normalize(matrix<T>& mtx)
{
    for (uint col = 0; col < mtx.size2(); col++)
    {
        T sum = T();
        for (uint row = 0; row < mtx.size1(); row++)
        {
            sum += mtx(row, col);
        }

        for (uint row = 0; row < mtx.size1(); row++)
        {
            mtx(row, col) /= sum;
        }
    }

}


template<typename T>
void fill(matrix<T>& m, const std::istream& istream)
{
    for (uint row = 0; row < m.size1(); row++)
        for (uint col = 0; col < m.size2(); col++)
            const_cast<std::istream&>(istream) >> m(row, col);
}


template<typename T>
void show(const matrix<T>& m)
{
    for (uint row = 0; row < m.size1(); row++)
    {
        for (uint col = 0; col < m.size2(); col++)
            std::cout << std::setw(3) << m(row, col) << "  ";
        std::cout << '\n';
    }
}




template<typename T>
QVector<T> avrRows(matrix<T>& mtx)
{
    QVector<T> vec((mtx.size1()));
    for (uint row = 0; row < mtx.size1(); row++)
    {
        T sum = T();
        for (uint col = 0; col < mtx.size2(); col++)
        {
            sum += mtx(row, col);
        }
        vec[row] = sum / mtx.size2();
    }
    return vec;
}


void showSolution_14_1_2_3();





int main(int argc, char* argv[])
{
    QCoreApplication app(argc, argv);
    using std::cout, std::endl;

#define log qDebug() << fixed << qSetRealNumberPrecision(3)

    showSolution_14_1_1();
    //    cout << "\n\n\n\n\n\n";
    //    showSample();



    //    showSolution_14_1_2_2();
    //    showSolution_14_1_2_3();




    cout << endl;
    //    return app.exec();
}



void showSolution_14_1_1()
{

#define log qDebug() << fixed << qSetRealNumberPrecision(3)
#define Log(x) qDebug() << fixed << qSetRealNumberPrecision(x)
#define s QString()+



    using std::cout, std::endl;
    cout << std::setprecision(3);

    std::istringstream stream;

    const int criterias1 = 2;
    const int criterias2 = 2;
    const int alternatives = 3;
    HierarchyAlgorithm ahp(alternatives);

    cout << "---------------------------------Уровень 1---------------------------------------\n";

    cout << "Отношение: Мартин, Джейн" << endl;
    matrix<double> lvl1(criterias1, criterias1);
    std::string str = "1    1 "
                      "1    1 ";
    stream.str(str);
    fill(lvl1, stream);
    ahp.addLevel({lvl1});

    show(lvl1);
    normalize(lvl1);
    QVector<double> vLvl1 = avrRows(lvl1);
    Log(2) << vLvl1;
    cout << endl;



    cout << "\n\n\n---------------------------------Уровень 2---------------------------------------\n";


    cout << "Мартин оценил отношение: местоположение, репутация" << endl;
    matrix<double> lvl2_1(criterias2, criterias2);
    str = "1  0.2 "
          "5    1 ";
    stream.str(str);
    fill(lvl2_1, stream);
    ahp.addLevel({lvl2_1});

    show(lvl2_1);
    normalize(lvl2_1);
    QVector<double> vLvl2_1 = avrRows(lvl2_1);
    Log(2) << vLvl2_1;
    cout << endl;



    cout << "Джейн оценила отношение: местоположение, репутация" << endl;
    matrix<double> lvl2_2(criterias2, criterias2);
    str = "1     0.42 "
          "2.33     1 ";
    stream.str(str);
    fill(lvl2_2, stream);
    ahp.addMatrix(2, {lvl2_2});

    show(lvl2_2);
    normalize(lvl2_2);
    QVector<double> vLvl2_2 = avrRows(lvl2_2);
    Log(2) << vLvl2_2;
    cout << endl;



    cout << "\n\n\n--------------------------------Уровень 3 (альтернативы)---------------------------------------\n";
    cout << "Мартин оценил отношение универов по местоположению" << endl;
    matrix<double> lvl3_1(alternatives, alternatives);
    str = "1   0.5  0.2 "
          "2   1    0.5 "
          "5   2      1 ";
    stream.str(str);
    fill(lvl3_1, stream);
    ahp.addLevel({lvl3_1});

    show(lvl3_1);
    normalize(lvl3_1);
    QVector<double> vLvl3_1 = avrRows(lvl3_1);
    Log(3) << vLvl3_1;
    cout << endl;

    cout << "Мартин оценил отношение универов по репутации" << endl;
    matrix<double> lvl3_2(alternatives, alternatives);
    str = "1           2        3 "
          "0.5         1      1.5 "
          "0.333   0.666        1 ";
    stream.str(str);
    fill(lvl3_2, stream);
    ahp.addMatrix(3, {lvl3_2});

    show(lvl3_2);
    normalize(lvl3_2);
    QVector<double> vLvl3_2 = avrRows(lvl3_2);
    Log(3) << vLvl3_2;
    cout << endl;


    cout << "\n------------\n";


    cout << "Джейн оценила отношение универов по местоположению" << endl;
    matrix<double> lvl3_3(alternatives, alternatives);
    str = "1     0.66    0.4 "
          "1.5    1      0.6 "
          "2.5   1.66      1 ";
    stream.str(str);
    fill(lvl3_3, stream);
    ahp.addMatrix(3, {lvl3_3});

    show(lvl3_3);
    normalize(lvl3_3);
    QVector<double> vLvl3_3 = avrRows(lvl3_3);
    Log(1) << vLvl3_3;
    cout << endl;

    cout << "Джейн оценила отношение универов по репутации" << endl;
    matrix<double> lvl3_4(alternatives, alternatives);
    str = "1     2.5    1.66 "
          "0.4   1      0.66 "
          "0.6   1.5      1 ";
    stream.str(str);
    fill(lvl3_4, stream);
    ahp.addMatrix(3, {lvl3_4});

    show(lvl3_4);
    normalize(lvl3_4);
    QVector<double> vLvl3_4 = avrRows(lvl3_4);
    Log(1) << vLvl3_4;
    cout << endl;


    cout << "\n\n\n AVR ROWS" << endl;
    log << "\t" << vLvl1;
    log << "" << vLvl2_1;
    log << "\t\t" << vLvl2_2;
    log << "" << vLvl3_1;
    log << "" << vLvl3_2;
    log << "\t\t" << vLvl3_3;
    log << "\t\t" << vLvl3_4;


    QVector<QVector<double>> alts;
    alts.push_back(vLvl3_1);
    alts.push_back(vLvl3_2);
    alts.push_back(vLvl3_3);
    alts.push_back(vLvl3_4);


    qDebug() << "TEST---------------------- " << ahp.calculateWeights();
    qDebug() << ahp.getAnswer();

    QVector<QVector<QVector<double>>> all;

    QVector<QVector<double>> vectorLVL_1;
    vectorLVL_1.push_back(vLvl1);

    all.push_back(vectorLVL_1);

    QVector<QVector<double>> vectorLVL_2;
    vectorLVL_2.push_back(vLvl2_1);
    vectorLVL_2.push_back(vLvl2_2);

    all.push_back(vectorLVL_2);

    QVector<QVector<double>> vectorLVL_3;
    vectorLVL_3.push_back(vLvl3_1);
    vectorLVL_3.push_back(vLvl3_2);
    vectorLVL_3.push_back(vLvl3_3);
    vectorLVL_3.push_back(vLvl3_4);

    all.push_back(vectorLVL_3);


    log << "ALL LEVEL VECTORS";
    log << all[0][0];
    log << all[1][0] << all[1][1];
    log << all[2][0] << all[2][1] << all[2][2] << all[2][3];

    log << calc(all, 3);





    QVector<double> result(alternatives);
    //    for (int i = 0; i < all[0].size(); i++)
    //    {
    //        for (int j = 0; j < all[0][i].size(); j++)
    //        {
    //            double dd = all[level][i][j]; //0.5
    //        }
    //    }





    //    for (int i = 0; i < alternatives; i++)
    //    {
    //        for (int j = 0; j < criterias; ++j)
    //        {
    //            result[i] += vLvl1[j] * alts[j][i];
    //        }
    //    }

    //    log << "\nRESULT";
    //    log << result;
    //    log << *std::max_element(result.begin(), result.end());

}




void showSolution_14_1_2_2()
{
#define log qDebug() << fixed << qSetRealNumberPrecision(3)
#define Log(x) qDebug() << fixed << qSetRealNumberPrecision(x)
#define s QString()+
    using std::cout, std::endl;
    cout << std::setprecision(3);

    std::istringstream stream;

    const int criterias1 = 2;
    const int criterias2 = 2;
    const int alternatives = 3;


    cout << "---------------------------------Уровень 1---------------------------------------\n";

    cout << "Отношение: Кевин, Джун" << endl;
    matrix<double> lvl1(criterias1, criterias1);
    std::string str = "1      1 "
                      "0.5    1 ";
    stream.str(str);
    fill(lvl1, stream);
    show(lvl1);
    normalize(lvl1);
    QVector<double> vLvl1 = avrRows(lvl1);
    Log(2) << vLvl1;
    cout << endl;



    cout << "\n\n\n---------------------------------Уровень 2---------------------------------------\n";


    //    cout << "Мартин оценил отношение: местоположение, репутация" << endl;
    matrix<double> lvl2_1(criterias2, criterias2);
    str = "1  0.333 "
          "3    1 ";
    stream.str(str);
    fill(lvl2_1, stream);
    show(lvl2_1);
    normalize(lvl2_1);
    QVector<double> vLvl2_1 = avrRows(lvl2_1);
    Log(2) << vLvl2_1;
    cout << endl;



    //    cout << "Джейн оценила отношение: местоположение, репутация" << endl;
    matrix<double> lvl2_2(criterias2, criterias2);
    str = "1       4 "
          "0.25     1 ";
    stream.str(str);
    fill(lvl2_2, stream);
    show(lvl2_2);
    normalize(lvl2_2);
    QVector<double> vLvl2_2 = avrRows(lvl2_2);
    Log(2) << vLvl2_2;
    cout << endl;




    cout << "\n\n\n--------------------------------Уровень 3 (альтернативы)---------------------------------------\n";
    //    cout << "Мартин оценил отношение универов по местоположению" << endl;
    matrix<double> lvl3_1(alternatives, alternatives);
    str = "1         2    3 "
          "0.5       1    2 "
          "0.333   0.5    1 ";
    stream.str(str);
    fill(lvl3_1, stream);
    show(lvl3_1);
    normalize(lvl3_1);
    QVector<double> vLvl3_1 = avrRows(lvl3_1);
    Log(3) << vLvl3_1;
    cout << endl;

    //    cout << "Мартин оценил отношение универов по репутации" << endl;
    matrix<double> lvl3_2(alternatives, alternatives);
    str = "1           2      0.5 "
          "0.5         1      0.333 "
          "2           3        1 ";
    stream.str(str);
    fill(lvl3_2, stream);
    show(lvl3_2);
    normalize(lvl3_2);
    QVector<double> vLvl3_2 = avrRows(lvl3_2);
    Log(3) << vLvl3_2;
    cout << endl;


    cout << "\n------------\n";


    //    cout << "Джейн оценила отношение универов по местоположению" << endl;
    matrix<double> lvl3_3(alternatives, alternatives);
    str = "1       4      2 "
          "0.25    1      3 "
          "0.5  0.333     1 ";
    stream.str(str);
    fill(lvl3_3, stream);
    show(lvl3_3);
    normalize(lvl3_3);
    QVector<double> vLvl3_3 = avrRows(lvl3_3);
    Log(1) << vLvl3_3;
    cout << endl;

    //    cout << "Джейн оценила отношение универов по репутации" << endl;
    matrix<double> lvl3_4(alternatives, alternatives);
    str = "   1     0.5    4 "
          "   2       1    3 "
          "0.25   0.333    1 ";
    stream.str(str);
    fill(lvl3_4, stream);
    show(lvl3_4);
    normalize(lvl3_4);
    QVector<double> vLvl3_4 = avrRows(lvl3_4);
    Log(1) << vLvl3_4;
    cout << endl;





    QVector<QVector<double>> alts;
    alts.push_back(vLvl3_1);
    alts.push_back(vLvl3_2);
    alts.push_back(vLvl3_3);
    alts.push_back(vLvl3_4);


    QVector<QVector<QVector<double>>> all;

    QVector<QVector<double>> vectorLVL_1;
    vectorLVL_1.push_back(vLvl1);

    all.push_back(vectorLVL_1);

    QVector<QVector<double>> vectorLVL_2;
    vectorLVL_2.push_back(vLvl2_1);
    vectorLVL_2.push_back(vLvl2_2);

    all.push_back(vectorLVL_2);

    QVector<QVector<double>> vectorLVL_3;
    vectorLVL_3.push_back(vLvl3_1);
    vectorLVL_3.push_back(vLvl3_2);
    vectorLVL_3.push_back(vLvl3_3);
    vectorLVL_3.push_back(vLvl3_4);

    all.push_back(vectorLVL_3);



    log << calc(all, 3);

}




void showSolution_14_1_2_3()
{
#define log qDebug() << fixed << qSetRealNumberPrecision(3)
#define Log(x) qDebug() << fixed << qSetRealNumberPrecision(x)
#define s QString()+
    using std::cout, std::endl;
    cout << std::setprecision(3);

    std::istringstream stream;

    const int criterias1 = 3;
    const int alternatives = 2;


    cout << "---------------------------------Уровень 1---------------------------------------\n";

    matrix<double> lvl1(criterias1, criterias1);
    std::string str = "1      1     0.25 "
                      "1      1      0.2 "
                      "4      5        1 ";

    stream.str(str);
    fill(lvl1, stream);
    show(lvl1);
    normalize(lvl1);
    QVector<double> vLvl1 = avrRows(lvl1);
    Log(2) << vLvl1;
    cout << endl;



    cout << "\n\n\n---------------------------------Уровень 2---------------------------------------\n";


    //    cout << "Мартин оценил отношение: местоположение, репутация" << endl;
    matrix<double> lvl2_1(alternatives, alternatives);
    str = "1      2 "
          "0.5    1 ";
    stream.str(str);
    fill(lvl2_1, stream);
    show(lvl2_1);
    normalize(lvl2_1);
    QVector<double> vLvl2_1 = avrRows(lvl2_1);
    Log(2) << vLvl2_1;
    cout << endl;



    //    cout << "Джейн оценила отношение: местоположение, репутация" << endl;
    matrix<double> lvl2_2(alternatives, alternatives);
    str = "1       0.5 "
          "2     1 ";
    stream.str(str);
    fill(lvl2_2, stream);
    show(lvl2_2);
    normalize(lvl2_2);
    QVector<double> vLvl2_2 = avrRows(lvl2_2);
    Log(2) << vLvl2_2;
    cout << endl;


    matrix<double> lvl2_3(alternatives, alternatives);
    str = "1       1 "
          "1     1 ";
    stream.str(str);
    fill(lvl2_3, stream);
    show(lvl2_3);
    normalize(lvl2_3);
    QVector<double> vLvl2_3 = avrRows(lvl2_3);
    Log(2) << vLvl2_3;
    cout << endl;






    QVector<QVector<QVector<double>>> all;

    QVector<QVector<double>> vectorLVL_1;
    vectorLVL_1.push_back(vLvl1);

    all.push_back(vectorLVL_1);

    QVector<QVector<double>> vectorLVL_2;
    vectorLVL_2.push_back(vLvl2_1);
    vectorLVL_2.push_back(vLvl2_2);
    vectorLVL_2.push_back(vLvl2_3);

    all.push_back(vectorLVL_2);


    log << calc(all, 2);

}






void showSample()
{
#define log qDebug() << fixed << qSetRealNumberPrecision(3)
    using std::cout, std::endl;
    cout << std::setprecision(3);

    std::istringstream stream;
    int criterias = 2;
    int alternatives = 3;

    matrix<double> lvl1(criterias, criterias);
    std::string str = "1  0.2 "
                      "5    1 ";
    stream.str(str);
    fill(lvl1, stream);

    matrix<double> lvl2_1(alternatives, alternatives);
    str = "1  0.5  0.2 "
          "2  1    0.5 "
          "5  2      1 ";
    stream.str(str);
    fill(lvl2_1, stream);

    matrix<double> lvl2_2(alternatives, alternatives);
    str = "1           2        3 "
          "0.5         1      1.5 "
          "0.333   0.666        1 ";
    stream.str(str);
    fill(lvl2_2, stream);

    show(lvl1);
    cout << endl;
    show(lvl2_1);
    cout << endl;
    show(lvl2_2);



    cout << "\nNORMALIZATION\n";
    normalize(lvl1);
    normalize(lvl2_1);
    normalize(lvl2_2);

    show(lvl1);
    cout << endl;
    show(lvl2_1);
    cout << endl;
    show(lvl2_2);


    cout << "\nAVR ROWS\n";
    QVector<double> vLvl1 = avrRows(lvl1);

    QVector<double> vLvl2_1 = avrRows(lvl2_1);
    QVector<double> vLvl2_2 = avrRows(lvl2_2);


    log << vLvl1;
    log << vLvl2_1;
    log << vLvl2_2;

    QVector<QVector<double>> alts;
    alts.push_back(vLvl2_1);
    alts.push_back(vLvl2_2);


    QVector<double> result(alternatives);

    for (int i = 0; i < alternatives; i++)
    {
        for (int j = 0; j < criterias; ++j)
        {
            result[i] += vLvl1[j] * alts[j][i];
        }
    }
    log << "\nRESULT";
    log << result;
    log << *std::max_element(result.begin(), result.end());
}







