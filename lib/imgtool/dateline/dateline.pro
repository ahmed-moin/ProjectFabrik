TEMPLATE    = app
CONFIG      = warn_on release
HEADERS     = ../include/cgi.h ../include/gd++.h ../include/stdfontpen.h
SOURCES     = dateline.cc ../lib/cgi.cc ../lib/stdfontpen.cc ../lib/gd++.cc
TARGET      = dateline
INCLUDEPATH = /usr/local/include/gd /usr/include/freetype2 ../include
LIBS        = -lgd -lpng -lz -ljpeg -lfreetype
