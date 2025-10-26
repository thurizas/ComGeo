#ifndef _edge_h_
#define _edge_h_

#include "point.h"

class edge
{
public:
  edge();
  edge(CPoint, CPoint);
  edge(const edge&);

  ~edge();

private:
  CPoint  org;
  CPoint  dest;
};

#endif