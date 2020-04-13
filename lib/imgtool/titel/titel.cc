#include "gd++.h"
#include "cgi.h"
#include "convert.h"

using namespace std;

////////////////////////////////////////////////////////////////////////
int main()
{
  try
  {
    char* fontfile1 = getenv("FONTFILE1");
    if (!fontfile1)
      throw runtime_error("FONTFILE1 nicht angegeben");

    char* fontfile2 = getenv("FONTFILE1");
    if (!fontfile2)
      throw runtime_error("FONTFILE2 nicht angegeben");

    char* fontsize = getenv("FONTSIZE1");
    if (!fontsize)
      throw runtime_error("FONTSIZE1 nicht angegeben");
    int fontsize1 = string2<int>(fontsize);

    fontsize = getenv("FONTSIZE2");
    if (!fontsize)
      throw runtime_error("FONTSIZE2 nicht angegeben");
    int fontsize2 = string2<int>(fontsize);

    CGI q;

    string titel = q.param(0);
    string untertitel = q.param(1, "");

    gd::Image image(500, 50);
    image.GetColor(0, 0, 0x80);
    int c = image.GetColor(0xff, 0xff, 0xff);

    gd::Pen pen(image, c);

    pen.SetFont(fontfile1)
       .SetFontSize(fontsize1)
       .MoveTo(8, 8)
       .WriteAlign(gd::Pen::LEFT|gd::Pen::TOP, titel);

    if (untertitel.size() > 0)
       pen.SetFont(fontfile2)
          .SetFontSize(fontsize2)
          .MoveTo(20, 32)
          .WriteAlign(gd::Pen::LEFT|gd::Pen::TOP, untertitel);

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
