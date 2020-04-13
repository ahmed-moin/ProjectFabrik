
#ifndef _CONVERT_H_
#define _CONVERT_H_

#include <strstream>
#include <stdexcept>

template <class T>
std::string to_string(T v)
{
  char buffer[80];
  std::ostrstream o(buffer, sizeof(buffer));
  o << v << std::ends;
  return buffer;
}

template <class T>
T string2(std::string s)
{
  T ret = 0;
  char buffer[20];
  s.append(1, '\0');
  s.copy(buffer, 20);
  if (!(std::istrstream(buffer, 20) >> ret))
    throw std::runtime_error("formatfehler");
  return ret;
}

#endif  // _CONVERT_H_
