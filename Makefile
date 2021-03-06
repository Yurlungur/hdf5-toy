CXX=h5c++
CXXFLAGS = -Wall -g # -show

default: all
all: hello-hdf5.bin
test: hello-hdf5.bin hello-hdf5.py
	./hello-hdf5.bin
	h5dump test.h5
	python hello-hdf5.py
hello-hdf5.bin: hello-hdf5.o
	$(CXX) $(CXXFLAGS) -o $@ $^
hello-hdf5.o: hello-hdf5.cpp

clean:
	$(RM) hello-hdf5.bin hello-hdf5.o test.h5 hello-hdf5.pyc

.PHONY: all test default
