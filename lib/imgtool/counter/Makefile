#############################################################################
# Makefile for building counter
# Generated by tmake at 23:03, 2001/04/16
#     Project: counter
#    Template: app
#############################################################################

####### Compiler, tools and options

CC	=	gcc
CXX	=	g++
CFLAGS	=	-pipe -Wall -W -O2
CXXFLAGS=	-pipe -fomit-frame-pointer -Wall -W -O2 -march=x86-64 -Wno-deprecated
INCPATH	=	-I/usr/local/include/gd -I/usr/include/freetype2 -I../include
LINK	=	g++
LIBS	=	$(SUBLIBS) -lgd -lpng -lz -ljpeg -lfreetype
MOC	=	$(QTDIR)/bin/moc
UIC	=	$(QTDIR)/bin/uic

TAR	=	tar -cf
GZIP	=	gzip -9f

####### Files

HEADERS =	../include/cgi.h \
		../include/gd++.h \
		../include/stdfontpen.h
SOURCES =	counter.cc \
		../lib/cgi.cc \
		../lib/stdfontpen.cc \
		../lib/gd++.cc
OBJECTS =	counter.o \
		../lib/cgi.o \
		../lib/stdfontpen.o \
		../lib/gd++.o
INTERFACES =	
UICDECLS =	
UICIMPLS =	
SRCMOC	=	
OBJMOC	=	
DIST	=	
TARGET	=	counter
INTERFACE_DECL_PATH = .

####### Implicit rules

.SUFFIXES: .cpp .cxx .cc .C .c

.cpp.o:
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o $@ $<

.cxx.o:
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o $@ $<

.cc.o:
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o $@ $<

.C.o:
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o $@ $<

.c.o:
	$(CC) -c $(CFLAGS) $(INCPATH) -o $@ $<

####### Build rules


all: $(TARGET)

$(TARGET): $(UICDECLS) $(OBJECTS) $(OBJMOC) 
	$(LINK) $(LFLAGS) -o $(TARGET) $(OBJECTS) $(OBJMOC) $(LIBS)

moc: $(SRCMOC)

tmake:
	tmake counter.pro

dist:
	$(TAR) counter.tar counter.pro $(SOURCES) $(HEADERS) $(INTERFACES) $(DIST)
	$(GZIP) counter.tar

clean:
	-rm -f $(OBJECTS) $(OBJMOC) $(SRCMOC) $(UICIMPLS) $(UICDECLS) $(TARGET)
	-rm -f *~ core

####### Sub-libraries


###### Combined headers


####### Compile

counter.o: counter.cc

../lib/cgi.o: ../lib/cgi.cc

../lib/stdfontpen.o: ../lib/stdfontpen.cc

../lib/gd++.o: ../lib/gd++.cc

