TEMPLATE    = app
CONFIG      = warn_on release
HEADERS     = ../include/cgi.h ../include/gd++.h ../include/stdfontpen.h label.h
SOURCES     = label.cc ../lib/cgi.cc ../lib/stdfontpen.cc ../lib/gd++.cc main.cc
TARGET      = label
INCLUDEPATH = /usr/include/gd /usr/include/freetype2 ../include
LIBS        = -lgd -lpng -lz -ljpeg -lfreetype
