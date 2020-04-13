#include "gd++.h"
#include "stdfontpen.h"

using namespace std;
using namespace gd;

void main()
{
  try
  {
    Image image(400, 200);
    image.GetColor(255, 255, 192);
    int black = image.GetColor(0, 0, 0);
    Stdfontpen p(image, black);

    p.Line(0, 50, 400, 50)
     .Line(200, 0, 200, 200)
     .Line(0, 150, 400, 150)
     .MoveTo(200, 50)
     .WriteAlign(Pen::LEFT|Pen::TOP, "LEFT|TOP")
     .WriteAlign(Pen::RIGHT|Pen::BOTTOM, "RIGHT|BOTTOM")
     .WriteAlign(Pen::RIGHT|Pen::TOP, "RIGHT|TOP")
     .WriteAlign(Pen::LEFT|Pen::BOTTOM, "LEFT|BOTTOM")
     .MoveTo(200, 150)
     .WriteAlign(Pen::CENTER, "CENTER");

    cout << "Content-type: image/png\r\n\r\n";
    gdImagePng(image, cout.rdbuf());
  }
  catch(exception e)
  {
    cerr << e.what() << endl;
  }
}
