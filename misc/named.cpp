#include <misc/named.hpp>

Named::Named(const std::string name)
  : mName(name)
{
}

const std::string& Named::getName() const
{
  return mName;
}


bool operator==(const Named& a, const Named& b)
{
  return a.mName == b.mName;
}
