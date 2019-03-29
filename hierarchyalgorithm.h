#ifndef _HIERARCHY_ALGORITHM_H_INCLUDED
#define _HIERARCHY_ALGORITHM_H_INCLUDED

#include <QList>
#include <QVector>
#include <QPair>

#include <boost/numeric/ublas/matrix.hpp>

using matrixf = boost::numeric::ublas::matrix<double>;



class HierarchyAlgorithm
{
private:
    int level;
    int alternatives;
    QList<QList<matrixf>> list;
    QVector<QVector<QVector<double>>> weights;
    QVector<double> results;


public:
    HierarchyAlgorithm(int alternatives);
    ~HierarchyAlgorithm();

    int getAlternatives() const;
    QPair<int, double> getAnswer() const;


    void addLevel(const QList<matrixf>& mtxs);
    bool addMatrix(int onLevel, const QList<matrixf>& mtxs);
    QVector<double> calculateWeights();


private:
    void normalize(matrixf& mtx);
    QVector<double> avrRows(matrixf& mtx);
    double combinedWeighting(int level, int onlvl, int alt);
};

#endif // _HIERARCHY_ALGORITHM_H_INCLUDED
