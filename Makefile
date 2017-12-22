

WXFLAGS:=$(shell wx-config --cxxflags --libs)
CFLAGS:=-stdlib=libc++ -std=c++11
FITSFLAGS:=-I../fits
WORKING_DIR:=$(shell pwd)
BUILD_DIR:=$(WORKING_DIR)/build

.phony: myapp hello

myapp: myapp.cpp imageframe.cpp
	mkdir -p $(BUILD_DIR)
	g++ myapp.cpp ../fits/fits.cpp  imageframe.cpp $(FITSFLAGS) $(WXFLAGS) $(CFLAGS) -o $(BUILD_DIR)/myapp


hello: hello.cpp
	mkdir -p $(BUILD_DIR)
	g++ hello.cpp $(FITSFLAGS) $(WXFLAGS) $(CFLAGS) -o $(BUILD_DIR)/hello

