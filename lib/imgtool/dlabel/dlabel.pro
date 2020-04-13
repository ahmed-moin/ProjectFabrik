TEMPLATE    = app
CONFIG      = warn_on release
HEADERS     = ../include/cgi.h ../include/gd++.h ../include/stdfontpen.h ../label/label.h
SOURCES     = dlabel.cc ../lib/cgi.cc ../lib/stdfontpen.cc ../lib/gd++.cc ../label/label.cc
TARGET      = dlabel
INCLUDEPATH = /usr/include/gd /usr/include/freetype2 ../include ../label
LIBS        = -lgd -lpng -lz -ljpeg -lfreetype
