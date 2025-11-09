#include "NumberEntry.h"

#include <QtCore/QVariant>
#include <QApplication>
#include <QHBoxLayout>
#include <QFormLayout>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QCheckBox>
#include <QWidget>
#include <QValidator>
#include <QMessageBox>

#include <array>




CNumberEntryDlg::CNumberEntryDlg(QWidget *p, Qt::WindowFlags f): QDialog(p,f), m_nNbr(0)
{
  m_minX = m_minY = -1;
  m_maxX = m_maxY = -1;
  m_useFloat = false;

  setupUI();
  m_edtNbrGenerate->setValidator(new QIntValidator(1,500));
  m_chkFloat->setChecked(m_useFloat);
}


void CNumberEntryDlg::setupUI() 
{
  if (objectName().isEmpty())
    setObjectName("Dialog");
  setWindowTitle("Number of Nodes to generate");
  resize(703, 445);

  QFont font;
  font.setFamilies({ "IntelOne Mono" });
  font.setPointSize(11);
  setFont(font);
  
  QHBoxLayout* buttonLayout = new QHBoxLayout();
  buttonLayout->setSpacing(6);
  buttonLayout->setObjectName("horizontalLayout");
  buttonLayout->setContentsMargins(0, 0, 0, 0);

  m_btnCancel = new QPushButton();
  m_btnCancel->setObjectName("cancelButton");
  m_btnCancel->setText("Cancel");

  m_btnOK = new QPushButton();
  m_btnOK->setObjectName("okButton");
  m_btnOK->setText(QCoreApplication::translate("Dialog", "OK", nullptr));

  buttonLayout->addWidget(m_btnCancel);
  buttonLayout->addWidget(m_btnOK);


  QFormLayout* formLayout = new QFormLayout();
  formLayout->setObjectName("formLayout");

  QLabel* label = new QLabel();
  label->setObjectName("label");
  label->setText("Number of nodes to generate:");
  label->setFont(font);

  m_edtNbrGenerate = new QLineEdit();
  m_edtNbrGenerate->setObjectName("lineEdit");
  m_edtNbrGenerate->setText("20");
  m_edtNbrGenerate->setFont(font);

  formLayout->setWidget(0, QFormLayout::LabelRole, label);
  formLayout->setWidget(0, QFormLayout::FieldRole, m_edtNbrGenerate);

  QLabel* label2 = new QLabel();
  label2->setObjectName("label2");
  label2->setFont(font);
  label2->setText("x-coordinate range [-1, 1]  :");

  m_xRange = new QLineEdit();
  m_xRange->setObjectName("xRange");
  m_xRange->setText("[-1, 1]");
  m_xRange->setFont(font);

  formLayout->setWidget(1, QFormLayout::LabelRole, label2);
  formLayout->setWidget(1, QFormLayout::FieldRole, m_xRange);

  QLabel* label3 = new QLabel();
  label3->setObjectName("label3");
  label3->setFont(font);
  label3->setText("y-coordinate range [-1, 1]  : ");

  m_yRange = new QLineEdit();
  m_yRange->setObjectName("lineEdit2");
  m_yRange->setText("[-1, 1]");
  m_yRange->setFont(font);

  formLayout->setWidget(2, QFormLayout::LabelRole, label3);
  formLayout->setWidget(2, QFormLayout::FieldRole, m_yRange);

  QLabel* label4 = new QLabel();
  label4->setObjectName("label4");
  label4->setText("Use Floating point values  :");

  m_chkFloat = new QCheckBox();
  m_chkFloat->setObjectName("checkBox");
  m_chkFloat->setText("");

  formLayout->setWidget(3, QFormLayout::LabelRole, label4);
  formLayout->setWidget(3, QFormLayout::FieldRole, m_chkFloat);

  QVBoxLayout* verticalLayout = new QVBoxLayout(this);
  verticalLayout->setObjectName("verticalLayout");
  verticalLayout->setContentsMargins(10, 10, 10, 10);

  verticalLayout->addLayout(formLayout);
  verticalLayout->addLayout(buttonLayout);

  QObject::connect(m_btnOK, &QPushButton::clicked, this, &CNumberEntryDlg::OnOK);
  QObject::connect(m_btnCancel, &QPushButton::clicked, this, &CNumberEntryDlg::OnCancel);

}


