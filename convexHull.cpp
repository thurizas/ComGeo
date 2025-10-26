#include "convexHull.h"
#include "utility.h"

#include <iostream>

convexHull::convexHull() : m_hull(nullptr)
{

}

convexHull::convexHull(QVector<CPoint*>* pts) : m_hull(nullptr)
{
  for (CPoint* pt : *pts)
  {
    m_pts.push_back(*pt);
  }
}

convexHull::~convexHull()
{

}


//*********************************************************************************************************************
// Function: jervisMarch
//
// Abstract: This calculates the convex hull of a point set by using Jervis March (or gift wrapping) algorithm.  The
//           pseudocode for the algorithm is
//
//            Input: A set of points P
//            Output: A list of points that form the convex hull of P
//
//            Find the leftmost point L in P and add it to the output list
//            Set the current point C to L
//            Repeat
//               Set the next point N to the first point in P
//               For each point Q in P
//                   If Q is not N and Q is on the left of the line segment CN
//                       Set N to Q
//               Add N to the output list
//               Set C to N
//            Until C is L
// 
// Input   : void
//
// Returns : and edge list for the convex hull
//
// Written : Oct 2025 (gkhuber)*
//*********************************************************************************************************************
QVector<edge*>* convexHull::jervisMarch()
{  
  QVector<CPoint>        hull;
  int                    leftNdx = 0;                           // index of left-most vertex

  int32_t cntPts = m_pts.size();                                // number of points in the set

  if (cntPts < 3)                                               // must have at least three points
  {
    return nullptr;
  }
  else
  {
    for (int i = 1; i < cntPts; i++)                            // find left most point
    {
      if(m_pts[i] < m_pts[leftNdx])
      {
        leftNdx = i;
      }
    }

    int32_t curNdx = leftNdx, nextNdx;
    hull.push_back(m_pts[curNdx]);                              // add left most point to hull list
    do
    {
      nextNdx = (curNdx + 1) % cntPts;
      for (int32_t i = 0; i < cntPts; i++)
      {
        int32_t o = orient(m_pts[curNdx], m_pts[nextNdx], m_pts[i]);

        if ((m_pts[i] != m_pts[nextNdx]) && (o == RIGHT))
        { 
          nextNdx = i;
        }
      }

      curNdx = nextNdx;                                         // nextNdx = 13, curNdx = 15, i = 14, leftNdx = 9
      hull.push_back(m_pts[curNdx]);
    } while (curNdx != leftNdx);

    for (CPoint& pt : hull)
    {
      // (-5, -1), (-3, -2), (1, -2), (3, -2), (5, 1), (7, 4), (6, 5), (3, 5), (2, 5), (0, 5), (-3, 4), (-5, 2), (-5, 1), (-5, -1),
      std::cout << "(" << pt.x() << ", " << pt.y() << "), ";
    }
  }

  return nullptr;

}
