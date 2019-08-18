#include "point.h"

#include <random>

CPoint::CPoint() : m_dim(0), m_pData(nullptr)
{

}

CPoint::CPoint(int x, int y, int dim) :  m_dim(dim), m_pData(new int[dim])
{
    m_pData[0] = x;
    m_pData[1] = y;
}

CPoint::CPoint(int x, int y, int z, int dim) : m_dim(dim), m_pData(new int[dim])
{
    m_pData[0] = x;
    m_pData[1] = y;
    m_pData[2] = z;
}


CPoint::~CPoint()
{
    if (nullptr != m_pData)
        delete[] m_pData;
}

void CPoint::random(int nXMax, int nYMax)
{
    std::random_device                 rand_dev;
    std::mt19937                       generator(rand_dev());
    std::uniform_int_distribution<int> distX(0, nXMax);
    std::uniform_int_distribution<int> distY(0, nYMax);

    m_dim = 2;
    m_pData = new int[2];

    int x = distX(generator);
    int y = distY(generator);
    // TODO : generate value in rane [0, nXMax]
    m_pData[0] = x - nXMax / 2;

    // TODO : generate value in range [0, nYMax]
    m_pData[1] = y - nYMax / 2;

}


