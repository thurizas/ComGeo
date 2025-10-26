#ifndef _convexHull_h_
#define _convexHull_h_

#include <QVector>

#include "point.h"
#include "edge.h"


class convexHull
{
public:
  convexHull();
  convexHull(QVector<CPoint*>*);

  ~convexHull();

  QVector<edge*>* jervisMarch();

private:
  QVector<edge*>*    m_hull;
  QVector<CPoint>    m_pts;
};

#endif

