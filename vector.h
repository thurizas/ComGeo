#ifndef _vector_h_
#define _vector_h_

#include <cfloat>
#include <cmath>

#include "point.h"

#include <ostream>

class CVector
{
public:
  CVector(float_t x1, float_t y1, float_t x2, float_t y2);
  CVector(const CPoint org, const CPoint dst);
  CVector(const CVector& other);
  CVector operator=(const CVector& other);

  CPoint& org() { return m_org; }
  CPoint& dst() { return m_dst; }

  CVector operator+(const CVector&);
  CVector operator-(const CVector&);
  CVector operator*(float_t factor);

  CVector normalize();

  float_t len2();
  float_t len();

  float_t dot(const CVector&);
  float_t angleBetween(const CVector&);

  bool operator==(const CVector&);
  bool operator!=(const CVector&);

  friend std::ostream& operator<<(std::ostream&, const CVector&);

private:
  CPoint    m_org;
  CPoint    m_dst;
};

#endif

