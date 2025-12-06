#include "logger.h"

#include "ComGeo.h"
#include "point.h"
#include "NumberEntry.h"
#include "utility.h"
#include "2DGeomView.h"
#include "dlgViewData.h"
#include "convexHull.h"
#include "ptSetTriangulation.h"
#include "voronoi.h"
#include "ptSizeDlg.h"

#include <QGraphicsScene>
#include <QGraphicsItem>
#include <QMainWindow>
#include <QVariant>
#include <QApplication>
#include <QGraphicsView>
#include <QHBoxLayout>
#include <QMenuBar>
#include <QStatusBar>
#include <QToolBar>
#include <QWidget>
#include <QDebug>
#include <QFile>
#include <QFileDialog>
#include <QMessageBox>
#include <QColorDialog>

#include <iostream>
#include <tuple>



ComGeo::ComGeo(uint32_t w, uint32_t h, uint32_t s, QWidget *parent) : QMainWindow(parent), m_pScene(nullptr), m_bDrawAxis(false), m_bDrawGrid(false)
{
  m_imageWidth = w;
  m_imageHeight = h;
  m_scale = s;

  // set up extents of the point set .. minx, maxx, miny, maxy
  m_extents = std::make_tuple(INFINITY, -INFINITY, INFINITY, -INFINITY);

  setupUI();
  setupActions();
  setupMenus();

  // build our scene object here... we will attach it to our render when we are ready to draw
  m_pScene = new QGraphicsScene;


  // initialize graphic items we will be using...
  m_ptPen = QPen(QBrush(Qt::black), 0, Qt::SolidLine);
  m_ptBrush = QBrush(Qt::SolidPattern);
  m_chPen = QPen(QBrush(Qt::black), 0, Qt::SolidLine);;
  //m_chBrush;
  m_triPen = QPen(QBrush(Qt::black), 0, Qt::SolidLine);;
  //m_triBrush;
  m_vorPen = QPen(QBrush(Qt::black), 0, Qt::SolidLine);;
  //m_vorBrush;
}

void ComGeo::setupUI()
{
    QMenuBar *menuBar;
    QToolBar *mainToolBar;
    QStatusBar *statusBar;


    if (this->objectName().isEmpty())
        this->setObjectName(QString::fromUtf8("caveGenClass"));
    this->resize(640, 480);


    QWidget* centralWidget = new QWidget(this);
    centralWidget->setObjectName(QString::fromUtf8("centralWidget"));


    QHBoxLayout* horizontalLayout = new QHBoxLayout(centralWidget);
    horizontalLayout->setSpacing(6);
    horizontalLayout->setContentsMargins(11, 11, 11, 11);
    horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));

    m_graphicsView = new /*QGraphicsView*/TwoDGeomView(centralWidget);
    m_graphicsView->setObjectName(QString::fromUtf8("graphicsView"));

    horizontalLayout->addWidget(m_graphicsView);

    menuBar = new QMenuBar(this);
    menuBar->setObjectName(QString::fromUtf8("menuBar"));
    menuBar->setGeometry(QRect(0, 0, 799, 21));
    this->setMenuBar(menuBar);

    mainToolBar = new QToolBar(this);
    mainToolBar->setObjectName(QString::fromUtf8("mainToolBar"));

    statusBar = new QStatusBar(this);
    statusBar->setObjectName(QString::fromUtf8("statusBar"));


    this->setCentralWidget(centralWidget);
    this->addToolBar(Qt::TopToolBarArea, mainToolBar);
    this->setStatusBar(statusBar);

    QMetaObject::connectSlotsByName(this);
}



