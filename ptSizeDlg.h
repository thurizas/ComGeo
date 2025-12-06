#ifndef _ptsizedlg_h_
#define _ptsizedlg_h_

#include <QDialog>
#include <QLineEdit>

class ptSizeDlg : public QDialog
{
  Q_OBJECT

public:

  ptSizeDlg(float_t, QWidget* parent = nullptr);

  float_t size() { return m_size; }
  void size(float_t);

public slots:
  void onOK();
  void onCancel();

private:
  float_t     m_size;                    // currently used point size

  QLineEdit* m_edtSize = nullptr;

  void setupUI();
};

#endif

