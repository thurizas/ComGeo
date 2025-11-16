#include "convexHull.h"
#include "utility.h"

#include "point.h"
#include "vector.h"

#include <iostream>
#include <iomanip>
#include <algorithm>
#include <QDebug>

convexHull::convexHull() 
{

}

convexHull::convexHull(QVector<CPoint*>* pts) 
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
QVector<CPoint> convexHull::jervisMarch()
{  
  QVector<CPoint>        hull;
  int                    leftNdx = 0;                           // index of left-most vertex

  int32_t cntPts = m_pts.size();                                // number of points in the set

  if (cntPts >= 3)                                              // must have at least three points
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
      // correct: (-5, -1), (-3, -2), (1, -2), (3, -2), (5, 1), (7, 4), (6, 5), (3, 5), (2, 5), (0, 5), (-3, 4), (-5, 2), (-5, 1), (-5, -1),
      // result : (-5, -1), (-3, -2), (1, -2), (3, -2), (5, 1), (7, 4), (6, 5), (3, 5), (2, 5), (0, 5), (-3, 4), (-5, 2),          (-5, -1),
      std::cout << "(" << pt.x() << ", " << pt.y() << "), ";
    }
  }

  return hull;
}

// structure to represent a point with additional fields needed by Graham Scan's algorithm
struct gspt
{
  float_t      angle;
  float_t      distance;
  CPoint       pt;
};

void printHull(QList<struct gspt>* hull)
{
  if (hull->size() == 0)
    std::cout << "empty";
  else
  {
    for (QList<struct gspt>::iterator iter = hull->begin(); iter != hull->end(); iter++)
    {
      std::cout << (*iter).pt.ndx() << ", ";
    }
  }
}

/***********************************************************************************************************************
 * Function: grahamScan
 *
 * Abstract:
 *           pseudo-code:
 *               let points be the list of points
 *               let stack = empty_stack()
 *
 *               find the lowest y-coordinate and leftmost point, called P0
 *               sort points by polar angle with P0, 
 *                if several points have the same polar angle then only keep the farthest
 *
 *               for point in points:
 *                  # pop the last point from the stack if we turn clockwise to reach this point
 *                  while count stack > 1 and ccw(next_to_top(stack), top(stack), point) <= 0:
 *                      pop stack
 *                  push point to stack
 *               end
 *
 * Input   :
 *
 * Returns :
 *
 * Written : Nov 2025 (gkhuber) 
 **********************************************************************************************************************/
QVector<CPoint> convexHull::grahamScan()
{
  QVector<CPoint>        hull;
  QVector<gspt>          points;
  QList<gspt>            stack;
  int                    leftNdx = 0;                           // index of left-most vertex

  int32_t cntPts = m_pts.size();                                // number of points in the set

  if (cntPts >= 3)                                              // must have at least three points
  {
    for (int i = 1; i < cntPts; i++)                            // find bottom most point, on ties, use left-most point.
    {
      if (m_pts[i].y() < m_pts[leftNdx].y())
      {
        leftNdx = i;
      }
      else if (m_pts[i].y() == m_pts[leftNdx].y())
      {
        if (m_pts[i].x() < m_pts[leftNdx].x())
          leftNdx = i;
      }
    }

    // reference horizontal vector through bottom most point
    CVector refLine(m_pts[leftNdx].x(), m_pts[leftNdx].y(), m_pts[leftNdx].x() + 1.0f, m_pts[leftNdx].y());

    for (CPoint pt : m_pts)
    {
      float angle = 0.0;
      float dist = 0.0;

      if (pt == m_pts[leftNdx])                      // handle reference point
      {
        angle = 0.0f;
        dist = 0.0f;
      }
      else
      {
        CVector temp(m_pts[leftNdx], pt);        // vector from left most point to test poine
        angle = temp.angleBetween(refLine);      // angle between horizontal line and vector
        dist = temp.len();                       // length of vector.
      }

      struct gspt   tempPt { angle, dist, pt };
      points.push_back(tempPt);
    }


    //for (gspt& p : points) { std::cout << std::setw(6) << std::setprecision(4) << p.pt.ndx() << ":(" << p.pt.x() << ", " << p.pt.y() << ")\tangle: " << p.angle << "\tdistance: "<< p.distance << std::endl; }
    //std::cout << std::endl;

    // sort list by angle, if multiple points with same angle keep furthest one.
    std::sort(points.begin(), points.end(), [](struct gspt& lhs, struct gspt& rhs) 
    {
        if (lhs.angle == rhs.angle) return lhs.distance < rhs.distance;
        else return lhs.angle < rhs.angle; 
    });

    //for (gspt& p : points) { std::cout << std::setw(6) << std::setprecision(4) << p.pt.ndx() << ":(" << p.pt.x() << ", " << p.pt.y() << ")\tangle: " << p.angle << "\tdistance: " << p.distance << std::endl; }
    //std::cout << std::endl;
    
    int step = 0;
    for (const gspt& pt : points)
    {
      std::cout << "step: " << step << " "; printHull(&stack); std::cout  << std::endl;

      int head = stack.size()-1;
      int prev = stack.size() - 2; //head - 1 ;

      while ((stack.size() > 1) && (orient(stack.at(prev).pt, stack.at(head).pt, pt.pt) < 0))
      {
        stack.pop_back();
        head = stack.size() - 1; prev = stack.size() - 2; //head - 1 ;
      }

      stack.push_back(pt);
      step++;
    }

    // populate convex-hull data structure and return to program
    for (gspt pt : stack)
    {
      hull.append(pt.pt);
      std::cout << pt.pt << ", ";
    }
  }

  return hull;
}



/***********************************************************************************************************************
 * Function: mergeHull
 *
 * Abstract:
 *
 * Input   :
 *
 * Returns :
 *
 * Written : Nov 2025 (gkhuber)
 **********************************************************************************************************************/
QVector<CPoint> convexHull::mergeHull()
{
  QVector<CPoint>        hull;

  qDebug() << "Not yet implemented";

  return hull;

}