void ComGeo::setupActions()
{ 
    m_fileNew = new QAction("&New", this);
    m_fileNew->setShortcuts(QKeySequence::Open);
    m_fileNew->setStatusTip("opens existing configuration file");
    connect(m_fileNew, &QAction::triggered, this, &ComGeo::onNew);

    m_fileExit = new QAction("e&Xit", this);
    m_fileExit->setShortcuts(QKeySequence::Quit);
    m_fileExit->setStatusTip("exits the application");
    connect(m_fileExit, &QAction::triggered, this, &ComGeo::onExit);

    m_dataRandomPtSet = new QAction("Random Pt set", this);
    m_dataRandomPtSet->setStatusTip("generate random point set");
    connect(m_dataRandomPtSet, &QAction::triggered, this, &ComGeo::onRandomPtSet);

    m_dataReadPtSet = new QAction("Read Pt set", this);
    m_dataReadPtSet->setStatusTip("read point set from a file");
    connect(m_dataReadPtSet, &QAction::triggered, this, &ComGeo::onReadPtSet);

    //m_dataRandomPoly = new QAction("Random Polygon", this);
    //m_dataRandomPoly->setStatusTip("generate random polygon");
    //connect(m_dataRandomPoly, &QAction::triggered, this, &ComGeo::onRandomPoly);

    m_dataReadPoly = new QAction("Read Polygon", this);
    m_dataReadPoly->setStatusTip("read polygon info from file");
    connect(m_dataReadPoly, &QAction::triggered, this, &ComGeo::onReadPoly);

    m_dataReadPolys = new QAction("Read multiple Polygons", this);
    m_dataReadPolys->setStatusTip("read multiple polygons from file");
    connect(m_dataReadPolys, &QAction::triggered, this, &ComGeo::onReadPolys);

    m_viewAxis = new QAction("view axis", this);
    m_viewAxis->setStatusTip("view coordiante axis");
    m_viewAxis->setCheckable(true);
    m_viewAxis->setChecked(m_bDrawAxis);
    connect(m_viewAxis, &QAction::triggered, this, &ComGeo::onViewAxis);

    m_viewGrid = new QAction("view grid", this);
    m_viewGrid->setStatusTip("view coordiante grid");
    m_viewGrid->setCheckable(true);
    m_viewGrid->setChecked(m_bDrawGrid);
    connect(m_viewGrid, &QAction::triggered, this, &ComGeo::onViewGrid);

    m_viewPointDataset = new QAction("view point set", this);
    m_viewPointDataset->setStatusTip("view point data set");
    connect(m_viewPointDataset, &QAction::triggered, this, std::bind(&ComGeo::onViewDataSet, this, ComGeo::DATA::POINT));

    m_viewLineDataset = new QAction("view line set", this);
    m_viewLineDataset->setStatusTip("view line data set");
    connect(m_viewLineDataset, &QAction::triggered, this, std::bind(&ComGeo::onViewDataSet, this, ComGeo::DATA::LINE));
    
    m_viewPolygonDataset = new QAction("view polygon set", this);
    m_viewPolygonDataset->setStatusTip("view polygon data set");
    connect(m_viewPolygonDataset, &QAction::triggered, this, std::bind(&ComGeo::onViewDataSet, this, ComGeo::DATA::POLYGON));

    m_viewRefresh = new QAction("refresh", this);
    m_viewRefresh->setStatusTip("Refresh the view");
    connect(m_viewRefresh, &QAction::triggered, this, &ComGeo::onViewRefresh);

    m_algoJarvisMarch= new QAction("Jarvis March",this);
    m_algoJarvisMarch->setStatusTip("calculate convex hull by Jarvis March");
    connect(m_algoJarvisMarch, &QAction::triggered, this, &ComGeo::onAlgoJarvisMarch);

    m_algoGrahamScan = new QAction("Graham Scan",this);
    m_algoGrahamScan->setStatusTip("calculate convex hull by Jarvis March");
    connect(m_algoGrahamScan, &QAction::triggered, this, &ComGeo::onAlgoGrahamScan);

    m_algoMergeHull  = new QAction("Merge Hull",this);
    m_algoMergeHull->setStatusTip("calculate convex hull by Jarvis March");
    connect(m_algoMergeHull, &QAction::triggered, this, &ComGeo::onAlgoMergeHull);

    m_algoIncremental = new QAction("Bowyer-Watson", this);
    m_algoIncremental->setStatusTip("calculate the triangualation by incremental method");
    connect(m_algoIncremental, &QAction::triggered, this, &ComGeo::onTriangulateIncremental);

    m_algoGraham = new QAction("Graham's Algorithm", this);
    m_algoGraham->setStatusTip("calculate the triangualation by Graham's Algorithm");
    connect(m_algoGraham, &QAction::triggered, this, &ComGeo::onTriangulateGrahmanAlgorithm);

    m_algoDivideConqure = new QAction("Divide and Conqure", this);
    m_algoDivideConqure->setStatusTip("calculate the triangualation by Divide adn Conquer");
    connect(m_algoDivideConqure, &QAction::triggered, this, &ComGeo::onTriangulateDivideConquer);

    m_algoDelaunay = new QAction("Delaunay", this);
    m_algoDelaunay->setStatusTip("calculate the triangualation by Delaunay Algorithm");
    connect(m_algoDelaunay, &QAction::triggered, this, &ComGeo::onTriangulateDelaunay);

    m_algoVoronoi = new QAction("Voronoi", this);
    m_algoVoronoi->setStatusTip("calculate the Voronoi partition of a point set");
    connect(m_algoVoronoi, &QAction::triggered, this, &ComGeo::onAlgoVoronoi);

    m_showPointSet = new QAction("show Point Set", this);
    m_showPointSet->setStatusTip("view/hide the point set");
    m_showPointSet->setCheckable(true);
    m_showPointSet->setChecked(m_bShowPtSet);
    connect(m_showPointSet, &QAction::triggered, this, &ComGeo::onShowPtSet);

    m_choosePointSetColor = new QAction("set Point Set color", this);
    m_choosePointSetColor->setStatusTip("set the color used to display point set");
    connect(m_choosePointSetColor, &QAction::triggered, this, &ComGeo::onChooseColorPtSet);

    m_setPointSetSize = new QAction("set Point Set size", this);
    m_setPointSetSize->setStatusTip("set the size used to display a point in the  point set");
    connect(m_setPointSetSize, &QAction::triggered, this, &ComGeo::onSetSizePtSet);

    m_showConvexHull = new QAction("show Convex Hull", this);
    m_showConvexHull->setStatusTip("view/hide the Convex Hull");
    m_showConvexHull->setCheckable(true);
    m_showConvexHull->setChecked(m_bShowConvexHull);
    connect(m_showConvexHull, &QAction::triggered, this, &ComGeo::onShowConvexHull);

    m_chooseConvexHullColor = new QAction("set Convex Hull color", this);
    m_chooseConvexHullColor->setStatusTip("set the color used to display the Convex Hull");
    connect(m_chooseConvexHullColor, &QAction::triggered, this, &ComGeo::onChooseColorConvexHull);

    m_setConvexHullLineProps = new QAction("set Convex Hull line properties", this);
    m_setConvexHullLineProps->setStatusTip("set the properties used to display the Convex Hull line");
    connect(m_setConvexHullLineProps, &QAction::triggered, this, &ComGeo::onSetConvexHullLineProps);
    
    m_showTriangulation = new QAction("show Triangulation", this);
    m_showTriangulation->setStatusTip("view/hide the Triangulation");
    m_showTriangulation->setCheckable(true);
    m_showTriangulation->setChecked(m_bShowTriangulation);
    connect(m_showTriangulation, &QAction::triggered, this, &ComGeo::onShowTriangulation);

    m_chooseTriangulationColor = new QAction("set Triangulation color", this);
    m_chooseTriangulationColor->setStatusTip("set the color used to display the Triangulation");
    connect(m_chooseTriangulationColor, &QAction::triggered, this, &ComGeo::onChooseColorTriangulation);

    m_setTriagulationLineProps = new QAction("set Triangulation line properties", this);
    m_setTriagulationLineProps->setStatusTip("set the properties used to display the Triangulation lines");
    connect(m_setTriagulationLineProps, &QAction::triggered, this, &ComGeo::onSetTriangulationLineProps);
    
    m_showVoronoi = new QAction("show Voronoi", this);
    m_showVoronoi->setStatusTip("view/hide the Voronoi Cells");
    m_showVoronoi->setCheckable(true);
    m_showVoronoi->setChecked(m_bShowVoronoi);
    connect(m_showVoronoi, &QAction::triggered, this, &ComGeo::onShowVoronoi);

    m_chooseVoronoiColor = new QAction("set Voronoi color", this);
    m_chooseVoronoiColor->setStatusTip("set the color used to display the Voronoi lines");
    connect(m_chooseVoronoiColor, &QAction::triggered, this, &ComGeo::onChooseColorVoronoi);

    m_setVoronoiLineProps = new QAction("set Voronoi line properties", this);
    m_setVoronoiLineProps->setStatusTip("set the properties used to display the Voronoi lines");
    connect(m_setVoronoiLineProps, &QAction::triggered, this, &ComGeo::onSetVoronoiLineProps);


    m_helpAbout = new QAction("About", this);
    //m_HelpAbout->setShortcuts(QKeySequence::About);
    m_helpAbout->setStatusTip("shows progam information");
    connect(m_helpAbout, &QAction::triggered, this, &ComGeo::onAbout);

    m_helpHelp = new QAction("&Help", this);
    m_helpHelp->setShortcuts(QKeySequence::HelpContents);
    m_helpHelp->setStatusTip("displays program help");
    connect(m_helpHelp, &QAction::triggered, this, &ComGeo::onHelp);
}



