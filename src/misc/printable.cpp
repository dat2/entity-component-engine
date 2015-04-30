#include <misc/printable.hpp>

#include <components/component.hpp>

Printable::Printable(const std::string printName)
  : mPrintName(printName)
{
}

std::ostream& operator<<(std::ostream& os, Printable const& p)
{

  os << p.mPrintName << "(";
  p.print(os);
  os << ")";

  return os;
}


