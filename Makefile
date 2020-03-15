# Projet de TP X2BI040
# 2019/2020
# Les prénoms

.PHONY: clean

CXX = g++
CXXFLAGS = -Wall -I. -O2
LDFLAGS = -Wl,-rpath=$(shell pwd)

EXECS = lecture_prenoms main

all: $(EXECS)

libdate.so: date.o
	$(CXX) -shared -o $@ $^

date.o: CXXFLAGS += -fPIC
date.o: date.cpp date.hpp

lecture_prenoms: base_prenoms.o
lecture_prenoms: CXXFLAGS += -L.
lecture_prenoms: LOADLIBES += -lpthread

base_prenoms.o: base_prenoms.cpp base_prenoms.hpp

main: base_deces.o base_prenoms.o
main: CXXFLAGS += -L.
main: LOADLIBES += -lpthread

base_deces.o: base_deces.cpp base_deces.hpp

clean:
	-rm *.o libdate.so $(EXECS)
