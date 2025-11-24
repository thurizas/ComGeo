#include "triangle.h"
#include "logger.h"
#include "utility.h"

triangle::triangle(CPoint a, CPoint b, CPoint c) : m_pts{ a,b,c }
{


}

CPoint triangle::pt(uint8_t ndx)
{
  if ((0 <= ndx) && (ndx < 3))
  {
    return m_pts[ndx];
  }
  else
  {
    CLogger::getInstance()->outMsg(cmdLine, CLogger::level::WARNING, "index out of bounds");
    // TODO : throw exception
  }
}

bool triangle::contains(CPoint pt)
{
  return (orient(m_pts[0], m_pts[1], pt) == orient(m_pts[1], m_pts[2], pt) == orient(m_pts[2], m_pts[0], pt));
}

std::ostream& operator<<(std::ostream& os, const triangle& t)
{
  os << "[" << const_cast<triangle&>(t).pt(0) << " - " << const_cast<triangle&>(t).pt(1) << " - " << const_cast<triangle&>(t).pt(2) << "]" << std::endl;
  return os;

}

