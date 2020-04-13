#include "gd++.h"
#include <iostream>

namespace gd
{

using namespace std;

static int _hex2int(string hex)
{
  int ret = 0;
  for(string::iterator b = hex.begin(); b != hex.end(); ++b)
    ret = (ret << 4) + (*b >= '0' && *b <= '9' ? (*b - '0')
                      : *b >= 'a' && *b <= 'f' ? (*b - 'a') + 10
                      : *b >= 'A' && *b <= 'F' ? (*b - 'A') + 10
                      : 0);
  return ret;
}

Color::Color(string color)
{
  switch(color.size())
  {
    case 0:
      Red = Green = Blue = 0;
      break;

    case 3:
      Red   = _hex2int(color.substr(0, 1)) << 4;
      Green = _hex2int(color.substr(1, 1)) << 4;
      Blue  = _hex2int(color.substr(2, 1)) << 4;
      break;

    case 6:
      Red   = _hex2int(color.substr(0, 2));
      Green = _hex2int(color.substr(2, 2));
      Blue  = _hex2int(color.substr(4, 2));
      break;

    default:
      string msg = "falsches Format bei der Farbangabe (";
      msg.append(color);
      msg.append(")");
      throw runtime_error(msg);
  }
}

void Image::Create(int x, int y)
{
  if (image)
    gdImageDestroy(image);
  image = gdImageCreate(x, y);
}

BRect Image::StringFT(string fontname, double ptsize,
                      string txt, double angle)
{
  int brect[8];
  char* err = gdImageStringFT(0, brect, 0,
         const_cast<char*>(fontname.c_str()),
         ptsize, angle, 0, 0, const_cast<char*>(txt.c_str()) );
  if (err)
    throw runtime_error(err);
  return BRect(brect);
}

BRect Image::StringFT(color_type fg, string fontname, double ptsize,
                Point pos, string txt, double angle)
{
  int brect[8];
  char* err = gdImageStringFT(image, brect, fg,
              const_cast<char*>(fontname.c_str()),
              ptsize, angle, pos.x, pos.y,
              const_cast<char*>(txt.c_str()) );
  if (err)
    throw runtime_error(err);
  return BRect(brect);
}

Pen& Pen::WriteAlign(int align, string txt, double angle)
{
  if (font.empty())
    throw runtime_error("Font nicht gesetzt");

  BRect r = GetStringSize(txt, angle);
  int x, y;
  x = (align & RIGHT) ?
        (align & LEFT) ? pos.x - (r.GetWidth()) / 2 -1
                       : pos.x - r.GetWidth() -1
        : pos.x;
  y = (align & TOP) ?
        (align & BOTTOM) ? pos.y + (r.GetHeight()) / 2
                         : pos.y + r.GetHeight()
        : pos.y;
  image->StringFT(color, font, fontsize, Point(x, y), txt, angle);
  return *this;
}

static int ostreamWriter(void* context, const char* buffer, int len)
{
  ostream* out = (ostream*)context;
  out->write(buffer, len);
  return len;
}


ostream& operator>> (ostream& out, const Image& im)
{
  gdSink ms;
  ms.context = (void*)&out;
  ms.sink = ostreamWriter;
  gdImagePngToSink(im.image, &ms);

  return out;
}

} // namespace gd
