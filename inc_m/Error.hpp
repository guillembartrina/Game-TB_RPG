#ifndef ERROR_HPP
#define ERROR_HPP

#include <string>

class Error
{
public:

  Error(std::string info)
  {
    _info = info;
  }

  std::string info()
  {
    return ("ERROR: " + _info);
  }

private:

  std::string _info;
};

#endif