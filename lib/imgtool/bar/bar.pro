TEMPLATE    = app
CONFIG      = warn_on release
HEADERS     = ../include/cgi.h ../include/gd++.h
SOURCES     = bar.cc ../lib/cgi.cc ../lib/gd++.cc
TARGET      = bar
INCLUDEPATH = /usr/include/gd /usr/include/freetype2 ../include
LIBS        = -lgd -lpng -lz -ljpeg -lfreetype
