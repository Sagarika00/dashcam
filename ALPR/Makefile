CXX=g++
CXXFLAGS=-fPIC -fopenmp -lopencv_core.so.3.2 -lopenalpr -std=c++11 -L/usr/lib/x86_64-linux-gnu/

default : app

alpr.o : alpr.cpp
	${CXX} -c ${OPTRPT} ${CXXFLAGS} -o "$@" "$<"

app : alpr.o
	${CXX} ${OPTRPT} ${CXXFLAGS} -o "$@" "$<" alpr.o

queue: app

clean :
	rm -rf ./app *.o