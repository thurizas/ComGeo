/********************************************************************************
** Form generated from reading UI file 'NbrNodeInput.ui'
**
** Created by: Qt User Interface Compiler version 5.12.3
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_NBRNODEINPUT_H
#define UI_NBRNODEINPUT_H

#include <QDialog>

class QPushButton;
class QLineEdit;

class CNumberEntryDlg : public QDialog
{
    Q_OBJECT

public:
    CNumberEntryDlg(QWidget *parent = nullptr, Qt::WindowFlags f = Qt::WindowFlags());

    void setupUI();

    void setNumber(int n) { m_nNbr = n; }
    int  getNumber() { return m_nNbr; }

private slots:
    void OnOK();
    void OnCancel();

private:

    QPushButton* m_btnCancel;
    QPushButton* m_btnOK;
    QLineEdit*   m_edtEntry;

    int           m_nNbr;

};


#endif // UI_NBRNODEINPUT_H
