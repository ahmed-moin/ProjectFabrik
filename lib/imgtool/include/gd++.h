////////////////////////////////////////////////////////////////////////

#ifndef _GDPP_H_
#define _GDPP_H_

#include <gd.h>
#include <string>
#include <algorithm>
#include <stdexcept>
#include <iostream>

namespace gd
{

////////////////////////////////////////////////////////////////////////
struct Point
{
  int x, y;
  Point()
  { }

  Point(int _x, int _y)
    : x(_x), y(_y)
  { }
};

////////////////////////////////////////////////////////////////////////
struct Rect
{
  Point leftUpper, rightLower;

  Rect()
  { }

  Rect(Point lu, Point rl)
    : leftUpper(lu), rightLower(rl)
  { }

  Rect(int x0, int y0, int x1, int y1)
    : leftUpper(x0, y0), rightLower(x1, y1)
  { }

};

////////////////////////////////////////////////////////////////////////
struct BRect
{
  int brect[8];

  BRect()
  { }

  BRect(int b[8])
  {
    for (int i = 0; i < 8; ++i)
      brect[i] = b[i];
  }

  operator int* ()  { return brect; }
  int operator [] (int i)  { return brect[i]; }

  int MinX() const
  {
    return std::min(
      std::min(brect[0], brect[2]),
      std::min(brect[4], brect[6]) );
  }

  int MinY() const
  {
    return std::min(
      std::min(brect[1], brect[3]),
      std::min(brect[5], brect[7]) );
  }

  int MaxX() const
  {
    return std::max(
      std::max(brect[0], brect[2]),
      std::max(brect[4], brect[6]) );
  }

  int MaxY() const
  {
    return std::max(
      std::max(brect[1], brect[3]),
      std::max(brect[5], brect[7]) );
  }

  int GetWidth() const
  {
    return MaxX() - MinX();
  }

  int GetHeight() const
  {
    return MaxY() - MinY();
  }

};

////////////////////////////////////////////////////////////////////////
class Color
{
    int Red, Green, Blue;

  public:
    Color()
      : Red(0), Green(0), Blue(0)
    { }

    Color(int red, int green, int blue)
      : Red(red), Green(green), Blue(blue)
    { }

    Color(std::string color);

    int  red() const   { return Red; }
    int  green() const { return Green; }
    int  blue() const  { return Blue; }

    void red(int r)   { Red = r; }
    void green(int g) { Green = g; }
    void blue(int b)  { Blue = b; }
};

////////////////////////////////////////////////////////////////////////
class Pen;

////////////////////////////////////////////////////////////////////////
class Image
{
  private:
    Image(const Image&);

  //protected:
  //  gdImagePtr image;

  public:
    gdImagePtr image;
    typedef int color_type;

    Image()
      : image(0)
    { }

    Image(int x, int y)
    { image = gdImageCreate(x, y); }

    ~Image()
    {
      if (image)
        gdImageDestroy(image);
    }

    operator gdImagePtr()  { return image; }

    void Create(int, int);

    int GetWidth() const
    { return gdImageSX(image); }

    color_type GetPixel(Point p) const
    { return gdImageGetPixel(image, p.x, p.y); }

    int GetHeight() const
    { return gdImageSY(image); }

    color_type ColorAllocate(int r, int g, int b)
    { return gdImageColorAllocate(image, r, g, b); }

    color_type ColorAllocate(Color color)
    { return ColorAllocate(color.red(), color.green(), color.blue()); }

    color_type ColorClosest(int r, int g, int b) const
    { return gdImageColorClosest(const_cast<gdImagePtr>(image), r, g, b); }

    color_type ColorClosest(Color color) const
    { return ColorClosest(color.red(), color.green(), color.blue()); }

    color_type GetColor(int r, int g, int b) const
    { return gdImageColorResolve(const_cast<gdImagePtr>(image), r, g, b); }

    color_type GetColor(Color color) const
    { return GetColor(color.red(), color.green(), color.blue()); }

    static BRect StringFT(std::string, double,
                           std::string, double = 0);
    BRect StringFT(color_type, std::string, double,
                    Point, std::string, double = 0);

    Pen Line(int, int, int, int, color_type);
    Pen Line(Point, Point, color_type);
    Pen GetPen(color_type);
    Pen GetPen(int, int, int);
    Pen GetPen(Color);

    friend std::istream& operator<<(std::istream&, Image&);
    friend std::ostream& operator>>(std::ostream&, const Image&);
};

////////////////////////////////////////////////////////////////////////
class Pen
{
  public:
    typedef Image::color_type color_type;

