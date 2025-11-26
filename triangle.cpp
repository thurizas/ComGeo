#include "triangle.h"
#include "logger.h"
#include "utility.h"

triangle::triangle(CPoint a, CPoint b, CPoint c) : m_pts{ a,b,c }
{
  if ((orient(a, b, c) == LEFT) && (orient(b, c, a) == LEFT) && (orient(c, a, b) == LEFT))
    m_winding = CCW;
  else if (((orient(a, b, c) == RIGHT) && (orient(b, c, a) == RIGHT) && (orient(c, a, b) == RIGHT)))
    m_winding = CW;
  else
    m_winding = UNK;

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

// we assume clock-wise winding
bool triangle::contains(CPoint pt)
{
  // quick check is the point on of the vertices
  if ((pt == m_pts[0]) || (pt == m_pts[1]) || (pt == m_pts[2])) return true;

  int32_t o1 = orient(m_pts[0], m_pts[1], pt);
  int32_t o2 = orient(m_pts[1], m_pts[2], pt);
  int32_t o3 = orient(m_pts[2], m_pts[0], pt);

  return ((o1 == RIGHT) && (o2 == RIGHT) && (o3 == RIGHT));
}

std::ostream& operator<<(std::ostream& os, const triangle& t)
{
  os << "[" << const_cast<triangle&>(t).pt(0) << ";" << const_cast<triangle&>(t).pt(1) << "; " << const_cast<triangle&>(t).pt(2) << "]" << std::endl;
  return os;

}

// a paramertize version of stream insertion operator for triangles
// printArgsT -- [in] a tuple consisting of the triangle structure (position 0) and a boolean (position 1)
//     if the boolean is true then a short form is printed (only the vertex id's)
//     if the boolean is false then a long form is printed (vertex id along with coordinates).
std::ostream& operator<<(std::ostream& os, const printArgsT& args)
{    
  triangle t = std::get<0>(args);
  if (std::get<1>(args))
  {
    os << "[" << t.pt(0).ndx() << ", " << t.pt(1).ndx() << ", " << t.pt(2).ndx() << "]:" << static_cast<int16_t>(t.m_winding) << std::endl;
  }
  else
  {
    os << t;
  }
  return os;
}

