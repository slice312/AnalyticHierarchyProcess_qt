#include <QCoreApplication>
#include <QDebug>
#include <QGenericMatrix>
#include <QList>
#include <QtAlgorithms>

#include <iostream>
#include <sstream>
#include <string>
#include <iomanip>
#include <algorithm>

#include <boost/numeric/ublas/matrix.hpp>
#include <boost/numeric/ublas/io.hpp>


template<class T>
using matrix = boost::numeric::ublas::matrix<T>;


void showSample();
void showSolution_14_1_1();



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
            std::cout << m(row, col) << "  ";
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






int main(int argc, char* argv[])
{
    QCoreApplication app(argc, argv);
    using std::cout, std::endl;

    #define log qDebug() << fixed << qSetRealNumberPrecision(3)

    showSolution_14_1_1();




    cout << endl;
    //    return app.exec();
}



void showSolution_14_1_1()
{

#define log qDebug() << fixed << qSetRealNumberPrecision(3)
#define s QString()+
    using std::cout, std::endl;
    cout << std::setprecision(3);

    std::istringstream stream;

    int criterias1 = 2;
    int criterias2 = 2;
    int criterias = 2;
    int alternatives = 3;

    log << "Отношение: Мартин, Джейн";
    matrix<double> lvl1(criterias1, criterias1);
    std::string str = "1    1 "
                      "1    1 ";
    stream.str(str);
    fill(lvl1, stream);

    //Мартин оценил соотношение: местоположение, репутация
    matrix<double> lvl2_1(criterias2, criterias2); //местоположение
    str = "1  0.2 "
          "5    1 ";
    stream.str(str);
    fill(lvl2_1, stream);


    show(lvl1);
    cout << endl;
    show(lvl2_1);
    cout << endl;



    cout << "\nNORMALIZATION\n";
    normalize(lvl1);
    normalize(lvl2_1);

    show(lvl1);
    cout << endl;
    show(lvl2_1);
    cout << endl;


//    cout << "\nAVR ROWS\n";
//    QVector<double> vLvl1 = avrRows(lvl1);

//    QVector<double> vLvl2_1 = avrRows(lvl2_1);
//    QVector<double> vLvl2_2 = avrRows(lvl2_2);


//    log << vLvl1;
//    log << vLvl2_1;
//    log << vLvl2_2;

//    QVector<QVector<double>> alts;
//    alts.push_back(vLvl2_1);
//    alts.push_back(vLvl2_2);


//    QVector<double> result(alternatives);

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







