#include <QApplication>
#include <QDesktopWidget>
#include <QStyleFactory>
#include <QDebug>

#include "mainwindow.h"
#include "compute/ahp.h"




int main(int argc, char* argv[])
{
    QApplication app(argc, argv);
    QApplication::setStyle(QStyleFactory::create("Fusion"));
    MainWindow w;
    w.setGeometry(QStyle::alignedRect(Qt::LeftToRight, Qt::AlignCenter, w.size(),
                                      qApp->desktop()->availableGeometry()));
    w.show();


    Matrix m1 = {
        {  1,     5,  1},
        {0.2,     1,  3},
        {  1, 0.333,  1}
    };

    Matrix m2 = {
        {1,   0.5,  0.2},
        {2,     1,  0.5},
        {5,     2,    1}
    };

    Matrix m3 = {
        {1, 1},
        {1, 1}
    };

    qDebug() << "m1=" << AlgorithmAHP::getCR(m1);
    qDebug() << "m2=" << AlgorithmAHP::getCR(m2);
    qDebug() << "m2=" << AlgorithmAHP::getCR(m3);

//    QTest::qExec(new AlghorithmAHP_Test, argc, argv);


    return app.exec();
}
