#include "edge.h"

edge::edge(const edge& other)
{
  this->m_org = other.m_org;
  this->m_dest = other.m_dest;
}

bool edge::operator<(const edge& rhs)
{
  if ((m_org < rhs.m_org)) { return true; }
  else if ((m_org == rhs.m_org)) {
    if ((m_dest < rhs.m_dest)) { return true; }
    else { return false; }
  }
  else { return false; }

}

bool edge::operator==(const edge& rhs)
{
  bool bRet = false;

  bRet = (((m_org == rhs.m_org) && (m_dest == rhs.m_dest)) || ((m_org == rhs.m_dest) && (m_dest == rhs.m_org)));
 
  return bRet;
}

std::ostream& operator<<(std::ostream& os, const edge& e)
{
  os << "edge: " << const_cast<edge&>(e).org() << " to " << const_cast<edge&>(e).dst();
  return os;
}