// TODO : parse and validate the controls
void CNumberEntryDlg::OnOK()
{
  bool  parseOK = true;

  m_nNbr = (m_edtNbrGenerate->text()).toInt();
  m_useFloat = m_chkFloat->isChecked();

  parseOK &= parseRangeDescription(m_xRange->text(), &m_minX, &m_maxX);
  parseOK &= parseRangeDescription(m_yRange->text(), &m_minY, &m_maxY);

  if (parseOK) QDialog::accept();
}


void CNumberEntryDlg::OnCancel()
{
  QDialog::reject();
}


bool    CNumberEntryDlg::useReals() 
{ 
  return m_chkFloat->isChecked(); 
}



// range is expected to be in normal range format: [min, max]; (min, max], [min, max) or (min, max)
bool CNumberEntryDlg::parseRangeDescription(QString range, float_t* min, float_t* max)
{
  bool  ret = false;
  bool  isComplete = false;
  int   nValue = 0;                                  // value being read in
  int   nNumPeriods = 0;                             // number of decimal points in number
  int   nLocInNumber = 0;                            // glyph of number we are parsing
  std::array<bool, 2> isOpen = { false, false };
  std::array<QString, 2> values = { QString(""), QString("")};

  range = range.trimmed();                           // remove white space from front.

  try
  {
    char* r = new char[range.size() + 1];
    memset((void*)r, '\0', range.size() + 1);
    strncpy(r, range.toStdString().c_str(), range.size());

    size_t len = range.size();

    for (int32_t ndx = 0; ndx < len; ndx++)
    {
      if ((r[ndx] == '[') || (r[ndx] == '('))
      {
        if(ndx == 0)                                              // [ or ( only allowed at start
          r[ndx] == '[' ? isOpen[0] = false : isOpen[0] = true;
        else
        {
          QString msg = QString("the range designator %1 is malformed").arg(r);
          QMessageBox::warning(this, "malformed range designator", msg);
          break;
        }
      }
      else if ((r[ndx] == ']') || (r[ndx] == ')'))
      {
        r[ndx] == ']' ? isOpen[1] = false : isOpen[1] = true;
        isComplete = true;
      }
      else if (r[ndx] == ',')
      {
        if (nValue != 0)
        {
          QString msg = QString("unexpected comman in %1").arg(r);
          QMessageBox::warning(this, "malformed range designator", msg);
          break;
        }
        else                 // got a ',' move on to next value, reset flags if necessary
        {
          nValue++;
          nNumPeriods = 0;
          nLocInNumber = 0;
        }
      }
      else if (isdigit(r[ndx]) || (r[ndx] == '-') || (r[ndx] == '.'))   
      {

        if ((r[ndx] == '-') && (nLocInNumber != 0))
        {
          QMessageBox::warning(this, "malformed range designator", QString("the range designator %1 is malformed").arg(r) + QString(" \'-\' at an unexpected place"));
          break;
        }

        if (r[ndx] == '.') nNumPeriods++;                               // increment number of decimal points in number 
        if (nNumPeriods > 1)
        {
          QMessageBox::warning(this, "malformed range designator", QString("the range designator %1 is malformed").arg(r) + QString(" too many decimal points"));
          break;
        }


        values[nValue].append(r[ndx]);
        nLocInNumber++;
      }
      else if (isspace(r[ndx]))
      {
        continue;
      }
    }
  
    delete[] r;
  }
  catch (std::bad_alloc& exc)
  {
    QMessageBox::critical(this, "critical error", "insufficient memory to parse range specification");
    // TODO : need a custom exception to throw back to main for so we can gracefully die.
  }

  if (isComplete)
  {
    *min = values[0].toFloat();
    *max = values[1].toFloat();
    qDebug() << "min value is: " << *min << " and max value is: " << *max;
    ret = true;
  }

  return ret;
}

