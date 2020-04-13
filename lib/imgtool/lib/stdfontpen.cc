#include "stdfontpen.h"
#include <strstream>

using namespace std;

namespace gd {

std::string Stdfontpen::fontname;
double Stdfontpen::fontsize = 0;

void Stdfontpen::Init()
{
  if (!fontname.empty())
    return;

  char* e = getenv("FONTFILE");
  if (!e)
    throw runtime_error("FONTFILE nicht angegeben");
  fontname = e;

  e = getenv("FONTSIZE");
  if (!e)
    throw runtime_error("FONTSIZE nicht angegeben");
  fontsize = 0;
  istrstream(e) >> fontsize;
  if (!fontsize)
    throw runtime_error("FONTSIZE ungueltig");
}

Stdfontpen::Stdfontpen(gd::Image& im, int color)
  : Pen(im, color)
{
  Init();
  SetFont(fontname, fontsize);
}

Stdfontpen::Stdfontpen(gd::Image* im, int color)
  : Pen(im, color)
{
  Init();
  SetFont(fontname, fontsize);
}

} // namespace gd
