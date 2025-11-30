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

  bool operator<(const edge&);
  bool operator==(const edge&);

  friend std::ostream& operator<<(std::ostream&, const edge&);

private:
  CPoint  m_org;
  CPoint  m_dest;
};

#endif