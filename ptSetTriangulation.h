#ifndef _ptSetTriangulation_h_
#define _ptSetTriangulation_h_

#include <QVector>
//#include <QGraphicsScene>
//#include <QGraphicsLineItem>

#include "point.h"
#include "edge.h"
#include "triangle.h"



class ptSetTriangulation
{
public:
  ptSetTriangulation();
  ptSetTriangulation(QVector<CPoint*>*);

  ~ptSetTriangulation();

  QVector<triangle> incremental();
  QVector<triangle> graham();
  QVector<triangle> divideConquer();
  QVector<triangle> bw_delaunay();

private:
  QVector<CPoint>    m_pts;

  triangle superTriangle();
  QVector<edge> uniqueEdges(QVector<edge>);


};

#endif

