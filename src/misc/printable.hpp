#ifndef _PRINTABLE_HPP
#define _PRINTABLE_HPP

// libraries
#include <algorithm>
#include <iostream>
#include <string>
#include <vector>

class Printable
{
public:
  Printable(const std::string printName);

  friend std::ostream& operator<<(std::ostream& os, Printable const& p);
  virtual void print(std::ostream& where) const = 0;
private:
  const std::string mPrintName;
};

template <class T>
void printField(std::ostream& os, const std::string& fieldName, T&& object)
{
  os << fieldName << "=" << object;
};

template <class T>
void printVector(std::ostream& os, const std::string& fieldName, const std::vector<T>& objects)
{
  os << fieldName << "=(";
  if(objects.size())
  {
    std::for_each(std::begin(objects), std::end(objects) - 1,
      [&os](const T& t) { os << t << ","; });
    os << objects.back();
  }
  os << ")";
}

template <class T, typename Func>
void printVector(std::ostream& os, const std::string& fieldName, const std::vector<T>& objects, Func transform)
{
  std::vector< std::string > results;

  for( auto &t : objects )
  {
    results.push_back(transform(t));
  }

  printVector(os, fieldName, results);
}


#endif
