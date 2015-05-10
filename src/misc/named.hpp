#ifndef _NAMED_HPP
#define _NAMED_HPP

// libraries
#include <string>

// engine

class Named
{
public:
  Named(const std::string name);

  const std::string& getName() const;

  friend bool operator==(const Named& a, const Named& b);

private:
  const std::string mName;
};

#endif