  private:
    Image* image;
    Point pos;
    color_type color;
    std::string font;
    double fontsize;

  public:
    Pen(Image& im, color_type c)
      : image(&im), color(c), fontsize(10)
    { }

    Pen(Image* im, color_type c)
      : image(im), color(c), fontsize(10)
    { }

    Pen(Image& im, Point p, color_type c)
      : image(&im), pos(p), color(c), fontsize(10)
    { }

    Pen(Image* im, Point p, color_type c)
      : image(im), pos(p), color(c), fontsize(10)
    { }

    Pen& SetColor(color_type c)
    {
      color = c;
      return *this;
    }

    Point GetPos() const
    { return pos; }

    color_type GetColor() const
    { return color; }

    Pen& SetFont(std::string f)
    {
      font = f;
      return *this;
    }

    Image& GetImage()
    {
      return *image;
    }

    const Image& GetImage() const
    {
      return *image;
    }

    Pen& SetFont(std::string f, double fs)
    {
      font = f;
      fontsize = fs;
      return *this;
    }

    Pen& SetFontSize(double fs)
    {
      fontsize = fs;
      return *this;
    }

    double GetFontSize() const
    { return fontsize; }

    Pen& Line(int x0, int y0, int x1, int y1)
    {
      image->Line(x0, y0, x1, y1, color);
      pos = Point(x1, y1);
      return *this;
    }

    Pen& Line(Point p0, Point p1)
    {
      image->Line(p0, p1, color);
      pos = p1;
      return *this;
    }

    Pen& LineTo(int x, int y)
    {
      image->Line(pos.x, pos.y, x, y, color);
      pos = Point(x, y);
      return *this;
    }

    Pen& LineTo(Point p)
    {
      image->Line(pos, p, color);
      pos = p;
      return *this;
    }

    Pen& MoveTo(int x, int y)
    {
      pos = Point(x, y);
      return *this;
    }

    Pen& MoveTo(Point p)
    {
      pos = p;
      return *this;
    }

    void Rectangle(int x1, int y1, int x2, int y2)
    { ::gdImageRectangle(*image, x1, y1, x2, y2, color); }

    Pen& Rectangle(int x, int y)
    {
      ::gdImageRectangle(*image, pos.x, pos.y, x, y, color);
      pos = Point(x, y);
      return *this;
    }

    void Arc(int cx, int cy, int w, int h, int s, int e)
    { ::gdImageArc(*image, cx, cy, w, h, s, e, color); }

    void Circle(int cx, int cy, int r)
    { ::gdImageArc(*image, cx, cy, r*2, r*2, 0, 360, color); }

    Pen& Circle(int r)
    {
      ::gdImageArc(*image, pos.x, pos.y, r*2, r*2, 0, 360, color);
      return *this;
    }

    void FillToBorder(int x, int y, color_type border)
    { ::gdImageFillToBorder(*image, x, y, border, color); }

    void Fill(int x, int y)
    { ::gdImageFill(*image, x, y, color); }

    BRect GetStringSize(std::string txt, double angle = 0)
    { return gd::Image::StringFT(font, fontsize, txt, angle); }

    Pen& Write(std::string txt, double angle = 0)
    {
      image->StringFT(color, font, fontsize, pos, txt, angle);
      return *this;
    }

    Pen& Write(int x, int y, std::string txt, double angle = 0)
    {
      MoveTo(x, y);
      return Write(txt, angle);
    }

    enum { LEFT = 1, HCENTER = 3, RIGHT = 2,
           TOP = 0x10, VCENTER = 0x30, BOTTOM = 0x20,
           CENTER = 0x33 };
    Pen& WriteAlign(int align, std::string txt, double angle = 0);
};

////////////////////////////////////////////////////////////////////////
inline Pen Image::Line(int x0, int y0, int x1, int y1, color_type color)
{
  gdImageLine(image, x0, y0, x1, y1, color);
  return Pen(this, Point(x1, y1), color);
}

inline Pen Image::Line(Point p0, Point p1, color_type color)
{
  gdImageLine(image, p0.x, p0.y, p1.x, p1.y, color);
  return Pen(this, p1, color);
}

inline gd::Pen gd::Image::GetPen(color_type color)
{
  return Pen(this, Point(0, 0), color);
}

inline gd::Pen gd::Image::GetPen(int r, int g, int b)
{
  return GetPen(GetColor(r, g, b));
}

inline gd::Pen gd::Image::GetPen(Color color)
{
  return GetPen(color.red(), color.green(), color.blue());
}

}  // namespace gd

#endif // _GDPP_H_
