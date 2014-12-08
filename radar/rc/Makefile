CXX=g++ 
CXXFLAGS= -Wall -O4 # -g

###############################################################################

FILES = c.cc r.cc sincronizador.h Makefile
SRC = c.cc r.cc
BIN = c r

###############################################################################

all:	${BIN}

clean:	
	rm -f ${BIN} *~ *.o core A S

c:	c.cc sincronizador.h Makefile
	g++  ${CXXFLAGS}  c.cc -o c

r:	r.cc Makefile
	g++  ${CXXFLAGS}  r.cc -o r

###############################################################################

.NOEXPORT:
