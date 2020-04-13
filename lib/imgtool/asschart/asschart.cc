#include <iostream>
#include "cgi.h"
#include "convert.h"
#include "gd++.h"
#include "stdfontpen.h"

using namespace std;

const int height = 200;
const int width = 540;

class XPen : public gd::Pen
{
  public:
    XPen(const gd::Pen& p)
      : gd::Pen(p)
      { }
    void quadrat(int w = 4);
    void raute(int w = 5);
};

void XPen::quadrat(int w)
{
  gd::Point pos = GetPos();
  for (int y = pos.y - w; y <= pos.y + w; ++y)
  {
    MoveTo(pos.x - w, y);
    LineTo(pos.x + w, y);
  }
  MoveTo(pos);
};

void XPen::raute(int w)
{
  gd::Point pos = GetPos();
  int i;
  for (i = 0; i < w; ++i)
  {
    MoveTo(pos.x - i, pos.y - w + i);
    LineTo(pos.x + i, pos.y - w + i);
    MoveTo(pos.x - i, pos.y + w - i);
    LineTo(pos.x + i, pos.y + w - i);
  }
  MoveTo(pos.x - w, pos.y);
  LineTo(pos.x + w, pos.y);
  MoveTo(pos);
};

// Hauptprogramm
int main()
{
  try
  {
    // Auswertung der CGI-Parameter
    CGI q;
    int i, x, y;

    int wert[2][5];
    for (i = 0; i < 5; ++i)
    {
      wert[0][i] = string2<int>(q.param(i));
      if ( wert[0][i] < 0 || wert[0][i] > 100 )
        throw runtime_error("out of range");

      wert[1][i] = string2<int>(q.param(i + 5));
      if ( wert[1][i] < 0 || wert[1][i] > 100 )
        throw runtime_error("out of range");
    }

    // Farben aus Umgebungsvariablen ermitteln
    char* BGCOLOR = getenv("BGCOLOR");
    char* COLOR1 = getenv("COLOR1");
    char* COLOR2 = getenv("COLOR2");

    if (!BGCOLOR)
      throw runtime_error("BGCOLOR fehlt");
    if (!COLOR1)
      throw runtime_error("COLOR1 fehlt");
    if (!COLOR2)
      throw runtime_error("COLOR2 fehlt");

    // berechne Hilfswerte
    gd::BRect zr = gd::Stdfontpen::GetStringSize("10");
    int zb = zr.GetWidth() * 3 / 2;
    int zh = zr.GetHeight() * 3 / 2;
    int hg = height - 3 * zh;
    int wg = width - 2 * zb;
    int s = wg / 5;
    int m = s / 2;

    // Image und Stifte anlegen
    gd::Image im(width, height);
    im.GetColor( gd::Color(BGCOLOR) );
    XPen pen1 = im.GetPen( gd::Color(COLOR1) );
    XPen pen2 = im.GetPen( gd::Color(COLOR2) );

    // horizontale Linien zeichnen
    gd::Pen rahmenstift = im.GetPen(0xcc, 0xcc, 0xcc);
    for (i = 1; i < 10; ++i)
    {
      y = zh + hg * i / 10;
      rahmenstift.Line(zb, y, width - zb - 1, y);
    }

    // vertikale Linien
    for (i = 1; i < 5; ++i)
      rahmenstift.Line(zb + i * s, zh, zb + i * s, zh + hg);

    // Rahmen zeichnen
    rahmenstift.MoveTo(zb, zh)
               .LineTo(width - zb - 1, zh)
               .LineTo(width - zb - 1, zh + hg - 1)
               .LineTo(zb, zh + hg - 1)
               .LineTo(zb, zh);

    // Beschriftung
    gd::Stdfontpen schreibstift(im, im.GetColor(0, 0, 0));
    const char* labels[] = { "Neurotizismus", "Extraversion",
      "Gewissenhaftigkeit", "Motivation", "soziale Kompetenz" };
    for (i = 0; i < 5; ++i)
    {
      schreibstift.MoveTo( zb + m + i * s, hg + 2 * zh )
                  .WriteAlign( gd::Pen::HCENTER | gd::Pen::BOTTOM,
                               labels[i] )
                  .MoveTo( zb + m + i * s, hg + 3 * zh )
                  .WriteAlign( gd::Pen::HCENTER | gd::Pen::BOTTOM,
                               to_string( wert[0][i] / 10.0 ) );
    }

    // horizontale Linien
    for (i = 0; i <= 10; ++i)
    {
      y = zh + hg - hg * i / 10;
      schreibstift.MoveTo(zb / 2, y)
                  .WriteAlign(gd::Pen::HCENTER | gd::Pen::VCENTER,
                              to_string(i));
    }

    // 1. Linie zeichnen
    pen1.MoveTo(zb + m, zh + hg - hg * wert[0][0] / 100);
    pen1.quadrat();
    for (i = 1; i < 5; ++i )
    {
      x = zb + m + i * s;
      y = zh + hg - hg * wert[0][i] / 100;
      pen1.LineTo(x, y);
      pen1.quadrat();
    }

    // 2. Linie zeichnen
    pen2.MoveTo(zb + m, zh + hg - hg * wert[1][0] / 100);
    pen2.raute();
    for (i = 1; i < 5; ++i )
    {
      x = zb + m + i * s;
      y = zh + hg - hg * wert[1][i] / 100;
      pen2.LineTo(x, y);
      pen2.raute();
    }

    // ...und raus damit
    cout << q.last_modified_header("image/png");
    //gdImagePng(im, cout.rdbuf());
    gdImagePng(im, stdout);
  }
  catch(runtime_error e)
  {
    cerr << e.what() << endl;
    return -1;
  }
}
