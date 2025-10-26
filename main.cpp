#include "console.h"
#include "logger.h"
#include "XGetOpt.h"

#include "ComGeo.h"
#include <QtWidgets/QApplication>

int main(int argc, char *argv[])
{
  uint32_t nRet;

  allocConsole();

  QApplication a(argc, argv);


  CLogger* pLogger = CLogger::getInstance();
  pLogger->regOutDevice(0, cmdColorOut);

  ComGeo w;
  w.show();
  nRet = a.exec();

  deallocConsole();
  pLogger->CLogger::delInstance();


  return nRet;
}
