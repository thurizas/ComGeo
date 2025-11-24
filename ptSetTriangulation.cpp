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




/*
 function BowyerWatson (pointList)
    triangulation := empty triangle mesh data structure
    add super-triangle to triangulation          // must be large enough to completely contain all the points in pointList
    for each point in pointList do               // add all the points one at a time to the triangulation
        badTriangles := empty set
        for each triangle in triangulation do    // first find all the triangles that are no longer valid due to the insertion
            if point is inside circumcircle of triangle
                add triangle to badTriangles
        polygon := empty set
        for each triangle in badTriangles do     // find the boundary of the polygonal hole
            for each edge in triangle do
                if edge is not shared by any other triangles in badTriangles
                    add edge to polygon
        for each triangle in badTriangles do     // remove them from the data structure
            remove triangle from triangulation
        for each edge in polygon do              // re-triangulate the polygonal hole
            newTri := form a triangle from edge to point
            add newTri to triangulation
    for each triangle in triangulation           // done inserting points, now clean up
        if triangle contains a vertex from original super-triangle
            remove triangle from triangulation
    return triangulation
 */
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
  CPoint pt2(-1,cm_x + 3*r , cm_y-r );
  CPoint pt3(-1,cm_x - r   , cm_y+3*r);
  triangle superTriangle(pt1, pt2, pt3);

  triangulation.push_back(superTriangle);

  int i = 0;
  for (CPoint pt : m_pts)
  {

    std::cout << "adding point: " << pt << std::endl;
    // get triangle that the point is in
    for(uint32_t ndx = 0; ndx < triangulation.size(); ndx++)
    {
      //if ((*viter).contains(pt));
      if(triangulation[ndx].contains(pt))
      {
        triangulation.push_back(triangle{ pt, triangulation[ndx].pt(0), triangulation[ndx].pt(1) });
        triangulation.push_back(triangle{ pt, triangulation[ndx].pt(1), triangulation[ndx].pt(2) });
        triangulation.push_back(triangle{ pt, triangulation[ndx].pt(2), triangulation[ndx].pt(0) });

        // remove the containing triangle
        triangulation.remove(ndx);

        i++;
        break;
      }
    }

    //if (i == 5) break;
  }

  // remove any triange containing a point from super triangle
  std::array<CPoint, 3> pts = { pt1, pt2, pt3 };
  for (CPoint pt : pts)
  {
    uint32_t cntTriangles = triangulation.size();
    for (int ndx = cntTriangles-1; ndx >= 0; ndx--)
    {
      if (triangulation[ndx].contains(pt))
        triangulation.remove(ndx);
    }
    triangulation.squeeze();
  }


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