void ComGeo::setupMenus()
{  
    m_fileMenu = menuBar()->addMenu("&File");
    m_fileMenu->addAction(m_fileNew);
    m_fileMenu->addSeparator();
    m_fileMenu->addAction(m_fileExit);

    m_dataMenu = menuBar()->addMenu("Data");
    m_dataMenu->addAction(m_dataRandomPtSet);
    m_dataMenu->addAction(m_dataReadPtSet);
    //m_dataMenu->addAction(m_dataRandomPoly);
    m_dataMenu->addAction(m_dataReadPoly);
    m_dataMenu->addAction(m_dataReadPolys);

    //m_dataMenu->addSeparator();
    //m_editMenu->addAction(m_editStep);
    //m_editMenu->addSeparator();
    //m_editMenu->addAction(m_editProps);

    m_viewMenu = menuBar()->addMenu("View");
    m_viewMenu->addAction(m_viewAxis);
    m_viewMenu->addAction(m_viewGrid);
    m_viewMenu->addSeparator();
    QMenu* viewData =  m_viewMenu->addMenu("View Data Set");
    viewData->addAction(m_viewPointDataset);
    viewData->addAction(m_viewLineDataset);
    viewData->addAction(m_viewPolygonDataset);

    m_viewMenu->addAction(m_viewRefresh);

    m_algoMenu = menuBar()->addMenu("Algorithms");
    QMenu* chAlgos = m_algoMenu->addMenu("Convex Hull");
    chAlgos->addAction(m_algoJarvisMarch);
    chAlgos->addAction(m_algoGrahamScan);
    chAlgos->addAction(m_algoMergeHull);
    QMenu* triAlgo = m_algoMenu->addMenu("Point Set Triangulation");
    triAlgo->addAction(m_algoIncremental);
    triAlgo->addAction(m_algoGraham);
    triAlgo->addAction(m_algoDivideConqure);
    triAlgo->addAction(m_algoDelaunay);
    m_algoMenu->addAction(m_algoVoronoi);

    m_editMenu = menuBar()->addMenu("&Edit");
    QMenu* editPointSet = m_editMenu->addMenu("Point Set");
    editPointSet->addAction(m_showPointSet);
    editPointSet->addAction(m_choosePointSetColor);
    editPointSet->addAction(m_setPointSetSize);

    QMenu* editConvexHull = m_editMenu->addMenu("Convex Hull");
    editConvexHull->addAction(m_showConvexHull);
    editConvexHull->addAction(m_chooseConvexHullColor);
    editConvexHull->addAction(m_setConvexHullLineProps);

    QMenu* editTriangulation = m_editMenu->addMenu("Triangulation");
    editTriangulation->addAction(m_showTriangulation);
    editTriangulation->addAction(m_chooseTriangulationColor);
    editTriangulation->addAction(m_setTriagulationLineProps);

    QMenu* editVoronoi = m_editMenu->addMenu("Voronoi");
    editVoronoi->addAction(m_showVoronoi);
    editVoronoi->addAction(m_chooseVoronoiColor);
    editVoronoi->addAction(m_setVoronoiLineProps);

    m_helpMenu = menuBar()->addMenu("&Help");
    m_helpMenu->addAction(m_helpAbout);
    m_helpMenu->addAction(m_helpHelp);
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// slots...
void ComGeo::onNew() 
{ 
  // TODO : delete scene and create a new scene and attach to view, redraw view.
    if (nullptr != m_pScene)
        m_pScene->clear();
    clearData();
}



void ComGeo::onExit() 
{ 
    QApplication::quit();
}


/**********************************************************************************************************************
 * Function: 
 *
 * Abstract:
 *
 * Input   :
 *
 * Returns :
 *
 * Written : Aug 2019 (gkhuber)
 *           Nov 2025 - added support for floating point representations of points.
 *********************************************************************************************************************/
void ComGeo::onRandomPtSet() 
{ 
    clearData();

    CNumberEntryDlg    dlg;
    dlg.show();
    if(QDialog::Accepted == dlg.exec())
    {
        int32_t cntPoints = dlg.getNumber();  
        int32_t minX = dlg.minX();
        int32_t minY = dlg.minY();
        int32_t maxX = dlg.maxX();
        int32_t maxY = dlg.maxY();

        m_extents = std::make_tuple(minX, maxX, minY, maxY);

        bool    useReals = dlg.useReals();

        for (int ndx = 0; ndx < cntPoints; ndx++)
        {
            CPoint*    pTemp = new CPoint;
            bool       collision = false;

            if (useReals) pTemp->randomFloatPoint(minX, maxX, minY, maxY);
            else          pTemp->randomIntPoint(minX, maxX, minY, maxY);

         
            for (CPoint* ppt : m_vecPointSet)
            {
              if (*ppt == *pTemp)
              {collision = true;
                break;
              }
            }

            if (!collision)
            {
              m_vecPointSet.push_back(pTemp);
            }
        }

        if (cntPoints != m_vecPointSet.size())
        {
          QMessageBox::information(this, "point set size", QString("failed to generate %1 unique point").arg(cntPoints));
        }

        m_bShowPtSet = true;
        m_showPointSet->setChecked(m_bShowPtSet);
        drawScene();  
    }
}



void ComGeo::onReadPtSet() 
{ 
    QString   qstrFileName = "";
    bool      ReadHeader = false;
    int       dim = 0;
    int       cnt = 0;


    qstrFileName = QFileDialog::getOpenFileName(this, "Open Point Set", ".\\DataSets", "Data files(*.dat)::Text Files(*.txt)::All Files(*.*)");

    if (qstrFileName != "")
    {
        clearData();
        QFile   inFile(qstrFileName);
        if (inFile.open(QIODevice::ReadOnly))
        {
            int          lineNbr = 0;
            QTextStream  in(&inFile);

            while (!in.atEnd())
            {
                ++lineNbr;
                QString    qstrLine = in.readLine();

                removeComments(&qstrLine);                         // remove comments, if any
                qstrLine = qstrLine.simplified();                  // remove leading & trailing whitespace (and reduce internal sequences to a single space)

                if (qstrLine.size() == 0)                          // we have an empty line continue
                {
                    continue;
                }
                else
                {
                    if (-1 != qstrLine.indexOf("header"))                               // look for `header' in line.
                    {
                        ReadHeader = processHeader(&qstrLine, "point", &dim, &cnt);     // flag to denote we've seen the header.
                    }
                    else                                                               // not a header line, error if we've not read the header...
                    {
                        QTextStream  lineIn(&qstrLine);
                        int       ndx;
                        float_t   xval, yval;

                        if (!ReadHeader)
                        {
                            QMessageBox::critical(nullptr, "file format error", "header must prescede all data points");
                            break;
                        }

                        lineIn >> ndx >> xval >> yval;
                        if (QTextStream::Ok == lineIn.status())
                        {
                          if (xval < std::get<0>(m_extents)) std::get<0>(m_extents) = xval;
                          if (std::get<1>(m_extents) < xval) std::get<1>(m_extents) = xval;
                          if (yval < std::get<2>(m_extents)) std::get<2>(m_extents) = yval;
                          if (std::get<3>(m_extents) < yval) std::get<3>(m_extents) = yval;

                          CPoint*    pTemp = new CPoint(ndx, xval, yval);
                          m_vecPointSet.push_back(pTemp);
                        }
                        else
                        {
                            QMessageBox::warning(nullptr, "warning", QString("failed to read line %1").arg(lineNbr));
                        }

                    }
                }
            }

            if (m_vecPointSet.size() != cnt)
            {
                QMessageBox::warning(nullptr, "warning", "failed to read expected points");
            }

            m_bShowPtSet = true;
            m_showPointSet->setChecked(m_bShowPtSet);
            m_ptSize = getDefaultPointSize();
            drawScene();
        }
        else
        {
            QMessageBox::critical(nullptr, "error", inFile.errorString());
        }
    }
}


void ComGeo::onRandomPoly() 
{ qDebug("not yet implemented"); }


void ComGeo::onReadPoly() 
{ 
    QString   qstrFileName = "";
    bool      ReadHeader = false;
    int       dim = 0;
    int       cnt = 0;


    qstrFileName = QFileDialog::getOpenFileName(this, "Open Point Set", ".\\DataSets", "Data files(*.dat)::Text Files(*.txt)::All Files(*.*)");

    if (qstrFileName != "")
    {
        clearData();
        QFile   inFile(qstrFileName);
        if (inFile.open(QIODevice::ReadOnly))
        {
            int          lineNbr = 0;
            QTextStream  in(&inFile);

            while (!in.atEnd())
            {
                ++lineNbr;
                QString    qstrLine = in.readLine();

                removeComments(&qstrLine);                         // remove comments, if any
                qstrLine = qstrLine.simplified();                  // remove leading & trailing whitespace (and reduce internal sequences to a single space)

                if (qstrLine.size() == 0)                          // we have an empty line continue
                {
                    continue;
                }
                else
                {
                    if (-1 != qstrLine.indexOf("header"))                               // look for `header' in line.
                    {
                        ReadHeader = processHeader(&qstrLine, "polygon", &dim, &cnt);   // flag to denote we've seen the header.
                    }
                    else                                                                // not a header line, error if we've not read the header...
                    {
                        QTextStream  lineIn(&qstrLine);
                        int       ndx, xval, yval;

                        if (!ReadHeader)
                        {
                            QMessageBox::critical(nullptr, "file format error", "header must prescede all data points");
                            break;
                        }

                        lineIn >> ndx >> xval >> yval;
                        if (QTextStream::Ok == lineIn.status())
                        {
                            CPoint*    pTemp = new CPoint(ndx, xval, yval);
                            m_vecPointSet.push_back(pTemp);
                        }
                        else
                        {
                            QMessageBox::warning(nullptr, "warning", QString("failed to read line %1").arg(lineNbr));
                        }

                    }
                }
            }

            if (m_vertexList.size() != cnt)
            {
                QMessageBox::warning(nullptr, "warning", "failed to read expected points");
            }


            drawScene();
        }
        else
        {
            QMessageBox::critical(nullptr, "error", inFile.errorString());
        }
    }
}


void ComGeo::onReadPolys()
{


}

void ComGeo::onViewAxis() 
{ 
    m_bDrawAxis = !m_bDrawAxis;
    m_viewAxis->setChecked(m_bDrawAxis);
   
    drawScene();
}


void ComGeo::onViewGrid() 
{  
    m_bDrawGrid = !m_bDrawGrid;
    m_viewGrid->setChecked(m_bDrawGrid);

    drawScene();
}

void ComGeo::onViewDataSet(uint8_t dataType)
{
  dlgViewData dlg(dataType, &m_vecPointSet);
  dlg.show();
  uint32_t ret = dlg.exec();
}



void ComGeo::onViewRefresh()
{
    drawScene();
}


void ComGeo::onAlgoJarvisMarch()
{
  if (m_vecPointSet.size() > 0)
  {
    QVector<edge*> edgeList;
    convexHull  ch(&m_vecPointSet);
    m_vertexList = ch.jervisMarch();
    m_bShowConvexHull = true;
    m_showConvexHull->setChecked(m_bShowConvexHull);
    drawScene();
  }
  else
  {
    QMessageBox::warning(this, "no data", "no point set - please generate a random point set, or read in a point set");
  }
}

void ComGeo::onAlgoGrahamScan()
{
  if (m_vecPointSet.size() > 0)
  {
    QVector<edge*> edgeList;
    convexHull  ch(&m_vecPointSet);
    m_vertexList = ch.grahamScan();
    m_bShowConvexHull = true;
    m_showConvexHull->setChecked(m_bShowConvexHull);
    drawScene();
  }
  else
  {
    QMessageBox::warning(this, "no data", "no point set - please generate a random point set, or read in a point set");
  }
}

void ComGeo::onAlgoMergeHull()
{
  if (m_vecPointSet.size() > 0)
  {
    QVector<edge*> edgeList;
    convexHull  ch(&m_vecPointSet);
    m_vertexList = ch.mergeHull();
    m_bShowConvexHull = true;
    m_showConvexHull->setChecked(m_bShowConvexHull);
    drawScene();
  }
  else
  {
    QMessageBox::warning(this, "no data", "no point set - please generate a random point set, or read in a point set");
  }
}


void ComGeo::onTriangulateIncremental()
{
  if (m_vecPointSet.size() > 0)
  {
    ptSetTriangulation tri(&m_vecPointSet);
    m_triangles = tri.incremental();
    m_bShowTriangulation = true;
    m_showConvexHull->setChecked(m_bShowConvexHull);
    drawScene();
  }
  else
  {
    QMessageBox::warning(this, "no data", "no point set - please generate a random point set, or read in a point set");
  }
}

void ComGeo::onTriangulateGrahmanAlgorithm()
{
  if (m_vecPointSet.size() > 0)
  {
    ptSetTriangulation tri(&m_vecPointSet);
    QVector<triangle> tris = tri.graham();
    m_bShowTriangulation = true;
    m_showTriangulation->setChecked(m_bShowTriangulation);
    drawScene();
  }
  else
  {
    QMessageBox::warning(this, "no data", "no point set - please generate a random point set, or read in a point set");
  }
}

void ComGeo::onTriangulateDivideConquer()
{
  if (m_vecPointSet.size() > 0)
  {
    ptSetTriangulation tri(&m_vecPointSet);
    m_triangles = tri.divideConquer();
    m_bShowTriangulation = true;
    m_showTriangulation->setChecked(m_bShowTriangulation);
    drawScene();
  }
  else
  {
    QMessageBox::warning(this, "no data", "no point set - please generate a random point set, or read in a point set");
  }
}

/**********************************************************************************************************************
 * Function: onAlgoVoronoi
 *
 * Abstract:
 *
 * Input   : void
 *
 * Returns : void
 *
 * Written : Nov 2025 (gkhuber)
 *********************************************************************************************************************/
void ComGeo::onTriangulateDelaunay()
{
  if (m_vecPointSet.size() > 0)
  {
    ptSetTriangulation tri(&m_vecPointSet);
    m_triangles = tri.bw_delaunay();
    m_bShowTriangulation = true;
    m_showTriangulation->setChecked(m_bShowTriangulation);
    drawScene();
  }
  else
  {
    QMessageBox::warning(this, "no data", "no point set - please generate a random point set, or read in a point set");
  }
}



/**********************************************************************************************************************
 * Function: onAlgoVoronoi
 *
 * Abstract:
 *
 * Input   : void 
 *
 * Returns : void 
 *
 * Written : Nov 2025 (gkhuber) 
 *********************************************************************************************************************/
void ComGeo::onAlgoVoronoi()
{
  if (m_vecPointSet.size() > 0)
  {
    voronoi vor(&m_vecPointSet);
    m_cells = vor.voronoiCells();
    m_bShowVoronoi = true;
    m_showVoronoi->setChecked(m_bShowVoronoi);
    drawScene();
  }
  else
  {
    QMessageBox::warning(this, "no data", "no point set - please generate a random point set, or read in a point set");
  }
}

/**********************************************************************************************************************
 * Function: onShowPtSet
 *
 * Abstract:
 *
 * Input   : void
 *
 * Returns : void
 *
 * Written : Dec 2025 (gkhuber)
 *********************************************************************************************************************/
void ComGeo::onShowPtSet() 
{
  m_bShowPtSet = !m_bShowPtSet;
  m_showPointSet->setChecked(m_bShowPtSet);
  drawScene();
}

/**********************************************************************************************************************
 * Function: onChooseColorPtSet
 *
 * Abstract:
 *
 * Input   : void
 *
 * Returns : void
 *
 * Written : Dec 2025 (gkhuber)
 *********************************************************************************************************************/
void ComGeo::onChooseColorPtSet() 
{
  QColor clrInitial = m_ptPen.color();
  QColor clrNew = QColorDialog::getColor(clrInitial, nullptr, QString("Choose color for point set"));
  m_ptPen.setColor(clrNew);
  m_ptBrush.setColor(clrNew);
  drawScene();
}

/**********************************************************************************************************************
 * Function: onSetSizePtSet
 *
 * Abstract:
 *
 * Input   : void
 *
 * Returns : void
 *
 * Written : Dec 2025 (gkhuber)
 *********************************************************************************************************************/
void ComGeo::onSetSizePtSet() 
{
  ptSizeDlg    dlg(m_ptSize);

  uint32_t nret = dlg.exec();

  if (QDialog::Accepted == nret)
  {
    m_ptSize = dlg.size();             // get the new point size
    drawScene();                       // redraw the scene
  }
}

/**********************************************************************************************************************
 * Function: onShowConvexHull
 *
 * Abstract:
 *
 * Input   : void
 *
 * Returns : void
 *
 * Written : Dec 2025 (gkhuber)
 *********************************************************************************************************************/
void ComGeo::onShowConvexHull() 
{
  m_bShowConvexHull = !m_bShowConvexHull;
  m_showConvexHull->setChecked(m_bShowConvexHull);
  drawScene();
}

/**********************************************************************************************************************
 * Function: onChooseColorConvexHull
 *
 * Abstract:
 *
 * Input   : void
 *
 * Returns : void
 *
 * Written : Dec 2025 (gkhuber)
 *********************************************************************************************************************/
void ComGeo::onChooseColorConvexHull() 
{
  QColor clrInitial = m_ptPen.color();
  QColor clrNew = QColorDialog::getColor(clrInitial, nullptr, QString("Choose color for point set"));
  m_chPen.setColor(clrNew);
  //m_ptBrush.setColor(clrNew);
  drawScene();
}

void ComGeo::onSetConvexHullLineProps() {}

/**********************************************************************************************************************
 * Function: onShowTriangulation
 *
 * Abstract:
 *
 * Input   : void
 *
 * Returns : void
 *
 * Written : Dec 2025 (gkhuber)
 *********************************************************************************************************************/
void ComGeo::onShowTriangulation() 
{
  m_bShowTriangulation = !m_bShowTriangulation;
  m_showTriangulation->setChecked(m_bShowTriangulation);
  drawScene();
}

/**********************************************************************************************************************
 * Function: onChooseColorTriangulation
 *
 * Abstract:
 *
 * Input   : void
 *
 * Returns : void
 *
 * Written : Dec 2025 (gkhuber)
 *********************************************************************************************************************/
void ComGeo::onChooseColorTriangulation() 
{
  QColor clrInitial = m_ptPen.color();
  QColor clrNew = QColorDialog::getColor(clrInitial, nullptr, QString("Choose color for point set"));
  m_triPen.setColor(clrNew);
  //m_ptBrush.setColor(clrNew);
  drawScene();
}

void ComGeo::onSetTriangulationLineProps() {}

/**********************************************************************************************************************
 * Function: onChooseColorVoronoi
 *
 * Abstract:
 *
 * Input   : void
 *
 * Returns : void
 *
 * Written : Dec 2025 (gkhuber)
 *********************************************************************************************************************/
void ComGeo::onShowVoronoi()
{
  m_bShowVoronoi = !m_bShowVoronoi;
  m_showVoronoi->setChecked(m_bShowVoronoi);
  drawScene();
}

/**********************************************************************************************************************
 * Function: onChooseColorVoronoi
 *
 * Abstract:
 *
 * Input   : void
 *
 * Returns : void
 *
 * Written : Dec 2025 (gkhuber)
 *********************************************************************************************************************/
void ComGeo::onChooseColorVoronoi() 
{
  QColor clrInitial = m_ptPen.color();
  QColor clrNew = QColorDialog::getColor(clrInitial, nullptr, QString("Choose color for point set"));
  m_vorPen.setColor(clrNew);
  //m_ptBrush.setColor(clrNew);
  drawScene();
}

void ComGeo::onSetVoronoiLineProps() {}

void ComGeo::onAbout() { qDebug("in onAbout"); }
void ComGeo::onHelp() { qDebug("in onHelp"); }



///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// private functions
void ComGeo::clearData()
{
    if (m_vecPointSet.size() > 0)
    {
        for (int ndx = 0; ndx < m_vecPointSet.size(); ndx++)
        {
            delete m_vecPointSet[ndx];
            m_vecPointSet[ndx] = nullptr;
        }

        m_vecPointSet.erase(m_vecPointSet.begin(), m_vecPointSet.end());
    }
        
    if (m_vertexList.size() > 0)
        m_vertexList.erase(m_vertexList.begin(), m_vertexList.end());

    if (m_triangles.size() > 0)
      m_triangles.erase(m_triangles.begin(), m_triangles.end());

    if (m_cells.size() > 0)
    {
      for (seg* s : m_cells)
      {
        delete s;
      }
      m_cells.erase(m_cells.begin(), m_cells.end());
    }
      
    m_pScene->clear();
}




// TODO : need to adjust for aspect ratio so things look correct.
/**********************************************************************************************************************
 * Function: drawScene
 *
 * Abstract:
 *
 * Input   :
 *
 * Returns :
 *
 * Written : (gkhuber)
 * Modified: Nov 2025 add support for convex hull, delaunay, and voronoi algorithsm
 * Modified: Dec 2025 add support for toggling various algorithmic output on/off
 *********************************************************************************************************************/
void ComGeo::drawScene()
{
    static QPen  gridPen(QBrush(Qt::darkGray), 0, Qt::DotLine);
    static QPen  axisPen(QBrush(Qt::black), 0, Qt::SolidLine);
    static QPen  linePen(QBrush(Qt::black), 0, Qt::SolidLine);

    std::cout << "x range: [" << std::get<0>(m_extents) << "," << std::get<1>(m_extents) << "]  y range: [";
    std::cout << std::get<2>(m_extents) << "," << std::get<3>(m_extents) << "]" << std::endl;
    
    if (nullptr != m_pScene)
    {
        m_pScene->clear();                               // prepare to rebuild the scene

        if (m_bDrawAxis)
        {
          float dx = (std::get<1>(m_extents) - std::get<0>(m_extents)) * m_scale;
          float dy = (std::get<3>(m_extents) - std::get<2>(m_extents))* m_scale; 

          uint32_t maxXTicks = m_imageWidth / (2 * m_scale);
          uint32_t maxYTicks = m_imageHeight / (2 * m_scale);

          m_pScene->addLine(0, 0.5 * m_imageHeight, m_imageWidth, 0.5 * m_imageHeight, axisPen);
          //m_pScene->addLine(  0, 0.5 * dy           , dx          , 0.5 * dy           , axisPen);

          for (uint32_t ndx = 1; ndx <= maxXTicks; ndx++)
          {
            m_pScene->addLine(0.5 * m_imageWidth - ndx * m_scale, 0.5 * m_imageHeight - 5, 0.5 * m_imageWidth - ndx * m_scale, 0.5 * m_imageHeight + 5, axisPen);
            m_pScene->addLine(0.5 * m_imageWidth + ndx * m_scale, 0.5 * m_imageHeight - 5, 0.5 * m_imageWidth + ndx * m_scale, 0.5 * m_imageHeight + 5, axisPen);
          }

          m_pScene->addLine(0.5 * m_imageWidth, 0, 0.5 * m_imageWidth, m_imageHeight, axisPen);
          //m_pScene->addLine(0.5 * dx, 0, 0.5 * dx, dy, axisPen);
          for (uint32_t ndx = 1; ndx <= maxYTicks; ndx++)
          {
            m_pScene->addLine(0.5 * m_imageWidth + 5, 0.5 * m_imageHeight - ndx * m_scale, 0.5 * m_imageWidth - 5, 0.5 * m_imageHeight - ndx * m_scale, axisPen);
            m_pScene->addLine(0.5 * m_imageWidth + 5, 0.5 * m_imageHeight + ndx * m_scale, 0.5 * m_imageWidth - 5, 0.5 * m_imageHeight + ndx * m_scale, axisPen);
          }
        }

        if (m_bDrawGrid)
        {
          uint32_t maxXTicks = m_imageWidth / (2 * m_scale);
          uint32_t maxYTicks = m_imageHeight / (2 * m_scale);

          for (uint32_t ndx = 0; ndx <= maxXTicks; ndx += 10)
          {
            m_pScene->addLine(0.0, 0.5 * m_imageHeight - ndx * m_scale, m_imageWidth, 0.5 * m_imageHeight - ndx * m_scale, gridPen);
            m_pScene->addLine(0.0, 0.5 * m_imageHeight + ndx * m_scale, m_imageWidth, 0.5 * m_imageHeight + ndx * m_scale, gridPen);
          }

          for (uint32_t ndx = 0; ndx <= maxYTicks; ndx+=10)
          {
            m_pScene->addLine(0.5 * m_imageWidth - ndx * m_scale, 0, 0.5 * m_imageWidth - ndx * m_scale, m_imageHeight, gridPen);
            m_pScene->addLine(0.5 * m_imageWidth + ndx * m_scale, 0, 0.5 * m_imageWidth + ndx * m_scale, m_imageHeight, gridPen);
          }
        }

        // draw the point set
        if (m_bShowPtSet)
        {
          if (m_vecPointSet.size() > 0)
          {
              QVector<CPoint*>::Iterator     viter;
              viter = m_vecPointSet.begin();

              while (m_vecPointSet.end() != viter)
              {
                  float_t x = ((*viter)->x())* m_scale + 0.5f*m_imageWidth;
                  float_t y = -((*viter)->y())* m_scale + 0.5f* m_imageHeight;     // remember y increases from top right of window.
                  m_pScene->addEllipse((x - m_ptSize), (y - m_ptSize) , (2 * m_ptSize) , (2 * m_ptSize) , m_ptPen, m_ptBrush);

                  viter++;
              }
          }
        }


        // draw the triangulation
        if (m_bShowTriangulation)
        {
          if (m_triangles.size() > 0)
          {
            QVector<triangle>::iterator   viter = m_triangles.begin();

            while (m_triangles.end() != viter)
            {
              for (uint8_t ndx = 0; ndx < 3; ndx++)
              {
                m_pScene->addLine(((*viter).pt(ndx).x())* m_scale + 0.5f * m_imageWidth, (-(*viter).pt(ndx).y()) * m_scale + 0.5f * m_imageHeight,
                                ((*viter).pt((ndx+1)%3).x()) * m_scale + 0.5f * m_imageWidth, (-(*viter).pt((ndx + 1)%3).y()) * m_scale + 0.5f * m_imageHeight,
                                m_triPen);
              }
              viter++;
            }
          }
        }


        // draw the voronoi cells
        if (m_bShowVoronoi)
        {
          if (m_cells.size() > 0)
          {
            for (seg* s : m_cells)
            {
              m_pScene->addLine(s->start.x() * m_scale + 0.5f*m_imageWidth, -s->start.y()*m_scale + 0.5f*m_imageHeight,
                              s->end.x()* m_scale + 0.5f*m_imageWidth, -s->end.y()*m_scale + 0.5f*m_imageHeight,
                              m_vorPen);
            }
          }
        }


        // draw polygon
        if (m_bShowConvexHull)
        {
          int vextexCnt = m_vertexList.size();
          if (vextexCnt > 0)
          {
              for (int ndx = 0; ndx < vextexCnt; ndx++)
              {
                  CPoint ptHead = m_vertexList.at(ndx);
                  CPoint ptTail = m_vertexList.at((ndx + 1)%vextexCnt);

                  m_pScene->addLine(ptHead.x() * m_scale + 0.5* m_imageWidth, -ptHead.y() * m_scale + 0.5* m_imageHeight, ptTail.x() * m_scale + 0.5* m_imageWidth, -ptTail.y() * m_scale + 0.5* m_imageHeight, m_chPen);
              }
          }
        }

        
        m_graphicsView->setScene(m_pScene);
        m_graphicsView->show();
    }
}


float_t ComGeo::getDefaultPointSize()
{
  float_t delta = 0.0500;

  float_t minExtents = std::min((std::get<1>(m_extents) - std::get<0>(m_extents)), (std::get<3>(m_extents) - std::get<2>(m_extents)));

  if (minExtents >= 20) delta = 2.00f;
  else if ((minExtents >= 15) && (minExtents < 20)) delta = 1.0f;
  else if ((minExtents >= 10) && (minExtents < 15)) delta = 0.75f;
  else if ((minExtents >= 5) && (minExtents < 10)) delta = 0.50f;
  else if ((minExtents >= 1) && (minExtents < 5)) delta = 0.10;
  else delta = 0.05;

  return delta;
}
