

-include make/engine.mk

platform = x64
CC = g++

-include make/opencv.mk

-include astro_image/astro_image.mk

WXFLAGS:=$(shell wx-config --cxxflags --libs)

CXXFLAGS+=-stdlib=libc++ -std=c++14 $(INCLUDES)

CXXFLAGS+=-I/usr/local/Cellar/jpeg/9b/include -L/usr/local/Cellar/jpeg/9b/lib -ljpeg

WORKING_DIR:=$(shell pwd)
BUILD_DIR:=$(WORKING_DIR)/build



.phony: myapp hello test

myapp: myapp.cpp imageframe.cpp
	mkdir -p $(BUILD_DIR)
	g++ myapp.cpp ../fits/fits.cpp  imageframe.cpp $(FITSFLAGS) $(OPENCVFLAGS) $(WXFLAGS) $(CFLAGS) -o $(BUILD_DIR)/myapp

hello: hello.cpp
	mkdir -p $(BUILD_DIR)
	g++ hello.cpp $(FITSFLAGS) $(WXFLAGS) $(CFLAGS) -o $(BUILD_DIR)/hello

test: main.cpp
	@echo "Sources: $(SOURCES)"
	@echo "Includes: $(INCLUDES)"
	@echo "Makefile list: $(MAKEFILE_LIST)"
	@mkdir -p $(BUILD_DIR)
	@g++ main.cpp $(SOURCES) $(WXFLAGS) $(CXXFLAGS) -o $(BUILD_DIR)/test


