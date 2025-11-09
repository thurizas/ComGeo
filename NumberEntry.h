
#ifndef _numberentry_h_
#define _numberentry_h_

#include <QDialog>


class QPushButton;
class QLineEdit;
class QCheckBox;

class CNumberEntryDlg : public QDialog
{
    Q_OBJECT

public:
    CNumberEntryDlg(QWidget *parent = nullptr, Qt::WindowFlags f = Qt::WindowFlags());

    void setupUI();

    void    setNumber(int32_t n) { m_nNbr = n; }
    int32_t getNumber() { return m_nNbr; }
    float_t minX() { return m_minX; }
    float_t minY() { return m_minY; }
    float_t maxX() { return m_maxX; }
    float_t maxY() { return m_maxY; }
    bool    useReals();


private slots:
    void OnOK();
    void OnCancel();
    
private:

    QPushButton* m_btnCancel;
    QPushButton* m_btnOK;
    QLineEdit*   m_edtNbrGenerate;
    QLineEdit*   m_xRange;
    QLineEdit*   m_yRange;
    QCheckBox*   m_chkFloat;
    int          m_nNbr;
    float_t      m_minX;
    float_t      m_maxX;
    float_t      m_minY;
    float_t      m_maxY;
    bool         m_useFloat;

    bool parseRangeDescription(QString range, float_t* min, float_t* max);

};


#endif // UI_NBRNODEINPUT_H
