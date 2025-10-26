#include "dlgViewData.h"
#include "ComGeo.h"
#include "logger.h"

#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QSpacerItem>
#include <QPushButton>
#include <QTableWidget> 


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

  QPushButton* btnOK = new QPushButton("OK");
  btnOK->setObjectName("okButton");

  QPushButton* btnCancel = new QPushButton("Cancel");
  btnCancel->setObjectName("cancelButton");
  
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

  connect(btnOK, &QPushButton::clicked, this, &dlgViewData::onAccept);
  connect(btnCancel, &QPushButton::clicked, this, &dlgViewData::onReject);
}

void dlgViewData::onAccept()
{
  QDialog::accept();
}

void dlgViewData::onReject()
{
  QDialog::reject();
}
