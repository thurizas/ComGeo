#ifndef _dlgViewData_h_
#define _dlgViewData_h_

#include <QDialog>

#include "point.h"

class dlgViewData :public QDialog
{
  Q_OBJECT

public:
  dlgViewData(uint8_t dt, QVector<CPoint*>* pVertices, QWidget* parent = nullptr);
  ~dlgViewData();

private slots:
  void onAccept();
  void onReject();

private:
  void setupUI();

  uint8_t  m_dataType;
  uint8_t  m_dimens;
  QVector<CPoint*>* m_pVertices;
};

#endif

