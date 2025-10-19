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


