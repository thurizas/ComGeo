#include "point.h"

#include <random>

CPoint::CPoint() : m_x(0), m_y(0)
{

}



CPoint::CPoint(float_t x, float_t y) :  m_x(x), m_y(y)
{ }


/**********************************************************************************************************************
 * Function: randomIntPoint
 *
 * Abstract:
 *
 * Input   :
 *
 * Returns :
 *
 * Written : Aug 2019 (gkhuber)
 *           Nov 2025 - name changed to 'randomIntPoint' from 'random'.
 *********************************************************************************************************************/
void CPoint::randomIntPoint(int xmin, int xmax, int ymin, int ymax)
{
    std::random_device                 rand_dev;
    std::mt19937                       generator(rand_dev());
    std::uniform_int_distribution<>   distX(xmin, xmax);
    std::uniform_int_distribution<>   distY(ymin, ymax);

    m_x = distX(generator);
    m_y = distY(generator);
}

void CPoint::randomFloatPoint(float_t xmin, float_t xmax, float_t ymin, float_t ymax)
{
  std::random_device                    rand_dev;
  std::mt19937                          generator(rand_dev());
  std::uniform_real_distribution<>      distX(xmin, xmax);
  std::uniform_real_distribution<>      distY(ymin, ymax);

  m_x = distX(generator);
  m_y = distY(generator);
}

CPoint& CPoint::operator=(const CPoint& rhs)
{
  if (this != &rhs)               // check for self-assignment
  {
    this->m_x = rhs.m_x;
    this->m_y = rhs.m_y;
  }

  return *this;
}

bool CPoint::operator==(const CPoint& other)
{
  bool bRet = false;

  if ((fabs(this->m_x - other.m_x) < FLT_EPSILON) && (fabs(this->m_y - other.m_y) < FLT_EPSILON))
    bRet = true;

  return bRet;
}

bool CPoint::operator==(const CPoint* other)
{
  bool bRet = false;

  if ((fabs(this->m_x - other->m_x) < FLT_EPSILON) && (fabs(this->m_y - other->m_y) < FLT_EPSILON))
    bRet = true;

  return bRet;
}

bool CPoint::operator!=(const CPoint& other)
{
  return !(*this == other);
}

bool CPoint::operator<(const CPoint& other)
{
  bool ret = false;

  if (this->x() < const_cast<CPoint&>(other).x()) ret =  true;
  else
  {
    if (fabs(this->x() - const_cast<CPoint&>(other).x()) < FLT_EPSILON)            // x-coordinates are equal
    {
      if (this->y() < const_cast<CPoint&>(other).y()) ret = true;
    }
  }

  return ret;
}