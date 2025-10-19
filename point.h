#pragma once

class CPoint
{
public:
    CPoint();
    CPoint(int, int, int m_dim = 2);
    CPoint(int, int, int, int m_dim = 3);
    ~CPoint();

    void random(int, int);

    int getX() { return m_pData[0]; }
    int getY() { if (m_dim >= 2) return m_pData[1]; else return -1; }
    int getZ() { if (m_dim >= 3) return m_pData[2]; else return -1; }

private:
    int   m_dim;
    int*  m_pData;
};
