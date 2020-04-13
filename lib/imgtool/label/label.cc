#include "label.h"
#include "stdfontpen.h"
#include <vector>

using namespace std;

////////////////////////////////////////////////////////////////////////
void Label::Create(string txt,
  int margin_left, int margin_right, int margin_top, int margin_bottom,
  int line_spacing)
{
  int x = 0, y = 0;  // Grösse des Labels

  string::iterator zeilenanfang = txt.begin();
  vector<lo> lines;
  int lineheight = gd::Stdfontpen::GetStringSize("X").GetHeight();

  for (string::iterator i = txt.begin(); 1; ++i)
  {
    if (i == txt.end() || *i == '\n')
    {
      // Zeilenende gefunden

      struct lo lo;
      lo.line = string(zeilenanfang, i);
      lo.rect = gd::Stdfontpen::GetStringSize(lo.line);

      // die Breite des Images wird durch die längste Zeile bestimmt
      if ( lo.rect.GetWidth() > x )
        x = lo.rect.GetWidth();
      // die Höhe ergibt sich aus den Zeilen
      y += lineheight + line_spacing;
      lines.push_back(lo);
      zeilenanfang = i + 1;
    }

    if (i == txt.end())
      break;
  }
  y -= line_spacing;

  gd::Image::Create(x + margin_left + margin_right,
                    y + margin_top + margin_bottom);

  GetColor(bgcolor);  // Hintergrundfarbe alloziieren
  gd::Stdfontpen pen(this, GetColor(fontcolor));

  y = margin_top + lineheight;
  for (vector<lo>::iterator i = lines.begin(); i != lines.end(); ++i)
  {
    pen.MoveTo(x + margin_left - (*i).rect.GetWidth() + shift, y + shift)
       .Write((*i).line);
    y += lineheight + line_spacing;
  }
}

////////////////////////////////////////////////////////////////////////
void Label::DrawFrame(int dx, int dy)
{
  int white = GetColor(255, 255, 255);
  gd::Pen p = GetPen(white);
  int X = GetWidth();
  int Y = GetHeight();

  p.Line(dx, 0, 0, 0).LineTo(0, dy)       // links oben
   .Line(dx - 1, 1, 1, 1).LineTo(1, dy - 1)
   .Line(X-1-dx, 0, X-1, 0).LineTo(X-1, dy)  // rechts oben
   .Line(X-2, dy-1, X-2, 1).LineTo(X-dx, 1)
   .Line(dx, Y-1, 0, Y-1).LineTo(0, Y-1-dy)  // links unten
   .Line(dx-1, Y-2, 1, Y-2).LineTo(1, Y-dy)
   .Line(X-1, Y-1-dy, X-1, Y-1).LineTo(X-1-dx, Y-1)  // rechts unten
   .Line(X-2, Y-dy, X-2, Y-2).LineTo(X-dx, Y-2);
}
