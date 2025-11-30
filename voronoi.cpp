#include "voronoi.h"

#include "point.h"
#include "edge.h"

#include <QVector>

voronoi::voronoi() : m_pts({})
{

}

voronoi::voronoi(QVector<CPoint*>* pts) 
{
  for (CPoint* pt : *pts)
  {
    CPoint point(pt->ndx(), pt->x(), pt->y());
    m_points.push(point);

    // keep track of min and max for bounding box
    bbMinX = std::min<float>(bbMinX, pt->x());
    bbMaxX = std::max<float>(bbMaxX, pt->x());
    bbMinY = std::min<float>(bbMinY, pt->y());
    bbMaxY = std::max<float>(bbMaxY, pt->y());
  }

  // add 20% margin to the bounding box.
  float dx = (fabs(bbMinX - bbMaxX) + 1) / 5.0f;
  float dy = (fabs(bbMinY - bbMaxY) + 1) / 5.0f;
  bbMinX -= dx; bbMaxX += dx;
  bbMinY -= dy; bbMaxY += dy;
}

/*
 * implementation of Fortune's algorithm
 *
 * pseudocode (from wikipedia)
 * also see 
 *  (a) de Berg, Mark et.al. Computational Geometry, (2nd revised ed) pp.151-160
 *  (b) Heunis, Jacques. "Fortunes Algorithm: An Intuitive explanation". https://jacquesheunis.com/post/fortunes-algorithm/
 *  (c) Vigier, Pierre. "Fortune's Algorithm: The Details". https://pvigier.github.io/2018/11/18/fortune-algorithm-details.html
 *  (d) Brubeck, Matt. "Fortune's Algorithm in C++". https://www.cs.hmc.edu/~mbrubeck/voronoi.html
 *
 * let *(z) be the transformation *(z) =(z_x,z_y+d(z)) where d(z) is the Euclidean distance between z and the nearest site
 * let T be the "beach line"
 * let R_p be the region covered by site p.
 * let C_{pq} be the boundary ray between sites p and q.
 * let S be a set of sites on which this algorithm is to be applied.
 * let p_1,p_2,...,p_m be the sites extracted from S with minimal y-coordinate, ordered by x-coordinate
 * let DeleteMin(X) be the act of removing the lowest and leftmost site of X (sort by y unless they're identical, in which
 *     case sort by x)
 * let V be the Voronoi map of S which is to be constructed by this algorithm
 *
 * Q <- {p_{1},p_{2},\dots ,p_{m},S
 * create initial vertical boundary rays C_{p_{1},p_{2}}^{0},C_{p_{2},p_{3}}^{0},\dots ,C_{p_{m-1},p_{m}}^{0}}
 * T <- *(R_{p_{1}}),C_{p_{1},p_{2}}^{0},*(R_{p_{2}}),C_{p_{2},p_{3}}^{0},\dots ,*(R_{p_{m-1}}),C_{p_{m-1},p_{m}}^{0},*(R_{p_{m}})}
 *
 * while not IsEmpty(Q) do
 *     p <- DeleteMin(Q)
 *     case p of
 *     p is a site in *(V):
 *         find the occurrence of a region *(R_{q})} in T containing p,
 *           bracketed byC_{rq} on the left  C_{qs}} on the right
 *         create new boundary rays C_{pq}^{-}} and C_{pq}^{+}} with bases p
 *         replace*(R_{q})} with *(R_{q}),C_{pq}^{-},*(R_{p}),C_{pq}^{+},*(R_{q})} in T
 *         delete from Q any intersection between  C_{rq} and C_{qs}}
 *         insert into Q any intersection between  C_{rq} and C_{pq}^{-}}
 *         insert into Q any intersection between C_{pq}^{+} and C_{qs}}
 *     p is a Voronoi vertex in *(V):
 *         let p be the intersection of C_{qr}} on the left and  C_{rs}} on the right
 *         let C_{uq}} be the left neighbor of  C_{qr} and
 *         let C_{sv} be the right neighbor of C_{rs} in T
 *         if q_{y}=s_{y}},
 *             create a new boundary ray C_{qs}^{0}
 *         else if p is right of the higher of q and s,
 *             create C_{qs}^{+}
 *         else
 *             createC_{qs}^{-}
 *         endif
 *         replace C_{qr},*(R_{r}),C_{rs}} with newly created C_{qs} in T
 *         delete from Q any intersection between C_{uq} and C_{qr}
 *         delete from Q any intersection between C_{rs} and  C_{sv}
 *         insert into Q any intersection between C_{uq} and C_{qs}
 *         insert into Q any intersection between C_{qs} and C_{sv}
 *         record p as the summit of C_{qr} and C_{rs} and the base of C_{qs}
 *         output the boundary segments C_{qr} and C_{rs}
 *     endcase
 * endwhile
 *
 * output the remaining boundary rays in T
 *
*/
QVector<seg*> voronoi::voronoiCells()
{
  while (!m_points.empty())
  {
    if (!m_events.empty() && m_events.top()->x <= const_cast<CPoint&>(m_points.top()).x())
      processEvent();
    else
      processPoint();
  }

  while (!m_events.empty())
    processEvent();

  finishEdges();

  // add bounding box to ouput

  output.push_back(new seg(CPoint(-1, bbMinX, bbMinY), CPoint(-1, bbMaxX, bbMinY)));  // top side
  output.push_back(new seg(CPoint(-1, bbMinX, bbMinY), CPoint(-1, bbMinX, bbMaxY)));  // left side
  output.push_back(new seg(CPoint(-1, bbMaxX, bbMinY), CPoint(-1, bbMaxX, bbMaxY)));  // right side
  output.push_back(new seg(CPoint(-1, bbMinX, bbMaxY), CPoint(-1, bbMaxX, bbMaxY)));  // bottom side
  
  return output;

}

