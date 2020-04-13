#include "cgi.h"
#include "label.h"
#include "convert.h"

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

    image.Create(label, 10, 10, 3, 6, 5);
    if (frame.size() > 0 && frame[0] != '0')
      image.DrawFrame(8, 6);

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
