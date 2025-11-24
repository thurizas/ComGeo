#ifndef _triangle_h_
#define _triangle_h_

#include <iostream>

#include "point.h"

struct triangle
{
  triangle(CPoint _a, CPoint _b, CPoint _c);

  CPoint pt(uint8_t ndx);
  bool contains(CPoint);

  friend std::ostream& operator<<(std::ostream&, const triangle&);

private:
  CPoint m_pts[3];
  };

#endif

