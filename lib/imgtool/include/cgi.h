#ifndef _CGI_H_
#define _CGI_H_

#include <string>
#include <vector>
#include <map>
#include <set>

class CGI
{
    typedef std::string string;
    typedef std::vector<string> stringvector;
    typedef std::set<string> stringset;
    typedef std::map<string, stringvector> stringvectormap;

    stringvector unnamed_params;
    stringvectormap named_params;

  public:
    CGI();

    stringvector::size_type paramcount() const;
    stringvector::size_type paramcount(string name) const;
    string param(stringvector::size_type n) const;
    string param(stringvector::size_type n, string def) const;
    string param(string name) const;
    string param(string name, string def) const;
    stringvector mparam() const;
    stringvector mparam(string name) const;
    stringset paramset() const;
    stringset paramset(string name) const;
    stringvector paramnames() const;

    static string stdheader( string contenttype = "text/html" );
    static string last_modified_header( string contenttype = "text/html",
                                        time_t t = time(0) );

    static string urldecode(string s);
    static string htmldecode(string s);
    static string cgidate(time_t t);
    static string cgidate(int year, int month, int mday,
      int hour=0, int min=0, int sec=0);
    static string cgidate(struct tm* tm);
};

#endif // _CGI_H_
