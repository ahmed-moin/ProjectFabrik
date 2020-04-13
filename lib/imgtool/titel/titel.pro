TEMPLATE    = app
CONFIG      = warn_on release
HEADERS     = ../include/cgi.h ../include/gd++.h ../include/stdfontpen.h
SOURCES     = titel.cc ../lib/cgi.cc ../lib/stdfontpen.cc ../lib/gd++.cc
TARGET      = titel
INCLUDEPATH = /usr/include/gd /usr/include/freetype2 ../include
LIBS        = -lgd -lpng -lz -ljpeg -lfreetype
