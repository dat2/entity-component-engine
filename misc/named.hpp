#ifndef _NAMED_HPP
#define _NAMED_HPP

#include <string>

class Named
{
public:
  Named(const std::string name);

  const std::string& getName() const;

  friend bool operator==(const Named& a, const Named& b);

private:
  const std::string mName;
};


namespace std
{
  template <> struct hash<Named>
  {
    size_t operator()(const Named& e) const
    {
      /* eg. return hash<int>()(x.value); */
      return hash<std::string>()(e.getName());
    }
  };
}


#endif
