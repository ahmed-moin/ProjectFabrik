#include <stdlib.h>
#include <iostream>
#include <string>
#include <vector>
#include "gd++.h"
#include "cgi.h"

using namespace std;

const double fontsize = 18;
const string fontname = "/usr/lib/fonts/truetype/LCDN.TTF";
const int minwidth = 120;

class Label : public gd::Image
{
  public:
    Label(string txt, gd::Color fg, gd::Color bg);
};

////////////////////////////////////////////////////////////////////////
Label::Label(string txt, gd::Color fg, gd::Color bg)
{
  gd::BRect bo;

  int x = minwidth, y = 0;
  string::iterator ti = txt.begin();
  vector<string> lines;
  vector<gd::BRect> brects;
  for (string::iterator i = txt.begin(); 1; ++i)
  {
    if (*i == '\n' || i == txt.end())
    {
      string t(ti, i);
      bo = gd::Image::StringFT(fontname, fontsize, t);
      int xx = bo.GetWidth();
      int yy = bo.GetHeight();
      if ( xx > x )
        x = xx;
      y += yy + 4;
      lines.push_back(t);
      brects.push_back(bo);
      ti = i + 1;
    }

    if (i == txt.end())
      break;
  }

  Create(x + 18, y + 4);
  GetColor(bg.red, bg.green, bg.blue);  // backgroundcolor
  gd::Pen pen(this, GetColor(fg.red, fg.green, fg.blue));
  pen.SetFont(fontname, fontsize);

  vector<string>::iterator li;
  vector<gd::BRect>::iterator bi;
  y = -2;
  for (li = lines.begin(), bi = brects.begin();
       li != lines.end() && bi != brects.end();
       ++li, ++bi)
  {
    y += (*bi).GetHeight() + 4;
    pen.MoveTo(x + 9 - (*bi).GetWidth(), y)
       .Write(*li);
  }

}

////////////////////////////////////////////////////////////////////////
string urldecode(string s)
{
  string ret;
  int v;
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
      else if (*c == '+')
        ret += ' ';
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
int main()
{
  try
  {
    CGI q;

    string label = q.param(0);
    Label image(label, gd::Color(255, 255, 255), gd::Color(0, 0, 0));
    cout << CGI::last_modified_header("image/png");
    gdImagePng(image, cout.rdbuf());
  }
  catch(runtime_error e)
  {
    cerr << "runtime_error: " << e.what() << endl;
  }

}
