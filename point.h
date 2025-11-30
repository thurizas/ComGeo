#pragma once

#include <cfloat>
#include <cmath>
#include <ostream>


class CPoint
{
  public:
      CPoint();
      CPoint(uint32_t, float_t, float_t);
      CPoint& operator=(const CPoint&);
  
      float_t x() { return m_x; }
      void x(float_t _x) { m_x = _x; }
      float_t y() { return m_y; }
      void y(float_t _y) { m_y = _y; }
      int32_t ndx() { return m_ndx; }
  
      void randomIntPoint(int, int, int, int);
      void randomFloatPoint(float_t, float_t, float_t, float_t);
  
      bool operator==(const CPoint& other);
      bool operator==(const CPoint* other);
      bool operator!=(const CPoint& other);
      bool operator<(const CPoint& other);
  
      friend std::ostream& operator<<(std::ostream&, const CPoint&);

  private:
    int32_t   m_ndx;
    float_t   m_x;
    float_t   m_y;
};
