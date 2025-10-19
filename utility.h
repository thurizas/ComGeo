#pragma once

class QString;

void removeComments(QString* pqstrLine);
bool processHeader(QString* qstrLine, QString qstrType, int* pDim, int* pCnt);