void voronoi::processEvent() 
{
  event* e = m_events.top();
  m_events.pop();

  if (e->valid)
  {
    seg* s = new seg(e->pt);            // start a new edge

    arc* a = e->a;                      // remove the associated arc from the front.
    if (a->prev)
    {
      a->prev->next = a->next;
      a->prev->s1 = s;
    }
    if (a->next)
    {
      a->next->prev = a->prev;
      a->next->s0 = s;
    }

    if (a->s0) a->s0->finish(e->pt);    // finish edges before and after a
    if (a->s1) a->s1->finish(e->pt);

    if (a->prev) checkCircleEvent(a->prev, e->x);
    if (a->next) checkCircleEvent(a->next, e->x);

  }
}

// get the next point and add it to the parabolic front.
void voronoi::processPoint() 
{
  CPoint pt = m_points.top();
  m_points.pop();

  frontInsert(pt);
} 

// adds a new point to the parabolic front
void voronoi::frontInsert(CPoint  p) 
{
  if (!m_root)               // if front does not exist, create and add point to it.
  {
    m_root = new arc(p);
    return;
  }

  // Find the current arc(s) at height p.y (if there are any).
  for (arc* i = m_root; i; i = i->next) {
    CPoint z, zz;
    if (intersect(p, i, &z)) 
    {
      // New parabola intersects arc i.  If necessary, duplicate i.
      if (i->next && !intersect(p, i->next, &zz)) {
        i->next->prev = new arc(i->p, i, i->next);
        i->next = i->next->prev;
      }
      else i->next = new arc(i->p, i);
      i->next->s1 = i->s1;

      // Add p between i and i->next.
      i->next->prev = new arc(p, i, i->next);
      i->next = i->next->prev;

      i = i->next; // Now i points to the new arc.

      // Add new half-edges connected to i's endpoints.
      i->prev->s1 = i->s0 = new seg(z);
      i->next->s0 = i->s1 = new seg(z);

      // Check for new circle events around the new arc:
      checkCircleEvent(i, p.x());
      checkCircleEvent(i->prev, p.x());
      checkCircleEvent(i->next, p.x());

      return;
    }
  }

  // Special case: If p never intersects an arc, append it to the list.
  arc* i;
  for (i = m_root; i->next; i = i->next); // Find the last node.

  i->next = new arc(p, i);
  // Insert segment between p and i
  CPoint start;
  start.x(bbMinX);
  start.y((i->next->p.y() + i->p.y()) / 2);
  i->s1 = i->next->s0 = new seg(start);
}

