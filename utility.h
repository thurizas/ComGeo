#ifndef _utility_h_
#define _utility_h_

#include "point.h"

#include <cstdint>

class QString;

enum orien {ERROR=-2, RIGHT=-1, ON=0, LEFT=1, END=2};
enum winding { UNK = -1, CW=0, CCW=1 };

void removeComments(QString* pqstrLine);
bool processHeader(QString* qstrLine, QString qstrType, int* pDim, int* pCnt);
int32_t orient(CPoint org, CPoint dest, CPoint test);

#endif
