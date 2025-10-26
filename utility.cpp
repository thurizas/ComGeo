#include "utility.h"

#include <QString>
#include <QTextStream>
#include <QMessageBox>


// comments start at a hash and continue to the end of the line.
void   removeComments(QString* pqstrLine)
{
    int loc;

    if (-1 != (loc = pqstrLine->indexOf('#')))       // find first hash mark...
    {
        int commentLen = pqstrLine->length() - loc;
        *pqstrLine = pqstrLine->left(pqstrLine->length() - commentLen);
    }
    else                                             // no hash found...
    {

    }
}


/*
reads the file header.  This must be the first non-comment line
The header has the format:
    header [type] cnt_items  dimensions
where
header -- key word identifying the type of file
[type] -- either point or polygon,  denotes the type of data
cnt_items -- number of items that follows
dimenaions -- number of dimensions, should be either 2 or 3
*/
bool processHeader(QString* qstrLine, QString qstrType, int* pDim, int* pCnt)
{
    bool       bRet = false;
    QString    keyword;
    QString    type;

    QTextStream  lineIn(qstrLine);

    lineIn >> keyword >> type >> *pCnt >> *pDim;
    if (QTextStream::Ok == lineIn.status())
    {
        if (type == qstrType)            // do we have teh correct type of file
        {
            bRet = true;
        }
        else
        {
            QMessageBox::warning(nullptr, "warning", "file does not contain a point set");
        }
    }
    else
    {
        QMessageBox::critical(nullptr, "error:", "failed to read header line");
    }

    return bRet;
}

//*********************************************************************************************************************
// geometric predicates and primitives





//*********************************************************************************************************************
// Function: orient
//
// Abstract: This determines the orienation between a point (test point) and the line segment from the point origin 
//           (org) to the destination (dest) point.  This evaluate by calculating the value of the followin determinate
// 
//      | 1_x  1_y   1 |   | 1_x         1_y           1 |
//      | 2_x  2_y   1 | = | (2_x - 1_x) (2_y - 1_y)   0 | = (2_x - 1_x) * (3_y - 1_y) - (2_y - 1_y) * (3_x - 1_x)
//      | 3_x  3_y   1 |   | (3_x - 1_x) (3_y - 1_y)   0 |
// 
//           which is determined by subtacting the first row from the second and the third row and then expanding by 
//           cofactors down the third column
//
// Input   : org -- [in] CPoint object denoting the origin of the line segment
//           dst -- [in] CPoint object denoting the destination of the line segment
//           test -- [in] CPoint object denoting the test point to determine the orientation of.
//
// Returns : a signed integer value representing the orientation, +1 denotes the point lies to the left of the line 
//           segment, 0 denote the point lies on the line segment (or its extension), and -1 denotes the point liest to
//           the right.
//
// Written : Oct 2025 (gkhuber)
//*********************************************************************************************************************
int32_t orient(CPoint org, CPoint dst, CPoint test)
{
  uint32_t orient = ERROR;

  float t1 = (dst.x() - org.x()) * (test.y() - org.y());
  float t2 = (dst.y() - org.y()) * (test.x() - org.x());
  float val = t1 - t2;

  if ((org == test) || (dst == test))
  {
    orient = END;
  }
  else if (fabs(val) < FLT_EPSILON)
  {
    orient = ON;
  }
  else if (val > 0) 
  { 
    orient = LEFT; 
  }
  else if (val < 0)
  {
    orient = RIGHT;
  }

  return orient;
}


