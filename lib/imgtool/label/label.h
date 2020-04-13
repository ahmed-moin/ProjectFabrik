#ifndef _LABEL_H_
#define _LABEL_H_

#include "gd++.h"

using namespace std;

class Label : public gd::Image
{
    struct lo
    {
      string    line;
      gd::BRect rect;
    };

    gd::Color bgcolor;
    gd::Color fontcolor;
    int shift;

  public:
    Label()
      : shift(0)
        { }

    void SetBgcolor(gd::Color bg)
    { bgcolor = bg; }
    void SetFontcolor(gd::Color fc)
    { fontcolor = fc; }
    void SetShift(int s)
    { shift = s; }

    void Create(string txt,
      int margin_left, int margin_right, int margin_top, int margin_bottom,
      int line_spacing);
    void DrawFrame(int dx = 8, int dy = 6);
};

#endif // _LABEL_H_
