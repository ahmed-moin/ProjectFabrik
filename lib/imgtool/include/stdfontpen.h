#ifndef _STDFONTPEN_H_
#define _STDFONTPEN_H_

#include "gd++.h"

namespace gd {

class Stdfontpen : public gd::Pen
{
    static std::string fontname;
    static double fontsize;

    static void Init();

  public:
    Stdfontpen(gd::Image& im, int color);
    Stdfontpen(gd::Image* im, int color);

    static std::string GetFontname()
    { Init(); return fontname; }

    static double GetFontsize()
    { Init(); return fontsize; }

    static gd::BRect GetStringSize(std::string txt, double angle = 0)
    { Init(); return gd::Image::StringFT(fontname, fontsize, txt, angle); }
};

} // namespace gd

#endif //  _STDFONTPEN_H_
