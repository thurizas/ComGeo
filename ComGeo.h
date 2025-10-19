#pragma once

#include <QtWidgets/QMainWindow>

//class QGraphicsView;
class QGraphicsScene;
class TwoDGeomView;
class QMenu;
class QAction;
class CPoint;

class ComGeo : public QMainWindow
{
    Q_OBJECT

public:
    ComGeo(QWidget *parent = Q_NULLPTR);
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
    void onViewRefresh();
    void onAbout();
    void onHelp();


private:
    TwoDGeomView*     m_graphicsView;
    QGraphicsScene* m_pScene;

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
    QAction* m_viewRefresh;
    QAction* m_helpAbout;
    QAction* m_helpHelp;

    QVector<CPoint*>    m_vecPointSet;
    QVector<CPoint*>    m_vertexList;

    bool               m_bDrawAxis;
    bool               m_bDrawGrid;

    void clearData();
    void drawScene();
};
