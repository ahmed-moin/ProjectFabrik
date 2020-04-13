#include <strstream>
#include "gd++.h"
#include "cgi.h"
#include "stdfontpen.h"
#include "convert.h"

using namespace std;

int main()
{
  try
  {
    CGI q;

    int width      = string2<int>(q.param(0));
    float start    = string2<float>(q.param(1));
    string end_str = q.param(2);
    float end      = string2<float>(q.param(2));
    float step_n   = string2<float>(q.param(3));
    float step_l   = string2<float>(q.param(4, q.param(3)));
    string unit    = q.param(5, "");
    bool arrow     = string2<int>(q.param("arrow", "1"));

    // Parameterprüfung
    if ( start > end && (step_n > 0 || step_l > 0)
      || start < end && (step_n < 0 || step_l < 0) )
      throw range_error("Skalierungsfehler");

    if (width > 2000)
      throw range_error("width > 2000");

    // Breite des Images = geforderte Breite + Breite der Einheit
    //   + halbe Breite den endlabels + 10
    // Höhe des Images = 1,5 * Labelhöhe + 12
    gd::BRect end_rect = gd::Stdfontpen::GetStringSize(end_str);
    gd::BRect unit_rect = gd::Stdfontpen::GetStringSize(unit);
    int imagewidth = width + unit_rect.GetWidth()
                    + end_rect.GetWidth() / 2
                    + 10;
    gd::Image image(imagewidth, end_rect.GetHeight() * 3 / 2 + 12);
    image.GetColor(255, 255, 255);
    gd::Image::color_type black = image.GetColor(0, 0, 0);
    gd::Stdfontpen pen(image, black);

    // Pfeil
    if (arrow)
    {
      pen.Line(0, 4, imagewidth - 1, 4);
      pen.Line(imagewidth - 1, 4, imagewidth - 5, 0);
      pen.Line(imagewidth - 1, 4, imagewidth - 5, 8);
    }
    else
    {
      pen.Line(0, 4, width, 4);
    }

    // Hilfsvariable
    int step_x, x;
    float n;

    // Teilstriche
    step_x = (int)(width * step_l / (end - start));
    for (x = 0; x <= width; x += step_x)
      pen.Line(x, 0, x, 8);

    // Beschriftung
    step_x = (int)(width * step_n / (end - start));
    int align = gd::Pen::LEFT | gd::Pen::TOP;
    for (x = 0, n = start;
         start > end ? (n > end + step_n) : (end + step_n > n);
         x += step_x, n += step_n)
    {
      pen.MoveTo(x, 11)
         .WriteAlign(align, to_string(n));

      align = gd::Pen::HCENTER | gd::Pen::TOP;
    }

    pen.MoveTo(imagewidth - 1, 11 + end_rect.GetHeight() / 2)
     .WriteAlign(gd::Pen::RIGHT | gd::Pen::TOP, unit);

    // Ausgabe
    cout << q.last_modified_header("image/png");
    //gdImagePng(image, cout.rdbuf());
    gdImagePng(image, stdout);
    return 0;
  }
  catch(range_error e)
  {
    cerr << "range_error: " << e.what() << endl;
  }
  catch(runtime_error e)
  {
    cerr << "runtime_error: " << e.what() << endl;
  }
}
