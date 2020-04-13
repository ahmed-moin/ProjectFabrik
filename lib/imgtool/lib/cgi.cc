
#include "cgi.h"
#include <stdio.h>
#include <stdlib.h>
#include <memory.h>
#include <iterator>
#include <stdexcept>
#include <map>
#include <strstream>

using namespace std;

////////////////////////////////////////////////////////////////////////
CGI::CGI()
{
  char* q = getenv("QUERY_STRING");
  if (!q)
    return;

  string Q = urldecode(q);

  int state = 1;
  string s1, s2;
  for(string::iterator charp = Q.begin(); charp != Q.end(); ++charp)
  {
    switch(state)
    {
      case 1:
        switch(*charp)
        {
          case '=':
            state = 2;
            break;
          case '&':
            unnamed_params.push_back(s1);
            s1.assign(string());
            break;
          default:
            s1.push_back(*charp);
        }
        break;

      case 2:
        switch(*charp)
        {
          case '&':
            named_params[s1].push_back(s2);
            s1.assign(string());
            s2.assign(string());
            state = 1;
            break;
          default:
            s2.push_back(*charp);
        }
        break;
    }
  }
  switch(state)
  {
    case 1:
      if (*q)
        unnamed_params.push_back(s1);
      break;
    case 2:
      named_params[s1].push_back(s2);
      break;
  }
}

////////////////////////////////////////////////////////////////////////
CGI::stringvector::size_type CGI::paramcount() const
{
  return unnamed_params.size();
}

////////////////////////////////////////////////////////////////////////
CGI::stringvector::size_type CGI::paramcount(string name) const
{
  stringvectormap::const_iterator i = named_params.find(name);
  return i == named_params.end() ? 0 : (*i).second.size();
}

////////////////////////////////////////////////////////////////////////
string CGI::param(CGI::stringvector::size_type n) const
{
  if (n >= unnamed_params.size())
  {
    char buffer[80];
    std::ostrstream msg(buffer, sizeof(buffer));
    msg << "Parameter " << n << " nicht vorhanden" << ends;
    throw range_error(buffer);
  }
  return unnamed_params[n];
}

////////////////////////////////////////////////////////////////////////
string CGI::param(CGI::stringvector::size_type n, string def) const
{
  return n >= unnamed_params.size() ? def : unnamed_params[n];
}

////////////////////////////////////////////////////////////////////////
string CGI::param(string name) const
{
  stringvectormap::const_iterator i = named_params.find(name);
  if ( i == named_params.end() )
  {
    char buffer[80];
    std::ostrstream msg(buffer, sizeof(buffer));
    msg << "Parameter " << name << " nicht vorhanden" << ends;
    throw range_error(buffer);
  }
  return (*i).second[0];
}

string CGI::param(string name, string def) const
{
  stringvectormap::const_iterator i = named_params.find(name);
  return i == named_params.end() ? def : (*i).second[0];
}

CGI::stringvector CGI::mparam() const
{
  return unnamed_params;
}

CGI::stringvector CGI::mparam(string name) const
{
  stringvectormap::const_iterator i = named_params.find(name);
  return i == named_params.end() ? stringvector() : (*i).second;
}

CGI::stringset CGI::paramset() const
{
  stringset ret;
  copy(unnamed_params.begin(), unnamed_params.end(),
       inserter(ret, ret.begin()));
  return ret;
}

CGI::stringset CGI::paramset(string name) const
{
  stringset ret;
  stringvectormap::const_iterator i = named_params.find(name);
  if (i != named_params.end())
    copy((*i).second.begin(), (*i).second.end(), inserter(ret, ret.begin()));
  return ret;
}

CGI::stringvector CGI::paramnames() const
{
  stringvector ret;
  for (stringvectormap::const_iterator i = named_params.begin();
       i != named_params.end(); ++i)
    ret.push_back((*i).first);
  return ret;
}

string CGI::stdheader(string contenttype)
{
  string ret = "Content-type: ";
  ret.append(contenttype);
  ret.append("\r\n\r\n");
  return ret;
}

string CGI::last_modified_header(string contenttype, time_t t)
{
  string ret = "Content-type: ";
  ret.append(contenttype);
  ret.append("\r\n");
  ret.append("Last-Modified: ");
  ret.append(CGI::cgidate(t));
  ret.append("\r\n\r\n");
  return ret;
}

////////////////////////////////////////////////////////////////////////
string CGI::urldecode(string s)
{
  string ret;
  int v = 0;
  int cnt = -1;
  for (string::iterator c = s.begin(); c != s.end(); ++c)
  {
    if (cnt == -1)
    {
      if (*c == '%')
      {
        cnt = 0;
        v = 0;
      }
      else
        ret += *c;
    }
    else
    {
      v = (v << 4)
        + ((*c >= 'a' && *c <= 'f') ? (*c - 'a' + 10)
         : (*c >= 'A' && *c <= 'F') ? (*c - 'A' + 10)
         : (*c - '0'));
      if (++cnt >= 2)
      {
        ret.append(1, (char)v);
        cnt = -1;
      }
    }
  }
  return ret;
}

////////////////////////////////////////////////////////////////////////
std::string CGI::htmldecode(string s)
{
  typedef std::map<std::string, std::string> decodemaptype;
  static decodemaptype decodemap;
  if (decodemap.empty())
  {
    decodemap["auml"] = 'ä';
    decodemap["ouml"] = 'ö';
    decodemap["uuml"] = 'ü';
    decodemap["Auml"] = 'Ä';
    decodemap["Ouml"] = 'Ö';
    decodemap["Uuml"] = 'Ü';
    decodemap["szlig"] = 'ß';
  }

  for(std::string::iterator pos = s.begin(); pos != s.end(); ++pos)
  {
    if (*pos == '&')
    {
      std::string::iterator end;
      for (end = pos; end != s.end() && *end != ';'; ++end)
        ;
      if (end != s.end())
      {
        std::string key = std::string(pos + 1, end);
        decodemaptype::iterator i = decodemap.find(key);
        if (i != decodemap.end())
        {
          string::difference_type p = pos - s.begin();
          s.replace(pos, end + 1, (*i).second);
          pos = s.begin() + p;
        }
      }
    }
  }

  return s;
}

////////////////////////////////////////////////////////////////////////
std::string CGI::cgidate(time_t t)
{
  struct tm tm;
  memcpy(&tm, localtime(&t), sizeof(tm));
  return cgidate(&tm);
}

std::string CGI::cgidate(int year, int month, int mday,
  int hour, int min, int sec)
{
  struct tm tm;

  tm.tm_sec = sec;
  tm.tm_min = min;
  tm.tm_hour = hour;
  tm.tm_mday = mday;
  tm.tm_mon = month - 1;
  tm.tm_year = (year >= 1900 ? year - 1900 : year);
  tm.tm_wday = 0;
  tm.tm_yday = 0;
  tm.tm_isdst = 0;

  return cgidate(&tm);
}

std::string CGI::cgidate(struct tm* tm)
{
  char* wday[] = {"Sun", "Mon", "Tue", "Wed", "Thu", "Fri", "Sat"};
  char* monthn[] = {"Jan", "Feb", "Mar", "Apr", "May", "Jun",
                    "Jul", "Aug", "Sep", "Oct", "Nov", "Dec"};
  char buffer[80];

  mktime(tm);
  sprintf(buffer, "%s, %02d %s %d %02d:%02d:%02d GMT",
    wday[tm->tm_wday], tm->tm_mday, monthn[tm->tm_mon], tm->tm_year + 1900,
    tm->tm_hour, tm->tm_min, tm->tm_sec);
  return buffer;
}
