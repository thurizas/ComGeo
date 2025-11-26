#ifndef _triangle_h_
#define _triangle_h_

#include <iostream>
#include <tuple>

typedef std::tuple<struct triangle, bool> printArgsT;

#include "point.h"

struct triangle
{
  triangle(CPoint _a, CPoint _b, CPoint _c);

  CPoint pt(uint8_t ndx);
  int8_t windingOrder() { return m_winding; }
  bool contains(CPoint);

  friend std::ostream& operator<<(std::ostream&, const triangle&);
  friend std::ostream& operator<<(std::ostream&, const printArgsT&);

private:
  CPoint  m_pts[3];
  uint8_t m_winding;
  };

#endif

