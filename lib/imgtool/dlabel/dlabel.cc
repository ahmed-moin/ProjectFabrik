#include "label.h"
#include "cgi.h"
#include "convert.h"
#include "stdfontpen.h"

using namespace std;

////////////////////////////////////////////////////////////////////////
int main()
{
  try
  {
    CGI q;

    string label;
    label = q.paramcount() > 0 ? q.param(0) : q.param("text");

    gd::Color fontcolor = q.param("fc", "000");
    gd::Color bgcolor   = q.param("bg", "FFFFFF");
    string frame     = q.param("f", "0");
    int shift        = string2<int>(q.param("s", "0"));

    Label image;
    image.SetBgcolor(bgcolor);
    image.SetFontcolor(fontcolor);
    image.SetShift(shift);

    image.Create(label, 20, 10, 4, 4, 5);
    if (frame.size() > 0 && frame[0] != '0')
      image.DrawFrame(8, 6);

    gd::Pen BluePen = image.GetPen(0, 0, 128);
    int y = 3 + gd::Stdfontpen::GetStringSize("X").GetHeight() / 2;
    BluePen.Rectangle(12, y - 1, 14, y + 1);
    BluePen.Fill(13, y);

    cout << CGI::last_modified_header("image/png");
    gdImagePng(image, stdout);
  }
  catch(runtime_error e)
  {
    cerr << e.what() << endl;
    return -1;
  }

 return 0;
}
