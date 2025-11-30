#ifndef _veronoi_h_
#define _veronoi_h_

#include <QVector>

#include <queue>
#include <set>
#include <cmath>

#include "point.h"

//forward declaration of arc, and segment datatypes
struct arc;
struct seg;

static QVector<seg*>  output;         // vector of output segments


struct event
{
  float  x;
  CPoint pt;
  arc*   a;
  bool   valid;

  event(float _x, CPoint _p, arc* _a) :x(_x), pt(_p), a(_a), valid(true) {}
};

struct arc
{
  CPoint   p;
  arc*     prev;
  arc*     next;
  event*   e;
  seg*     s0;
  seg*     s1;

  arc(CPoint pt, arc* a = nullptr, arc* b = nullptr) : p(pt), prev(a), next(b), e(0), s0(0), s1(0) {}
};

struct seg
{
  CPoint start;
  CPoint end;
  bool   done;

  seg(CPoint pt) : start(pt), end(-1, 0.0, 0.0), done(false)
  {
    output.push_back(this);
  }

  seg(CPoint a, CPoint b) :start(a), end(b), done(true) {}

  void finish(CPoint pt)
  {
    if (done) return;
    end = pt;
    done = true;
  }
};

struct gt
{
  bool operator()(CPoint a, CPoint b) { return a.x() == b.x() ? a.y() > b.y() : a.x() > b.x(); }
  bool operator()(event* a, event* b) {return a->x > b->x; }
};


class voronoi
{
public:
  voronoi();
  voronoi(QVector<CPoint*>*);

  QVector<seg*> voronoiCells();

private:
  QVector<CPoint>            m_pts;
  std::priority_queue<CPoint, std::vector<CPoint>, gt> m_points;             // site events
  std::priority_queue<event*, std::vector<event*>, gt> m_events;             // circle events


  void processPoint();
  void processEvent();
  void frontInsert(CPoint  p);

  bool circle(CPoint a, CPoint b, CPoint c, double* x, CPoint* o);
  void checkCircleEvent(arc* i, double x0);

  bool intersect(CPoint p, arc* i, CPoint* result = 0);
  CPoint intersection(CPoint p0, CPoint p1, double l);

  void finishEdges();
  //void print_output();

  arc*  m_root = nullptr; // root of the linked list

  float bbMinX = INFINITY;   //X0        //bounding box coordinates.
  float bbMaxX = -INFINITY;  //X1
  float bbMinY = INFINITY;   //Y0
  float bbMaxY = -INFINITY;  //Y1
};

#endif

