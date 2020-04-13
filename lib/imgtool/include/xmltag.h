#ifndef _XMLTAG_H_
#define _XMLTAG_H_

#include <iostream>
#include <string>

class xmltag
{
    std::string Tag;
  public:
    xmltag(std::string tag)
      : Tag(tag)
    { cout << '<' << Tag << '>'; }
    xmltag(std::string tag, std::string param)
      : Tag(tag)
    { cout << '<' << Tag << ' ' << param << '>'; }
    ~xmltag()
    { cout << "</" << Tag << '>'; }
};

#endif // _XMLTAG_H_
