#############################################################################
# Makefile for building titel
# Generated by tmake at 12:13, 2001/04/17
#     Project: titel
#    Template: app
#############################################################################

####### Compiler, tools and options

CC	=	gcc
CXX	=	g++
CFLAGS	=	-pipe -Wall -W -O2
CXXFLAGS=	-pipe -fomit-frame-pointer -Wall -W -O2 -march=x86-64 -Wno-deprecated
INCPATH	=	-I/usr/include/gd -I/usr/include/freetype2 -I../include
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
SOURCES =	titel.cc \
		../lib/cgi.cc \
		../lib/stdfontpen.cc \
		../lib/gd++.cc
OBJECTS =	titel.o \
		../lib/cgi.o \
		../lib/stdfontpen.o \
		../lib/gd++.o
INTERFACES =	
UICDECLS =	
UICIMPLS =	
SRCMOC	=	
OBJMOC	=	
DIST	=	
TARGET	=	titel
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
	tmake titel.pro

dist:
	$(TAR) titel.tar titel.pro $(SOURCES) $(HEADERS) $(INTERFACES) $(DIST)
	$(GZIP) titel.tar

clean:
	-rm -f $(OBJECTS) $(OBJMOC) $(SRCMOC) $(UICIMPLS) $(UICDECLS) $(TARGET)
	-rm -f *~ core

####### Sub-libraries


###### Combined headers


####### Compile

titel.o: titel.cc

../lib/cgi.o: ../lib/cgi.cc

../lib/stdfontpen.o: ../lib/stdfontpen.cc

../lib/gd++.o: ../lib/gd++.cc

