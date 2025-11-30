#pragma once

#include <QtWidgets/QMainWindow>

#include <vector>

#include "point.h"
#include "ptSetTriangulation.h"
#include "voronoi.h"

//class QGraphicsView;
class QGraphicsScene;
class QGraphicsLineItem;
class TwoDGeomView;
class QMenu;
class QAction;

class ComGeo : public QMainWindow
{
    Q_OBJECT

public:
  enum DATA : std::uint8_t { POINT = 0, LINE = 1, POLYGON = 2 };

    ComGeo(uint32_t, uint32_t, uint32_t, QWidget *parent = Q_NULLPTR);
    void setupUI();
    void setupActions();
    void setupMenus();

private slots:
    void onNew();
    void onExit();
    void onRandomPtSet();
    void onReadPtSet();
    void onRandomPoly();
    void onReadPoly();
    void onReadPolys();
    void onViewAxis();
    void onViewGrid();
    void onViewDataSet(uint8_t);
    void onViewRefresh();
    void onAlgoJarvisMarch();
    void onAlgoGrahamScan();
    void onAlgoMergeHull();
    void onTriangulateIncremental();
    void onTriangulateGrahmanAlgorithm();
    void onTriangulateDivideConquer();
    void onTriangulateDelaunay();
    void onAlgoVoronoi();

    void onAbout();
    void onHelp();


private:
    TwoDGeomView*     m_graphicsView;
    QGraphicsScene*   m_pScene;
    float_t           m_scale;
    uint32_t          m_imageWidth;
    uint32_t          m_imageHeight;

    QMenu* m_fileMenu;
    QMenu* m_dataMenu;
    QMenu* m_viewMenu;
    QMenu* m_algoMenu;
    QMenu* m_helpMenu;

    QAction* m_fileNew;
    QAction* m_fileExit;
    QAction* m_dataRandomPtSet;
    QAction* m_dataReadPtSet;
//    QAction* m_dataRandomPoly;
    QAction* m_dataReadPoly;
    QAction* m_dataReadPolys;
    QAction* m_viewAxis;
    QAction* m_viewGrid;
    QAction* m_viewPointDataset;
    QAction* m_viewLineDataset;
    QAction* m_viewPolygonDataset;
    QAction* m_viewRefresh;
    QAction* m_algoJarvisMarch;
    QAction* m_algoGrahamScan;
    QAction* m_algoMergeHull;
    QAction* m_algoIncremental;
    QAction* m_algoGraham;
    QAction* m_algoDivideConqure;
    QAction* m_algoDelaunay;
    QAction* m_algoVoronoi;
    QAction* m_helpAbout;
    QAction* m_helpHelp;

    std::tuple<float, float, float, float> m_extents;

    QVector<CPoint*>                       m_vecPointSet;
    QVector<triangle>                      m_triangles;
    QVector<CPoint>                        m_vertexList;
    QVector<seg*>                          m_cells;

    bool               m_bDrawAxis;
    bool               m_bDrawGrid;

    void clearData();
    void drawScene();
};
