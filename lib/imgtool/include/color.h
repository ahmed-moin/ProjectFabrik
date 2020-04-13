////////////////////////////////////////////////////////////////////////

#ifndef _COLOR_H_
#define _COLOR_H_

#include <string>

////////////////////////////////////////////////////////////////////////

class color
{
    int Red, Green, Blue;

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

  public:
    color()
    { }

    color(int red, int green, int blue)
      : Red(red), Green(green), Blue(blue)
    { }

    color(std::string color)
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
          Red   = _hex2int(color.substr(0, 2)) << 4;
          Green = _hex2int(color.substr(2, 2)) << 4;
          Blue  = _hex2int(color.substr(4, 2)) << 4;
          break;

        default:
          string msg = "falsches Format bei der Farbangabe (";
          msg.append(color);
          msg.append(")");
          throw runtime_error(msg);
      }
    }

    int red() const
    { return Red; }

    void red(int r) const
    { Red = r; }

    int green() const
    { return Green; }

    void green(int g) const
    { Green = g; }

    int blue() const
    { return Blue; }

    void blue(int b) const
    { Blue = b; }
}
