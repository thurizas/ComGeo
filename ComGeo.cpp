#include "ComGeo.h"
#include "point.h"

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



ComGeo::ComGeo(QWidget *parent) : QMainWindow(parent), m_pScene(nullptr), m_bDrawAxis(false), m_bDrawGrid(false)
{
    //ui.setupUi(this);
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

    m_graphicsView = new QGraphicsView(centralWidget);
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

    //m_dataRandomPoly = new QAction("Read Pt set", this);
    //m_dataRandomPoly->setStatusTip("generate random polygon");
    //connect(m_dataRandomPoly, &QAction::triggered, this, &ComGeo::onRandomPoly);

    m_dataReadPoly = new QAction("Read Pt set", this);
    m_dataReadPoly->setStatusTip("generate random polygon");
    connect(m_dataReadPoly, &QAction::triggered, this, &ComGeo::onReadPoly);

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

    m_viewRefresh = new QAction("refresh", this);
    m_viewRefresh->setStatusTip("Refresh the view");
    connect(m_viewRefresh, &QAction::triggered, this, &ComGeo::onViewRefresh);

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

    //m_dataMenu->addSeparator();
    //m_editMenu->addAction(m_editStep);
    //m_editMenu->addSeparator();
    //m_editMenu->addAction(m_editProps);

    m_viewMenu = menuBar()->addMenu("View");
    m_viewMenu->addAction(m_viewAxis);
    m_viewMenu->addAction(m_viewGrid);
    m_viewMenu->addSeparator();
    m_viewMenu->addAction(m_viewRefresh);

    m_helpMenu = menuBar()->addMenu("&Help");
    m_helpMenu->addAction(m_helpAbout);
    m_helpMenu->addAction(m_helpHelp);
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// slots...
void ComGeo::onNew() 
{ 
    if (nullptr != m_pScene)
        m_pScene->clear();
    clearData();
}



void ComGeo::onExit() 
{ 
    QApplication::quit();
}



void ComGeo::onRandomPtSet() 
{ 
    clearData();

    int cntPoints = 10;                // TODO: this should be configurable

    //for (int ndx = 0; ndx < cntPoints; ndx++)
    //{
    //    CPoint*   pTemp = new CPoint(10 * ndx, 10 * ndx);
    //    m_vecPointSet.push_back(pTemp);
    //}


    for (int ndx = 0; ndx < cntPoints; ndx++)
    {
        CPoint*    pTemp = new CPoint;
        pTemp->random(m_graphicsView->width(), m_graphicsView->height());

        qDebug("adding (%d, %d) to vector", pTemp->getX(), pTemp->getY());

        m_vecPointSet.push_back(pTemp);
    }

    drawScene();  
}
void ComGeo::onReadPtSet() { qDebug("in onReadPtSet"); }
void ComGeo::onRandomPoly() { qDebug("in onRandomPoly"); }
void ComGeo::onReadPoly() { qDebug("in onReadPoly"); }

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



void ComGeo::onViewRefresh()
{
    drawScene();
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
}


// TODO : need to adjust for aspect ratio so things look correct.
void ComGeo::drawScene()
{
    static QPen  gridPen(QBrush(Qt::lightGray), 1, Qt::DotLine);
    static QPen  axisPen(QBrush(Qt::black), 2, Qt::SolidLine);

    int windowWidth = m_graphicsView->width();
    int windowHeigth = m_graphicsView->height();
    

    float step =  20.0;      // TODO : step size should be configurable...

    if (nullptr != m_pScene)
    {
        m_pScene->clear();                               // prepare to rebuild the scene

        if (m_bDrawAxis)
        {
            m_pScene->addLine(0, 0.5*windowHeigth, windowWidth, 0.5*windowHeigth, axisPen);    // draw x-axis ...
            m_pScene->addLine(0.5*windowWidth, 0, 0.5*windowWidth, windowHeigth, axisPen);     // draw y-axis ...
        }

        if (m_bDrawGrid)
        {
            for (int ndx = 0; ndx <= 20; ndx++)
            {
                    m_pScene->addLine(0, (ndx / step)*windowHeigth, windowWidth, (ndx / step)*windowHeigth, gridPen);
                    m_pScene->addLine((ndx / step)*windowWidth, 0, (ndx / step)*windowWidth, windowHeigth, gridPen);
             
            }
        }

        // draw the point set
        if (m_vecPointSet.size() > 0)
        {
            QVector<CPoint*>::Iterator     viter;
            viter = m_vecPointSet.begin();

            while (m_vecPointSet.end() != viter)
            {
                int x = (*viter)->getX() + 0.5*windowWidth;
                int y = -(*viter)->getY() + 0.5*windowHeigth;     // remember y increases from top right of window.

                m_pScene->addEllipse(QRect(QPoint(x - 1, y - 1), QPoint(x + 1, y + 1)));

                viter++;
            }
        }
        
        m_graphicsView->setScene(m_pScene);
        m_graphicsView->show();
    }
}
