#include "label.h"
#include "cgi.h"
#include "convert.h"
#include "stdfontpen.h"
#include <math.h>
#include <strstream>

using namespace std;

void raute(gd::Pen pen, int w)
{
  gd::Point pos = pen.GetPos();
  for (int i = 0; i < w / 2; ++i)
  {
    pen.MoveTo(pos.x - i, pos.y - w/2 + i)
       .LineTo(pos.x + i, pos.y - w/2 + i)
       .MoveTo(pos.x - i, pos.y + w/2 - i)
       .LineTo(pos.x + i, pos.y + w/2 - i);
  }
  pen.MoveTo(pos.x - w/2, pos.y)
     .LineTo(pos.x + w/2, pos.y)
     .MoveTo(pos);
};

void bombe(gd::Pen pen, int w)
{
  int r = w*3/8;
  gd::Point pos = pen.GetPos();
  int my = pos.y + w/2 - r;
  for(int y = 0; y <= r; ++y)
  {
    int x = (int)sqrt(w*w*9/64 - y*y);
    pen.MoveTo(pos.x - x, my + y)
       .LineTo(pos.x + x, my + y)
       .MoveTo(pos.x - x, my - y)
       .LineTo(pos.x + x, my - y);
  }

  pen.MoveTo(pos.x,     pos.y - w/4)
     .LineTo(pos.x,     pos.y - w/2)
     .LineTo(pos.x + r, pos.y - w*3/8)
     .LineTo(pos.x,     pos.y - w/4);
  if (pen.GetImage().GetWidth() > pos.x && w > 8)  // w/4 > 2
    pen.Fill(pos.x + 1, pos.y - w*3/8);

}

////////////////////////////////////////////////////////////////////////
int main()
{
  try
  {
    CGI q;

    int x = string2<int>(q.param("x"));
    int y = string2<int>(q.param("y", "10"));

    // Position von Milestones
    vector<string> _m = q.mparam("m");
    vector<int> m;
    for(vector<string>::iterator i = _m.begin(); i != _m.end(); ++i)
    {
      int value = string2<int>(*i);
      m.push_back(value);
    }

    // Position von Bomben
    vector<string> _b = q.mparam("b");
    vector<int> b;
    for(vector<string>::iterator i = _b.begin(); i != _b.end(); ++i)
    {
      int value = string2<int>(*i);
      b.push_back(value);
    }

    // Farben von Milestones
    vector<string> _c = q.mparam("c");
    vector<gd::Color> c;
    for(vector<string>::iterator i = _c.begin(); i != _c.end(); ++i)
      c.push_back(gd::Color(*i));

    // Parameterprüfung
    if (m.size() + b.size() != c.size())
    {
      char buffer[100];
      ostrstream msg(buffer, sizeof(buffer));
      msg << "Anzahl der Milestones (" << m.size()
          << ") + Bomben (" << b.size()
          << ") stimmt nicht mit der Anzahl der Farben (" << c.size()
          << ") überein" << ends;
      throw runtime_error(buffer);
    }

    // Hintergrundfarbe
    gd::Color bgcolor = q.param("bg", "ffffff");

    // Balkenfarbe
    gd::Color fgcolor = q.param("fg", "000000");

    // Image erstellen
    gd::Image im(x, y);
    im.GetColor(bgcolor);

    // Balken zeichnen
    gd::Pen p1 = im.GetPen(fgcolor);
    p1.MoveTo(0, 0)
      .LineTo(x-1, 0)
      .LineTo(x-1, y-1)
      .LineTo(0, y-1)
      .LineTo(0, 0);

    if (x > 2 && y > 2)
      p1.Fill(1, 1);

    // Milestones zeichnen
    vector<int>::iterator i;
    vector<gd::Color>::iterator ci = c.begin();
    for(i = m.begin();
        i != m.end() && ci != c.end();
        ++i, ++ci)
    {
      gd::Pen pen = im.GetPen(*ci);
      pen.MoveTo(*i, y/2);
      raute(pen, y);
    }

    // Bomben zeichnen
    for(i = b.begin();
        i != b.end() && ci != c.end();
        ++i, ++ci)
    {
      gd::Pen pen = im.GetPen(*ci);
      pen.MoveTo(*i, y/2);
      bombe(pen, y);
    }

    cout << CGI::last_modified_header("image/png");
    //gdImagePng(im, cout.rdbuf());
    gdImagePng(im, stdout);
  }
  catch(runtime_error e)
  {
    cerr << e.what() << endl;
    return -1;
  }

 return 0;
}
