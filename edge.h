#ifndef _edge_h_
#define _edge_h_

#include "point.h"

class edge
{
public:
  edge();
  edge(CPoint o, CPoint d) : m_org(o), m_dest(d) {}
  edge(const edge&);

  ~edge() { }

  CPoint org() { return m_org; }
  CPoint dst() { return m_dest; }

private:
  CPoint  m_org;
  CPoint  m_dest;
};

#endif