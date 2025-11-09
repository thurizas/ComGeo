#pragma once

#include <cfloat>
#include <cmath>

class CPoint
{
public:
    CPoint();
    CPoint(float_t, float_t);
    CPoint& operator=(const CPoint&);

    float_t x() { return m_x; }
    float_t y() { return m_y; }

    void randomIntPoint(int, int, int, int);
    void randomFloatPoint(float_t, float_t, float_t, float_t);

    bool operator==(const CPoint& other);
    bool operator==(const CPoint* other);
    bool operator!=(const CPoint& other);
    bool operator<(const CPoint& other);

private:
  float_t   m_x;
  float_t   m_y;
};
