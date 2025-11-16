#include "vector.h"
#include "point.h"


CVector::CVector(float_t x1, float_t y1, float_t x2, float_t y2) : m_org(CPoint(-1, x1, y1)), m_dst(CPoint(-1, x2,y2))
{  }

CVector::CVector(const CPoint org, const CPoint dst) : m_org(org), m_dst(dst)
{ }

CVector::CVector(const CVector& other)
{
  m_org = other.m_org;
  m_dst = other.m_dst;
}

CVector CVector::operator=(const CVector& other)
{
  if (*this == other)
    return *this;

  CVector ret(other.m_org, other.m_dst);
  return ret;
}

CVector CVector::operator+(const CVector& rhs)
{
  CVector ret(this->m_org.x() + const_cast<CVector&>(rhs).org().x(), this->m_org.y() + const_cast<CVector&>(rhs).org().y(),
              this->m_dst.x() + const_cast<CVector&>(rhs).dst().x(), this->m_dst.y() + const_cast<CVector&>(rhs).dst().y());
  return ret;
}

CVector CVector::operator-(const CVector& rhs)
{
  CVector ret(this->m_org.x() - const_cast<CVector&>(rhs).org().x(), this->m_org.y() - const_cast<CVector&>(rhs).org().y(),
              this->m_dst.x() - const_cast<CVector&>(rhs).dst().x(), this->m_dst.y() - const_cast<CVector&>(rhs).dst().y());
  return ret;
}

CVector CVector::operator*(float_t factor)
{
  CVector ret(m_org.x() * factor, m_org.y() * factor, m_dst.x() * factor, m_dst.y() * factor);
  return ret;
}

float_t CVector::len2()
{
  return ((m_dst.x() - m_org.x()) * (m_dst.x() - m_org.x()) + (m_dst.y() - m_org.y()) * (m_dst.y() - m_org.y()));
}

float_t CVector::len()
{
  return sqrt(len2());
}

float_t CVector::dot(const CVector& rhs)
{
  float res = 0;
  float xs = (m_dst.x() - m_org.x()) * (const_cast<CVector&>(rhs).m_dst.x() - const_cast<CVector&>(rhs).m_org.x());
  float ys = (m_dst.y() - m_org.y()) * (const_cast<CVector&>(rhs).m_dst.y() - const_cast<CVector&>(rhs).m_org.y());
  return xs + ys;
}

float CVector::angleBetween(const CVector& rhs)
{

  float_t temp = this->dot(rhs) / (this->len() * const_cast<CVector&>(rhs).len());
  return acosf(temp);
}

CVector CVector::normalize()
{
  float_t factor = this->len();  
  
  if (!(fabs(factor) < FLT_EPSILON))
  {
    CVector ret(m_org.x()/factor, m_org.y()/factor, m_dst.x()/factor, m_dst.y()/factor);
    return ret;
  }
  
  return *this;
}

/* vector equality (1) same length and (2) parallel*/
bool CVector::operator==(const CVector& rhs)
{
  bool ret = false;
  if (this->len2() == const_cast<CVector&>(rhs).len2())                  // check for equality of length
  {
    float_t temp = this->dot(rhs)/(this->len() * const_cast<CVector&>(rhs).len());
    if (fabs(temp - 1.0) <= FLT_EPSILON)                       // vectors are parallel
    {
      ret = true;
    }
  }
  return ret;
}

bool CVector::operator!=(const CVector& rhs)
{
  return !(*this == rhs);
}

std::ostream& operator<<(std::ostream& os, const CVector& vec)
{
  CVector v1 = const_cast<CVector&>(vec);

  os << "<" << v1.m_dst.x() - v1.m_org.x() << ", " << v1.m_dst.y() - v1.m_org.y() << ">";

  //os << "<(" << v1.m_org.x() << ", " << v1.m_org.y() << "), (" << v1.m_dst.x() << ", " << v1.m_dst.y() << ")>";
  return os;
}
