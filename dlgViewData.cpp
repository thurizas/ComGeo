#include "dlgViewData.h"
#include "ComGeo.h"
#include "logger.h"

#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QSpacerItem>
#include <QPushButton>
#include <QTableWidget> 
#include <QFileDialog>
#include <QDir>
#include <QMessageBox>


dlgViewData::dlgViewData(uint8_t dt, QVector<CPoint*>* vertexList, QWidget* p) : QDialog(p), m_dataType(dt), m_dimens(2), m_pVertices(vertexList)
{
  setupUI();
}


dlgViewData::~dlgViewData()
{

}


void dlgViewData::setupUI()
{
  if (objectName().isEmpty())
    setObjectName("view data");
  resize(400, 300);


  QVBoxLayout* layoutWidget = new QVBoxLayout(this);
  setObjectName("mainLayout");
  setGeometry(QRect(20, 250, 351, 33));


  QHBoxLayout* btnBox = new QHBoxLayout();

  btnBox->setContentsMargins(0, 0, 0, 0);
  btnBox->setObjectName("hboxLayout");
  btnBox->setContentsMargins(0, 0, 0, 0);

  QSpacerItem* spacerItem = new QSpacerItem(131, 31, QSizePolicy::Expanding, QSizePolicy::Minimum);

  QPushButton* btnExport = new QPushButton("Export");
  btnExport->setObjectName("ExportButton");

  QPushButton* btnOK = new QPushButton("OK");
  btnOK->setObjectName("okButton");

  QPushButton* btnCancel = new QPushButton("Cancel");
  btnCancel->setObjectName("cancelButton");
  
  btnBox->addWidget(btnExport);
  btnBox->addItem(spacerItem);
  btnBox->addWidget(btnOK);
  btnBox->addWidget(btnCancel);

  QTableWidget* tblData = new QTableWidget();
  tblData->setObjectName("dataSet");
  
  //TODO : adjust table dimensions and column headers based on type of data being displayed.
  QStringList headers;
  if (m_dataType == ComGeo::DATA::POINT)
  {
    tblData->setColumnCount(m_dimens);
    headers << "x-coord " << "y-coord";
    if(m_dimens == 3) headers << "z-coord";
    tblData->setHorizontalHeaderLabels(headers);
    uint32_t row = 0;                               // current row in the table
    if (m_pVertices->size() > 0)
    {
      for (CPoint* pPoint : *m_pVertices)
      {
        tblData->insertRow(row);
        tblData->setItem(row, 0, new QTableWidgetItem(QString("%1").arg(pPoint->x())));
        tblData->setItem(row, 1, new QTableWidgetItem(QString("%1").arg(pPoint->y())));
      }
    }
  }
  else if (m_dataType == ComGeo::DATA::LINE)
  {

  }
  else if (m_dataType == ComGeo::DATA::POLYGON)
  {

  }
  else
  {
    CLogger::getInstance()->outMsg(cmdLine, CLogger::level::WARNING, "Unknown type %d", static_cast<uint32_t>(m_dataType));
  }

  layoutWidget->addWidget(tblData);
  layoutWidget->addLayout(btnBox);

  connect(btnExport, &QPushButton::clicked, this, &dlgViewData::onExport);
  connect(btnOK, &QPushButton::clicked, this, &dlgViewData::onAccept);
  connect(btnCancel, &QPushButton::clicked, this, &dlgViewData::onReject);
}

void dlgViewData::onExport()
{
  QDate curDate = QDate::currentDate();


  // TODO : get the file name
  QString outFileName = QFileDialog::getSaveFileName(nullptr, "Export file name", QDir::currentPath(), "Data files (*.dat);;All Files (*.*)", new QString("Data file (*.dat)"));
  
  // TODO : open/create the file name
  QFile outFile(outFileName, nullptr);
  if (!outFile.open(QIODevice::ReadWrite | QIODevice::Text | QIODevice::Truncate))
  {
    QString errStr = outFile.errorString();
    QMessageBox::warning(this, "IO Error", QString("could not open %1 for writting\nError %2").arg(outFileName).arg(errStr));
  }
  else
  {
    QTextStream out(&outFile);// TODO : setup an input stream
 
    switch (m_dataType)
    {
      case ComGeo::DATA::POINT:

        out << "#\n";
        out << QString("# exported point set, exported on %1%2%3\n").arg(curDate.year()).arg(curDate.month()).arg(curDate.day());;
        out << QString("# %1 point-set \n").arg(m_pVertices->size());
        out << "\n";
        out << "header point " << m_pVertices->size() << " 2\n";  

        exportPointSet(&out);

        break;
      case ComGeo::DATA::LINE:

        break;

      case ComGeo::DATA::POLYGON:

        break;

      default:
        CLogger::getInstance()->outMsg(cmdLine, CLogger::level::WARNING, "%d is an unknown data type", m_dataType);
    } 
    
    outFile.close();
  }
}

void dlgViewData::onAccept()
{
  QDialog::accept();
}

void dlgViewData::onReject()
{
  QDialog::reject();
}

/*
 #
 # file for point set 1, from  Computational Geometry in C, pg 85
 #
 # this point set is for a 2D-point space columns are index, x-coordinate
 # and the ny-coordinate.  First line gives number of points and dimensions
header point 19  2
 0  3  3     # first point
*/
void dlgViewData::exportPointSet(QTextStream* pout)
{
  uint32_t ndx = 0;
  for(CPoint* ppt : *m_pVertices)
  { 
    *pout << ndx << " " << ppt->x() << " " << ppt->y() << "\n";
    ndx++;
  }

}
