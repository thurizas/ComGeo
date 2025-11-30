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

// see https://en.wikipedia.org/wiki/Bowyer%E2%80%93Watson_algorithm
QVector<triangle> ptSetTriangulation::incremental()
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

  triangulation.squeeze();

  return triangulation;
}

/*
* The S-Hull algorithm
* see: https://www.s-hull.org/
1: sellect a seed point x_0 from x_i.
2: sort according to |x_i - x_0|^2.
3: find the point x_j closest to x_0.
4: find the point x_k that creates the smallest circum-circle with x_0 and x_j and record the center of the circum-circle C.
5: order point x_0, x_j, x_k to give a right handed system thi is the initial seed convex hull.
6: resort the remaining points according to x_i - C|^2 to give points s_i.
7: sequentially add the points s_i to the porpagating 2D convex hull that is seeded with the triangle formed from x_0, x_j, x_k .
as a new point is added the facets of the 2D-hull that are visible to it form new triangles.
8: a non-overlapping triangulation of the set of points is created.
(This is an extremely fast method for creating an non-overlapping triangualtion of a 2D point set).
9: adjacent pairs of triangles of this triangulation must be 'flipped' to create a Delaunay triangulation from the initial non-overlapping triangulation.
*/
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

/*
* see: https://www.gorillasun.de/blog/bowyer-watson-algorithm-for-delaunay-triangulation/
function BowyerWatson (pointList)
    // pointList is a set of coordinates defining the points to be triangulated
    triangulation := empty triangle mesh data structure
    add super-triangle to triangulation                     // must be large enough to completely contain all the points in pointList
    for each point in pointList do                          // add all the points one at a time to the triangulation
        badTriangles := empty set
        for each triangle in triangulation do               // first find all the triangles that are no longer valid due to the insertion
            if point is inside circumcircle of triangle
                add triangle to badTriangles
        polygon := empty set
        for each triangle in badTriangles do                // find the boundary of the polygonal hole
            for each edge in triangle do
                if edge is not shared by any other triangles in badTriangles
                    add edge to polygon
        for each triangle in badTriangles do                // remove them from the data structure
            remove triangle from triangulation
        for each edge in polygon do                         // re-triangulate the polygonal hole
            newTri := form a triangle from edge to point
            add newTri to triangulation
    for each triangle in triangulation                      // done inserting points, now clean up
        if triangle contains a vertex from original super-triangle
            remove triangle from triangulation
    return triangulation
*/
QVector<triangle> ptSetTriangulation::bw_delaunay()
{
  QVector<triangle> triangles;
  QVector<edge>     edges;

  triangle st = superTriangle();
  triangles.push_back(st);

  // Remove triangles with circumcircles containing the vertex
  int cnt = 0;
  for (CPoint pt : m_pts)
  {
    cnt++;

    edges.clear();                                                               // reset edge list to empty list.
    edges.squeeze();                                                             // reset the capacity to zero

    int32_t cntTriangles = triangles.size();
    for(int32_t ndx = cntTriangles-1; ndx >= 0; ndx--)
    {
      if(triangles[ndx].inCircumCircle(pt))
      {
        edges.push_back(edge{ triangles[ndx].pt(0), triangles[ndx].pt(1) });     //push edges on to edge vector
        edges.push_back(edge{ triangles[ndx].pt(1), triangles[ndx].pt(2) });
        edges.push_back(edge{ triangles[ndx].pt(2), triangles[ndx].pt(0) });
        triangles.remove(ndx);                                                   //  remove triangle
        triangles.squeeze();                                                     //  squeeze triangle list to reclaim space
      }
    }
  
    // Remove duplicates from edge list
    //std::sort(edges.begin(), edges.end());
    //edges.erase(std::unique(edges.begin(), edges.end()), edges.end());
    edges = uniqueEdges(edges);

    // triangulate the polygonal hole
    for (edge e : edges)
    {
      triangles.push_back(triangle{ e.org(), e.dst(), pt });
    }

    //if (cnt == 5) break;
  }

  // remove any triange containing a point from super triangle
  uint32_t cntTriangles = triangles.size();
  for (int ndx = cntTriangles - 1; ndx >= 0; ndx--)
  {
    if (triangles[ndx].contains(st.pt(0)) || triangles[ndx].contains(st.pt(1)) || triangles[ndx].contains(st.pt(2)))
    {
      triangles.remove(ndx);
    }
  }

  // reclaim any space
  triangles.squeeze();

  return triangles;
}


triangle ptSetTriangulation::superTriangle()
{
  float minx = INFINITY, miny = INFINITY;
  float maxx = -INFINITY, maxy = -INFINITY;

  for (CPoint pt : m_pts)
  {
    minx = std::min<float>(minx, pt.x());
    miny = std::min<float>(miny, pt.y());
    maxx = std::max<float>(maxx, pt.x());
    maxy = std::max<float>(maxy, pt.y());
  }


  float dx = (maxx - minx) * 10;
  float dy = (maxy - miny) * 10;

  CPoint pt1(-1, minx - dx, miny - dy * 3);
  CPoint pt2(-2, minx - dx, maxy + dy);
  CPoint pt3(-3, maxx + dx * 3, maxy + dy);

  triangle st{ pt1, pt2, pt3 };
  return st;
}

QVector<edge> ptSetTriangulation::uniqueEdges(QVector<edge> edges)
{
  QVector<edge> uniqueEdges;

  for (int32_t ndx = 0; ndx < edges.size(); ndx++)
  {
    bool isUnique = true;

    for (int jdx = 0; jdx < edges.size(); jdx++)
    {
      if ((ndx != jdx) && (edges[ndx] == edges[jdx]))
      {
        isUnique = false;
        break;
      }
    }

    if (isUnique) uniqueEdges.push_back(edges[ndx]);
  }

  return uniqueEdges;
}