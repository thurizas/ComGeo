#include "logger.h"

#include "ComGeo.h"
#include "point.h"
#include "NumberEntry.h"
#include "utility.h"
#include "2DGeomView.h"
#include "dlgViewData.h"
#include "convexHull.h"
#include "ptSetTriangulation.h"

#include <QGraphicsScene>
#include <QGraphicsItem>
#include <QMainWindow>
#include <QVariant>
#include <QApplication>
#include <QGraphicsView>
#include <QHBoxLayout>
#include <QMainWindow>
#include <QMenuBar>
#include <QStatusBar>
#include <QToolBar>
#include <QWidget>
#include <QDebug>
#include <QFile>
#include <QFileDialog>
#include <QMessageBox>

#include <iostream>


// TODO : implement triangulation algorithms
// TODO : implement voronoi algorithms

ComGeo::ComGeo(uint32_t w, uint32_t h, uint32_t s, QWidget *parent) : QMainWindow(parent), m_pScene(nullptr), m_bDrawAxis(false), m_bDrawGrid(false)
{
  m_imageWidth = w;
  m_imageHeight = h;
  m_scale = s;

  setupUI();
  setupActions();
  setupMenus();

  // build our scene object here... we will attach it to our render when we are ready to draw
  m_pScene = new QGraphicsScene;
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
    m_algoIncremental->setStatusTip("calculate the triangualation by Bowyer-Watson method");
    connect(m_algoIncremental, &QAction::triggered, this, &ComGeo::onBowyerWatson);

    m_algoGraham = new QAction("Graham's Algorithm", this);
    m_algoGraham->setStatusTip("calculate the triangualation by Graham's Algorithm");
    connect(m_algoGraham, &QAction::triggered, this, &ComGeo::onTriangulateGrahmanAlgorithm);


    m_algoDivideConqure = new QAction("Divide and Conqure", this);
    m_algoDivideConqure->setStatusTip("calculate the triangualation by Divide adn Conquer");
    connect(m_algoDivideConqure, &QAction::triggered, this, &ComGeo::onTriangulateDivideConquer);


    m_algoDelaunay = new QAction("Delaunay", this);
    m_algoDelaunay->setStatusTip("calculate the triangualation by Delaunay Algorithm");
    connect(m_algoDelaunay, &QAction::triggered, this, &ComGeo::onTriangulateDelaunay);

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
    QMenu* vcAlgo = m_algoMenu->addMenu("Voronoi Cells");

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

            //clearData();
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
    drawScene();
  }
  else
  {
    QMessageBox::warning(this, "no data", "no point set - please generate a random point set, or read in a point set");
  }
}


void ComGeo::onBowyerWatson()
{
  if (m_vecPointSet.size() > 0)
  {
    ptSetTriangulation tri(&m_vecPointSet);
    m_triangles = tri.BowyerWatson();
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
    ptSetTriangulation tri;
    QVector<triangle> tris = tri.graham();
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
    ptSetTriangulation tri;
    QVector<triangle> tris = tri.divideConquer();
    drawScene();
  }
  else
  {
    QMessageBox::warning(this, "no data", "no point set - please generate a random point set, or read in a point set");
  }
}

void ComGeo::onTriangulateDelaunay()
{
  if (m_vecPointSet.size() > 0)
  {
    ptSetTriangulation tri;
    QVector<triangle> tris = tri.delaunay();
    drawScene();
  }
  else
  {
    QMessageBox::warning(this, "no data", "no point set - please generate a random point set, or read in a point set");
  }
}



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

    m_pScene->clear();
}




// TODO : need to adjust for aspect ratio so things look correct.
void ComGeo::drawScene()
{
    static QPen  gridPen(QBrush(Qt::darkGray), 0, Qt::DotLine);
    static QPen  axisPen(QBrush(Qt::black), 0, Qt::SolidLine);
    static QPen  linePen(QBrush(Qt::black), 0, Qt::SolidLine);
    
    if (nullptr != m_pScene)
    {
        m_pScene->clear();                               // prepare to rebuild the scene

        if (m_bDrawAxis)
        {
          uint32_t maxXTicks = m_imageWidth / (2 * m_scale);
          uint32_t maxYTicks = m_imageHeight / (2 * m_scale);

          m_pScene->addLine(0, 0.5 * m_imageHeight, m_imageWidth, 0.5 * m_imageHeight, axisPen);

          for (uint32_t ndx = 1; ndx <= maxXTicks; ndx++)
          {
            m_pScene->addLine(0.5 * m_imageWidth - ndx * m_scale, 0.5 * m_imageHeight - 5, 0.5 * m_imageWidth - ndx * m_scale, 0.5 * m_imageHeight + 5, axisPen);
            m_pScene->addLine(0.5 * m_imageWidth + ndx * m_scale, 0.5 * m_imageHeight - 5, 0.5 * m_imageWidth + ndx * m_scale, 0.5 * m_imageHeight + 5, axisPen);
          }

          m_pScene->addLine(0.5 * m_imageWidth, 0, 0.5 * m_imageWidth, m_imageHeight, axisPen);
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
        if (m_vecPointSet.size() > 0)
        {
            float   delta = 0.500f;
            QVector<CPoint*>::Iterator     viter;
            viter = m_vecPointSet.begin();

            while (m_vecPointSet.end() != viter)
            {
                float_t x = ((*viter)->x())* m_scale + 0.5f*m_imageWidth;
                float_t y = -((*viter)->y())* m_scale + 0.5f* m_imageHeight;     // remember y increases from top right of window.
                m_pScene->addEllipse((x - delta), (y - delta) , (2 * delta) , (2 * delta) , linePen, QBrush(Qt::SolidPattern));

                viter++;
            }
        }

        // draw the triangulation
        if (m_triangles.size() > 0)
        {
          QVector<triangle>::iterator   viter = m_triangles.begin();

          while (m_triangles.end() != viter)
          {
            for (uint8_t ndx = 0; ndx < 3; ndx++)
            {
              m_pScene->addLine(((*viter).pt(ndx).x())* m_scale + 0.5f * m_imageWidth, (-(*viter).pt(ndx).y()) * m_scale + 0.5f * m_imageHeight,
                                ((*viter).pt((ndx+1)%3).x()) * m_scale + 0.5f * m_imageWidth, (-(*viter).pt((ndx + 1)%3).y()) * m_scale + 0.5f * m_imageHeight,
                                linePen);
            }
            viter++;
          }
        }

        // draw polygon
        int vextexCnt = m_vertexList.size();
        if (vextexCnt > 0)
        {
            for (int ndx = 0; ndx < vextexCnt; ndx++)
            {
                CPoint ptHead = m_vertexList.at(ndx);
                CPoint ptTail = m_vertexList.at((ndx + 1)%vextexCnt);

                m_pScene->addLine(ptHead.x() * m_scale + 0.5* m_imageWidth, -ptHead.y() * m_scale + 0.5* m_imageHeight, ptTail.x() * m_scale + 0.5* m_imageWidth, -ptTail.y() * m_scale + 0.5* m_imageHeight, linePen);
            }
        }
        
        m_graphicsView->setScene(m_pScene);
        m_graphicsView->show();
    }
}
