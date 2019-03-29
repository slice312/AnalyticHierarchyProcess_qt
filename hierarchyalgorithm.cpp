#include <QDebug>

#include "hierarchyalgorithm.h"


HierarchyAlgorithm::HierarchyAlgorithm(int alternatives)
{
    this->level = 0;
    this->alternatives = alternatives;
}



HierarchyAlgorithm::~HierarchyAlgorithm()
{
    qDebug() << "model deleted";
}



int HierarchyAlgorithm::getAlternatives() const
{
    return alternatives;
}



QPair<int, double> HierarchyAlgorithm::getAnswer() const
{
    double max = 0.0;
    int altr = -1;
    for (int i = 0; i < results.size(); i++)
    {
        if (results[i] > max)
        {
            max = results[i];
            altr = i;
        }
    }
    return QPair<int, double>(altr + 1, max);
}



void HierarchyAlgorithm::addLevel(const QList<matrixf>& mtxs)
{
    QList<matrixf> list(mtxs);
    this->list.push_back(list);
    this->level++;
}



bool HierarchyAlgorithm::addMatrix(int onLevel, const QList<matrixf>& mtxs)
{
    if (onLevel <= level && onLevel > 0)
    {
        list[onLevel - 1].append(mtxs);
        return true;
    }
    return false;
}



QVector<double> HierarchyAlgorithm::calculateWeights()
{
    weights.resize(level);

    for (int i = 0; i < list.size(); i++)
    {
        for (int j = 0; j < list[i].size(); j++)
        {
            matrixf& m = list[i][j];
            normalize(m);
            weights[i].push_back(avrRows(m));
        }
    }

    for (int i = 0; i < alternatives; i++)
    {
        double res = combinedWeighting(0, 0, i);
        Q_ASSERT_X(res > 0.0, __func__, "wrong weight");
        results.push_back(res);
    }
    return results;
}



void HierarchyAlgorithm::normalize(matrixf& mtx)
{
    for (uint col = 0; col < mtx.size2(); col++)
    {
        double sum = 0.0;
        for (uint row = 0; row < mtx.size1(); row++)
            sum += mtx(row, col);

        for (uint row = 0; row < mtx.size1(); row++)
            mtx(row, col) /= sum;
    }
}



QVector<double> HierarchyAlgorithm::avrRows(matrixf& mtx)
{
    QVector<double> vec((mtx.size1()));
    for (uint row = 0; row < mtx.size1(); row++)
    {
        double sum = 0.0;
        for (uint col = 0; col < mtx.size2(); col++)
            sum += mtx(row, col);

        vec[row] = sum / mtx.size2();
    }
    return vec;
}



double HierarchyAlgorithm::combinedWeighting(int lvl, int onlvl, int alt)
{
    if (lvl == this->level - 1)
        return weights[lvl][onlvl][alt];

    for (int i = lvl; i <= this->level; i++)
    {
        for (int vecOnLevel = onlvl; vecOnLevel < list[lvl].size(); vecOnLevel++)
        {
            double sum = 0.0;
            for (int i = 0; i < weights[lvl][vecOnLevel].size(); i++)
            {
                sum += weights[lvl][vecOnLevel][i] *
                        combinedWeighting(lvl + 1, vecOnLevel * 2 + i, alt);
            }
            return sum;
        }
    }
    return -1.0;
}
