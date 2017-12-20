

WXFLAGS:=$(shell wx-config --cxxflags --libs)
FITSFLAGS:=-I../fits
WORKING_DIR:=$(shell pwd)
BUILD_DIR:=$(WORKING_DIR)/build

.phony: myapp

myapp: myapp.cpp imageframe.cpp
	mkdir -p $(BUILD_DIR)
	g++ myapp.cpp ../fits/fits.cpp  imageframe.cpp $(FITSFLAGS) $(WXFLAGS) -o $(BUILD_DIR)myapp
