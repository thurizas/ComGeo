#include "2dGeomView.h"



TwoDGeomView::TwoDGeomView(QWidget *parent) : QGraphicsView(parent)
{
    setDragMode(QGraphicsView::ScrollHandDrag);
}


void TwoDGeomView::wheelEvent(QWheelEvent *event)
{
    if (event->delta() > 0)
        scale(1.25, 1.25);
    else
        scale(0.8, 0.8);
}

void TwoDGeomView::keyPressEvent(QKeyEvent *event)
{
    if (event->key() == Qt::Key_Left)
        rotate(1);
    else if (event->key() == Qt::Key_Right)
        rotate(-1);
}