#include <iostream>
#include <strstream>
#include "cgi.h"
#include "stdfontpen.h"
#include "convert.h"

using namespace std;

class Monat
{
    int monat, jahr;

  public:
    Monat(int m, int j)
      : monat(m), jahr(j)
      { }
    Monat(string m);

    int GetMonat() const
    { return monat; }

    int GetJahr() const
    { return jahr; }

    Monat& operator++ ();
    bool operator< (const Monat& m) const;

    int GetMonateBis(const Monat& m) const;
    string GetMonatsName(string lang = "") const;
};

Monat::Monat(string m)
{
  int v = string2<int>(m);
  jahr = v % 10000;
  monat = v / 10000;
}

Monat& Monat::operator++ ()
{
  if (++monat > 12)
  {
    monat = 1;
    ++jahr;
  }
  return *this;
}

bool Monat::operator< (const Monat& m) const
{
  return jahr < m.jahr
      || jahr == m.jahr && monat < m.monat;
}

int Monat::GetMonateBis(const Monat& m) const
{
  return (m.jahr - jahr) * 12 + m.monat - monat;
}

string Monat::GetMonatsName(string lang) const
{
  static char* Monate[] = { "Jan", "Feb", "Mär", "Apr", "Mai", "Jun",
                            "Jul", "Aug", "Sep", "Okt", "Nov", "Dez" };
  static char* MonateEn[] = { "jan", "feb", "mar", "apr", "may", "jun",
                              "jul", "aug", "sep", "oct", "nov", "dec" };
  return lang == "en" ? MonateEn[monat-1] : Monate[monat-1];
}

ostream& operator<< (ostream& o, const Monat& m)
{
  o << m.GetMonat() << '/' << m.GetJahr();
  return o;
}

int main()
{
  try
  {
    gd::BRect r = gd::Stdfontpen::GetStringSize("Jan");

    CGI q;
    Monat startmonat(q.param(0));
    Monat endemonat(q.param(1));
    int segbreite = string2<int>(q.param(2));
    string lang(q.param("lang", ""));

    int anzmonate = startmonat.GetMonateBis(endemonat) + 1;
    int breite = anzmonate * segbreite + 20;

    int zl1_y = r.GetHeight() + 2;
    int zl2_y = zl1_y * 2;
    int strahl_y = zl2_y + 8;
    int hoehe = zl2_y + 14;

    gd::Image im(breite, hoehe);
    im.GetColor(255, 255, 255);
    int black = im.GetColor(0, 0, 0);

    gd::Stdfontpen pen(im, black);

    // Zeitstrahl zeichnen
    pen.Line(0, strahl_y, breite, strahl_y);
    pen.Line(breite, strahl_y, breite - 4, strahl_y - 4);
    pen.Line(breite, strahl_y, breite - 4, strahl_y + 4);

    // 1. Jahreszahl und Monat
    int x = 0;
    int xx = (segbreite + 1) / 2;

    Monat a = startmonat;
    pen.Line(0, strahl_y - 4, 0, strahl_y + 4)
       .MoveTo(x, zl1_y)
       .WriteAlign(gd::Pen::LEFT, to_string(a.GetJahr()))
       .MoveTo(xx, zl2_y + 2)
       .WriteAlign(gd::Pen::HCENTER, a.GetMonatsName(lang));

    x += segbreite;
    xx += segbreite;

    for ( ++a; a <= endemonat; ++a, x += segbreite, xx += segbreite )
    {
      pen.MoveTo(xx, zl2_y + 2)
         .WriteAlign(gd::Pen::HCENTER, a.GetMonatsName(lang));

      if (a.GetMonat() == 1)
      {
        pen.MoveTo(x, zl1_y)
           .WriteAlign(gd::Pen::LEFT, to_string(a.GetJahr()))
           .Line(x, strahl_y - 4, x, strahl_y + 4);
      }
      else
      {
        pen.Line(x, strahl_y - 2, x, strahl_y + 2);
      }
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
