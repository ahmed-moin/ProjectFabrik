#include "gd++.h"
#include "cgi.h"
#include "convert.h"

using namespace std;

////////////////////////////////////////////////////////////////////////
int main()
{
  try
  {
    CGI q;

    int y, x1, x2;

    y  = string2<int>(q.param("y"));
    x1 = string2<int>(q.param(0));
    x2 = string2<int>(q.param(1));
    gd::Color color1 = q.param("c1");
    gd::Color color2 = q.param("c2");

    int imagewidth = max(x1, x2) + 1;
    gd::Image image(imagewidth, y*3/2+1);
    image.GetColor(255, 255, 255);
    int c1 = image.GetColor(color1);
    int c2 = image.GetColor(color2);
    gd::Pen p1 = image.GetPen(c1);
    gd::Pen p2 = image.GetPen(c2);

    int y1 = y/2;
    int y2 = y*3/2;
    int xmin = min(x1, x2);

    imagewidth -= 1;
    if (xmin > 0)
    {
      p1.MoveTo(imagewidth, 0)
        .LineTo(imagewidth - xmin, 0)
        .LineTo(imagewidth - xmin, y1)
        .LineTo(imagewidth, y1)
        .LineTo(imagewidth, 0)
        .Fill(imagewidth - 1, 1);
      p2.MoveTo(imagewidth, y1+1)
        .LineTo(imagewidth - xmin, y1+1)
        .LineTo(imagewidth - xmin, y2)
        .LineTo(imagewidth, y2)
        .LineTo(imagewidth, y1+1)
        .Fill(imagewidth - 1, y1+2);
    }

    if (x1 > x2)
      p1.MoveTo(imagewidth - x2-1, 0)
        .LineTo(imagewidth - x1, 0)
        .LineTo(imagewidth - x1, y)
        .LineTo(imagewidth - x2-1, y)
        .LineTo(imagewidth - x2-1, 0)
        .Fill(imagewidth - x2-2, 1);
    else if (x2 > x1)
      p2.MoveTo(imagewidth - x1-1, y1+1)
        .LineTo(imagewidth - x2, y1+1)
        .LineTo(imagewidth - x2, y2)
        .LineTo(imagewidth - x1-1, y2)
        .LineTo(imagewidth - x1-1, y1+1)
        .Fill(1, y1+2);

    cout << CGI::last_modified_header("image/png");
    //gdImagePng(image, cout.rdbuf());
    gdImagePng(image, stdout);
  }
  catch(runtime_error e)
  {
    cerr << e.what() << endl;
    return -1;
  }

  return 0;
}
