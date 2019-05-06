#include <QApplication>
#include <QDesktopWidget>
#include <QStyleFactory>
#include <QFile>
#include <QStyle>
#include <QDebug>
#include <QFontDatabase>

#include "mainwindow.h"
#include "alg_ahp.h"


void test1();
void test2();
void test3();


//template<typename T>
//struct is_pointer { static const bool value = false; };

//template<typename T>
//struct is_pointer<T*> { static const bool value = true; };


template<class T>
void foo(T n = T())
{
    qDebug() << n;
    qDebug() << ((void*)n == nullptr);
    qDebug() << "check " << is_pointer<T>::value;
}


int main(int argc, char* argv[])
{
    foo<int*>();
    foo<int>();
    QApplication app(argc, argv);

    QFontDatabase db;
    QStringList const families = db.families();
    qDebug() << families;

    MainWindow w;
    w.setGeometry(QStyle::alignedRect(Qt::LeftToRight, Qt::AlignCenter, w.size(),
                                      qApp->desktop()->availableGeometry()));
    w.show();
//    Matrix m1 = {
//        {1,   5,    1},
//        {0.2, 1,    3},
//        {1,  0.333, 1}
//    };


//    Matrix m2 = {
//        {1,   0.5,  0.2},
//        {2,     1,    0.5},
//        {5,     2, 1}
//    };

//    Matrix m3 = {
//        {1, 1},
//        {1, 1}
//    };

//    qDebug() << "m1=" << AlghorithmAHP::getCR(m1);
//    qDebug() << "m2=" << AlghorithmAHP::getCR(m2);
//    qDebug() << "m2=" << AlghorithmAHP::getCR(m3);



        Matrix m = {
            {1, 0.2},
            {5, 1}
        };

//        qDebug() << "m1=" << AlghorithmAHP::getCR(m);
//        qDebug() << "check" << (AlghorithmAHP::getCR(m) > 0.1);
    //    test1();
    //    test2();
//        test3();


    QApplication::setStyle(QStyleFactory::create("Fusion"));
    return app.exec();
}


////задача 14_1_2_3
//void test1()
//{
//    AlghorithmAHP ahp(2);

//    std::vector<int> ves;

//    Matrix mx = {
//        { 1, 1, 0.25 },
//        { 1, 1,  0.2 },
//        { 4, 5,    1 }
//    };

//    auto CRs = ahp.addLevel({mx});


//    Matrix lvl2_1 = {
//        {   1, 2 },
//        { 0.5, 1 }
//    };


//    Matrix lvl2_2 = {
//        { 1, 0.5 },
//        { 2,   1 }
//    };

//    Matrix lvl2_3 = {
//        { 1, 1 },
//        { 1, 1 }
//    };


//    ahp.addLevel({lvl2_1, lvl2_2, lvl2_3});

//    auto pair = ahp.answer();
//    for (double wght : pair.second)
//    {
//        qDebug() << wght;
//    }
//    qDebug() << "INDEX = " << pair.first;
//    qDebug() << CRs[0];

//    // правильный ответ index = 0, [0.501916; 0,498084]
//    // CR = 0.0066
//}



////задача 14_1_2_2
//void test2()
//{
//    AlghorithmAHP ahp(3);

//    //----------------------level 1--------------------------
//    Matrix mx = {
//        {   1, 2 },
//        { 0.5, 1 }
//    };
//    ahp.addLevel({mx});


//    //----------------------level 2--------------------------

//    Matrix lvl2_1 = {
//        { 1, 0.333 },
//        { 3,     1 }
//    };

//    Matrix lvl2_2 = {
//        {    1, 4 },
//        { 0.25, 1 }
//    };

//    ahp.addLevel({lvl2_1, lvl2_2});

//    //----------------------level 3--------------------------

//    Matrix lvl3_1 = {
//        {     1,   2, 3 },
//        {   0.5,   1, 2 },
//        { 0.333, 0.5, 1 }
//    };

//    Matrix lvl3_2 = {
//        {   1, 2,   0.5 },
//        { 0.5, 1, 0.333 },
//        {   2, 3,     1 }
//    };

//    Matrix lvl3_3 = {
//        {    1,     4, 2 },
//        { 0.25,     1, 3 },
//        {  0.5, 0.333, 1 }
//    };

//    Matrix lvl3_4 = {
//        {    1,   0.5, 4 },
//        {    2,     1, 3 },
//        { 0.25, 0.333, 1 }
//    };

//    ahp.addLevel({lvl3_1, lvl3_2, lvl3_3, lvl3_4});


//    auto pair = ahp.answer();
//    for (double wght : pair.second)
//    {
//        qDebug() << wght;
//    }
//    qDebug() << "INDEX = " << pair.first;
//}


////задача 14_1_1  Выбор универа Мартин и Джейн
//void test3()
//{
//    AlghorithmAHP ahp(3);

//    //----------------------level 1--------------------------
//    Matrix mx = {
//        { 1, 1 },
//        { 1, 1 }
//    };
//    ahp.addLevel({mx});


//    //----------------------level 2--------------------------

//    Matrix lvl2_1 = {
//        { 1, 0.2 },
//        { 5,   1 }
//    };

//    Matrix lvl2_2 = {
//        {    1, 0.42 },
//        { 2.33,    1 },
//    };

//    ahp.addLevel({lvl2_1, lvl2_2});

//    //----------------------level 3--------------------------

//    Matrix lvl3_1 = {
//        { 1, .5, .2 },
//        { 2,  1, .5 },
//        { 5,  2,  1 }
//    };

//    Matrix lvl3_2 = {
//        {    1,    2,   3 },
//        {   .5,    1, 1.5 },
//        { .333, .666,   1 }
//    };

//    Matrix lvl3_3 = {
//        {    1,   .66, .4 },
//        {  1.5,     1, .6 },
//        {  2.5,  1.66,  1 }
//    };

//    Matrix lvl3_4 = {
//        {  1, 2.5, 1.66 },
//        { .4,   1,  .66 },
//        { .6, 1.5,    1 }
//    };


//    //    Matrix lvl3_5 = {
//    //        {  1,  .2, 1.66 },
//    //        {  5,   1, .333 },
//    //        { .6,   3,    1 }
//    //    };

//    std::vector<double>  CR =  ahp.addLevel({lvl3_1, lvl3_2, lvl3_3, lvl3_4});

////    for (double coef : CR) {
////        cout << "coef: " << coef << endl;
////    }


//    auto pair = ahp.answer();
//    for (double wght : pair.second)
//    {
//        qDebug() << wght;
//    }
//    qDebug() << "INDEX = " << pair.first;

//    // правильный ответ index = 0, [0.44214, 0.25184, 0.30602]
//}