// finds right most point on a circle through a, b, and c
bool voronoi::circle(CPoint a, CPoint b, CPoint c, double* x, CPoint* o) 
{ 
  // Check that bc is a "right turn" from ab.
  if ((b.x() - a.x()) * (c.y() - a.y()) - (c.x() - a.x()) * (b.y() - a.y()) > 0)
    return false;

  // find the circumcircle for the thee points, algorithm from O'Rourke 2ed p. 189.
  double A = b.x() - a.x();
  double B = b.y() - a.y();
  double C = c.x() - a.x();
  double D = c.y() - a.y();
  double E = A * (a.x() + b.x()) + B * (a.y() + b.y());
  double F = C * (a.x() + c.x()) + D * (a.y() + c.y());
  double G = 2 * (A * (c.y() - b.y()) - B * (c.x() - b.x()));

  if (G == 0) return false;  // Points are co-linear.

  // Point o is the center of the circle.
  o->x((D * E - B * F) / G);
  o->y((A * F - C * E) / G);

  // o.x plus radius equals max x coordinate.
  *x = o->x() + sqrt(pow(a.x() - o->x(), 2) + pow(a.y() - o->y(), 2));
  return true;
}

// seeing if we need to create a new circle event for arc 'i'.
void voronoi::checkCircleEvent(arc* i, double x0) 
{
  if (i->e && i->e->x != x0)      // invalidate any old events
    i->e->valid = false;
  i->e = nullptr;                 // TODO : ??? are we leaking memory here

  if (!i->prev || !i->next)
    return;

  double x;
  CPoint pt;

  if (circle(i->prev->p, i->p, i->next->p, &x, &pt))
  {
    i->e = new event(x, pt, i);   // create a new event
    m_events.push(i->e);
  }
}

// does a new parabola at point p intersect with arc i
bool voronoi::intersect(CPoint p, arc* i, CPoint* result) 
{ 
  if (i->p.x() == p.x()) return false;

  double a, b;
  if (i->prev) // Get the intersection of i->prev, i.
    a = intersection(i->prev->p, i->p, p.x()).y();
  if (i->next) // Get the intersection of i->next, i.
    b = intersection(i->p, i->next->p, p.x()).y();

  if ((!i->prev || a <= p.y()) && (!i->next || p.y() <= b)) {
    result->y(p.y());

    // Plug it back into the parabola equation.
    result->x((i->p.x() * i->p.x() + (i->p.y() - result->y()) * (i->p.y() - result->y()) - p.x() * p.x())
      / (2 * i->p.x() - 2 * p.x()));

    return true;
  }
  return false;
}

// where do two parabolae, with centers @p0 and p1 intersect
CPoint voronoi::intersection(CPoint p0, CPoint p1, double l) 
{
  CPoint res, p = p0;

  if (p0.x() == p1.x())
    res.y((p0.y() + p1.y()) / 2);
  else if (p1.x() == l)
    res.y(p1.y());
  else if (p0.x() == l) {
    res.y(p0.y());
    p = p1;
  }
  else {
    // Use the quadratic formula.
    double z0 = 2 * (p0.x() - l);
    double z1 = 2 * (p1.x() - l);

    double a = 1 / z0 - 1 / z1;
    double b = -2 * (p0.y() / z0 - p1.y() / z1);
    double c = (p0.y() * p0.y() + p0.x() * p0.x() - l * l) / z0
      - (p1.y() * p1.y() + p1.x() * p1.x() - l * l) / z1;

    res.y((-b - sqrt(b * b - 4 * a * c)) / (2 * a));
  }
  // Plug back into one of the parabola equations.
  res.x((p.x() * p.x() + (p.y() - res.y()) * (p.y() - res.y()) - l * l) / (2 * p.x() - 2 * l));
  return res;
}

void voronoi::finishEdges() 
{
  double l = bbMaxX + (bbMaxX - bbMinX) + (bbMaxY - bbMinY);

  for (arc* i = m_root; i->next; i = i->next)
  {
    if (i->s1)
      i->s1->finish(intersection(i->p, i->next->p, 2 * l));
  }
}
