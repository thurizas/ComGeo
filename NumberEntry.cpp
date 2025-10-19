#include "NumberEntry.h"

#include <QtCore/QVariant>
#include <QApplication>
#include <QHBoxLayout>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QWidget>
#include <QValidator>




CNumberEntryDlg::CNumberEntryDlg(QWidget *p, Qt::WindowFlags f): QDialog(p,f), m_nNbr(0)
{
    setupUI();
    m_edtEntry->setValidator(new QIntValidator(1,500));
}



void CNumberEntryDlg::setupUI() 
{
    QWidget*      layoutWidget;
    QHBoxLayout* horizontalLayout;
    QLabel *label;

    QFont font;
    font.setFamily(QString::fromUtf8("Courier New"));
    font.setPointSize(10);
    

    if (this->objectName().isEmpty())
        this->setObjectName(QString::fromUtf8("Dialog"));
    this->resize(390, 87);
    this->setWindowTitle("Number of Nodes to generate");

    layoutWidget = new QWidget(this);
    layoutWidget->setObjectName(QString::fromUtf8("layoutWidget"));
    layoutWidget->setGeometry(QRect(220, 50, 158, 25));
    
    horizontalLayout = new QHBoxLayout(layoutWidget);
    horizontalLayout->setSpacing(6);
    horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
    horizontalLayout->setContentsMargins(0, 0, 0, 0);
    
    m_btnCancel = new QPushButton(layoutWidget);
    m_btnCancel->setObjectName(QString::fromUtf8("cancelButton"));
    m_btnCancel->setText("Cancel");
    m_btnCancel->setDefault(false);

    m_btnOK = new QPushButton(layoutWidget);
    m_btnOK->setObjectName(QString::fromUtf8("okButton"));
    m_btnOK->setText("OK");
    m_btnOK->setDefault(true);

    horizontalLayout->addWidget(m_btnCancel);
    horizontalLayout->addWidget(m_btnOK);

    label = new QLabel(this);
    label->setObjectName(QString::fromUtf8("label"));
    label->setGeometry(QRect(20, 20, 231, 21));
    label->setText("Number of nodes to generate:");
    label->setFont(font);

    m_edtEntry = new QLineEdit(this);
    m_edtEntry->setObjectName(QString::fromUtf8("lineEdit"));
    m_edtEntry->setGeometry(QRect(260, 20, 113, 20));

    QObject::connect(m_btnOK, SIGNAL(clicked()), this, SLOT(OnOK()));
    QObject::connect(m_btnCancel, SIGNAL(clicked()), this, SLOT(OnCancel()));

    QMetaObject::connectSlotsByName(this);
} 



void CNumberEntryDlg::OnOK()
{
    m_nNbr = (m_edtEntry->text()).toInt();

    QDialog::accept();
}


void CNumberEntryDlg::OnCancel()
{
    QDialog::reject();
}


