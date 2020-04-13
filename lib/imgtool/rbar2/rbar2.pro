TEMPLATE    = app
CONFIG      = warn_on release
HEADERS     = ../include/cgi.h ../include/gd++.h
SOURCES     = rbar2.cc ../lib/cgi.cc ../lib/gd++.cc
TARGET      = rbar2
INCLUDEPATH = /usr/local/include/gd /usr/include/freetype2 ../include
LIBS        = -lgd -lpng -lz -ljpeg -lfreetype
