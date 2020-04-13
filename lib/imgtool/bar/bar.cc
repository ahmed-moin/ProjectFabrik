#include "gd++.h"
#include "cgi.h"
#include "convert.h"

////////////////////////////////////////////////////////////////////////
using namespace std;

int main()
{
  try
  {
    CGI q;

    int x     = string2<int>(q.param(0));
    int y     = string2<int>(q.param(1));
    int red   = string2<int>(q.param(2));
    int green = string2<int>(q.param(3));
    int blue  = string2<int>(q.param(4));

    if (x > 2000 || y > 2000)
      throw range_error("Werteüberlauf");

    gd::Image image(x, y);
    image.GetColor(red, green, blue);

    cout << CGI::last_modified_header("image/png");
    //gdImagePng(image, cout.rdbuf());
    gdImagePng(image, stdout);
  }
  catch(range_error e)
  {
    cerr << "range_error: " << e.what() << endl;
  }
  catch(runtime_error e)
  {
    cerr << "runtime_error: " << e.what() << endl;
  }
  return 0;
}

