#include <fstream>
#include <strstream>
#include "gd++.h"
#include "cgi.h"
#include "stdfontpen.h"
#include "convert.h"

using namespace std;

int main()
{
  try
  {
    // werte Parameter aus
    CGI q;
    string user         = q.param(0);
    gd::Color fontcolor = q.param("fc", "000000");
    gd::Color bgcolor   = q.param("bg", "FFFFFF");

    // ermittele Konfigurationsverzeichnis
    char* counterdir = getenv("COUNTERDIR");
    if (!counterdir)
      throw runtime_error("COUNTERDIR fehlt");

    // baue Dateiname aus $CONTERDIR/user zusammen
    string file = counterdir;
    file.append(1, '/');
    file.append(user);

    // lese aktuellen Zählerstand
    int count;
    if (!(ifstream(file.c_str()) >> count))
    {
      string msg = "Datei ";
      msg.append(file);
      msg.append(" konnte nicht gelesen werden");
      throw runtime_error(msg);
    }

    // erhöhe aktuellen Zählerstand
    ++count;
    if (!(ofstream(file.c_str()) << count))
    {
      string msg = "Datei ";
      msg.append(file);
      msg.append(" konnte nicht geschrieben werden");
      throw runtime_error(msg);
    }

    // erzeuge Grafik
    char buffer[30];
    ostrstream(buffer, 30) << count << ends;
    string label = buffer;

    gd::BRect rect = gd::Stdfontpen::GetStringSize(label);

    gd::Image image(rect.GetWidth() + 3, rect.GetHeight());
    image.GetColor(bgcolor);
    gd::Stdfontpen pen(image, image.GetColor(fontcolor));
    pen.MoveTo(0, rect.GetHeight())
       .Write(label);

    cout << CGI::last_modified_header("image/png");
    //gdImagePng(image, cout.rdbuf());
    gdImagePng(image, stdout);
  }
  catch(runtime_error e)
  {
    cerr << e.what() << endl;
    return -1;
  }

 return 0;
}
