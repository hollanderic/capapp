

platform = x64

CC = g++

PLAT = $(shell uname -s)


ifeq ($(PLAT),Darwin)
#INCLIB = /usr/local/include
#LDLIB = /usr/local/lib
OPENCV = /usr/local/Cellar/opencv@2/2.4.13.4
OPENCVCFLAGS = -I$(OPENCV)/include
OPENCVLIBS = -L$(OPENCV)/lib -lopencv_calib3d \
             -lopencv_contrib -lopencv_core -lopencv_features2d -lopencv_flann \
             -lopencv_gpu -lopencv_highgui -lopencv_imgproc -lopencv_legacy -lopencv_ml \
             -lopencv_nonfree -lopencv_objdetect -lopencv_ocl -lopencv_photo \
             -lopencv_stitching -lopencv_superres -lopencv_ts -lopencv_video -lopencv_videostab
endif

ifeq ($(PLAT),Linux)
platform = x64
OPENCVCFLAGS = $(shell pkg-config --cflags opencv) -I/usr/include/opencv2
OPENCVLIBS = $(shell pkg-config --libs opencv)
endif

OPENCVFLAGS = $(OPENCVCFLAGS) $(OPENCVLIBS)

WXFLAGS:=$(shell wx-config --cxxflags --libs)
CFLAGS:=-stdlib=libc++ -std=c++11
FITSFLAGS:=-I../fits
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
	mkdir -p $(BUILD_DIR)
	g++ main.cpp ../fits/fits.cpp  $(FITSFLAGS) $(OPENCVFLAGS) $(WXFLAGS) $(CFLAGS) -o $(BUILD_DIR)/test


