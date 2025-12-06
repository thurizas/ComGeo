#include "ptSizeDlg.h"

#include <QDialog>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QPushButton>
#include <QLineEdit>
#include <QLabel>


ptSizeDlg::ptSizeDlg(float_t size, QWidget* p) : QDialog(p), m_size(size)
{
  setupUI();


}

void ptSizeDlg::size(float_t size)
{
  if ((size < FLT_EPSILON) || (size > 5))
    return;
  else m_size = size;
}

void ptSizeDlg::setupUI()
{
  if (this->objectName().isEmpty())
    this->setObjectName("sizeDlg");
  this->setWindowTitle("Point Size");

  QVBoxLayout* mainLayout = new QVBoxLayout(this);

  QHBoxLayout* btnLayout = new QHBoxLayout();
  QPushButton* btnOK = new QPushButton("OK");
  btnOK->setDefault(true);
  QPushButton* btnCancel = new QPushButton("Cancel");

  btnLayout->addSpacerItem(new QSpacerItem(150, 16));
  btnLayout->addWidget(btnCancel);
  btnLayout->addWidget(btnOK);

  QHBoxLayout* ctrlLayout = new QHBoxLayout();
  ctrlLayout->setObjectName("sizeMainLayout");

  QLabel* lbl = new QLabel("Enter new point size: ");

  m_edtSize = new QLineEdit();
  m_edtSize->setObjectName("sizeEdtCtrl");
  m_edtSize->setText(QString("%1").arg(m_size));

  ctrlLayout->addWidget(lbl);
  ctrlLayout->addSpacerItem(new QSpacerItem(40, 16));
  ctrlLayout->addWidget(m_edtSize);

  mainLayout->addLayout(ctrlLayout);
  mainLayout->addLayout(btnLayout);

  connect(btnOK, &QPushButton::clicked, this, &ptSizeDlg::onOK);
  connect(btnCancel, &QPushButton::clicked, this, &ptSizeDlg::onCancel);
}


void ptSizeDlg::onOK()
{
  m_size = m_edtSize->text().toFloat();
  QDialog::accept();
}

void ptSizeDlg::onCancel()
{
  QDialog::reject();
}
