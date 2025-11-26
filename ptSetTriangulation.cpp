#include "ptSetTriangulation.h"

#include "point.h"
#include "edge.h"

#include <QVector>
#include <array>

ptSetTriangulation::ptSetTriangulation()
{


}

ptSetTriangulation::ptSetTriangulation(QVector<CPoint*>* pvec)
{
  for (CPoint* pt : *pvec)
  {
    m_pts.push_back(*pt);
  }
}

ptSetTriangulation::~ptSetTriangulation()
{

}


QVector<triangle> ptSetTriangulation::BowyerWatson()
{
  QVector<triangle> triangulation;

  // build super-triangle
  float cm_x = 0;
  float cm_y = 0;
  float minx, maxx, miny, maxy;

  minx = maxx = (const_cast<CPoint&>(m_pts.at(0))).x();
  miny = maxy = (const_cast<CPoint&>(m_pts.at(0))).y();

  // calculate center of mass and extent of point set
  for (CPoint pt : m_pts)
  {
    cm_x += pt.x();
    cm_y += pt.y();

    minx = std::min<float>(minx, pt.x());
    maxx = std::max<float>(maxx, pt.x());
    miny = std::min<float>(miny, pt.y());
    maxy = std::max<float>(maxy, pt.y());
  }

  cm_x /= m_pts.size();
  cm_y /= m_pts.size();
  float r = 1.1 * std::max({ fabs(cm_x - minx), fabs(maxx - cm_x), fabs(cm_y - miny), fabs(maxy - cm_y) });

  CPoint pt1(-1,cm_x - r   , cm_y-r);
  CPoint pt2(-2, cm_x - r, cm_y + 3 * r);
  CPoint pt3(-3, cm_x + 3 * r, cm_y - r);
  triangle superTriangle(pt1, pt2, pt3);

  triangulation.push_back(superTriangle);

  int i = 0;
  for (CPoint pt : m_pts)
  {
    // get triangle that the point is in
    for(uint32_t ndx = 0; ndx < triangulation.size(); ndx++)
    {
      if(triangulation[ndx].contains(pt))
      {
        triangulation.push_back(triangle{pt, triangulation[ndx].pt(0), triangulation[ndx].pt(1)});
        triangulation.push_back(triangle{pt, triangulation[ndx].pt(1), triangulation[ndx].pt(2)});
        triangulation.push_back(triangle{pt, triangulation[ndx].pt(2), triangulation[ndx].pt(0)});

        // remove the containing triangle
        triangulation.remove(ndx);
        triangulation.squeeze();

        i++;
        break;
      }
    }
  }

  // remove any triange containing a point from super triangle
  uint32_t cntTriangles = triangulation.size();
  for (int ndx = cntTriangles -1; ndx >= 0; ndx--)
  {
    if (triangulation[ndx].contains(pt1) || triangulation[ndx].contains(pt2) || triangulation[ndx].contains(pt3))
    {
      triangulation.remove(ndx);
    }
  }

  //triangulation.squeeze();

  return triangulation;
}


QVector<triangle> ptSetTriangulation::graham()
{
  QVector<triangle> triangles;

  return triangles;
}

QVector<triangle> ptSetTriangulation::divideConquer()
{
  QVector<triangle> triangles;

  return triangles;
}

QVector<triangle> ptSetTriangulation::delaunay()
{
  QVector<triangle> triangles;

  return triangles;
}