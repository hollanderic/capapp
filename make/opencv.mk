ifeq ($(PLAT),Darwin)
OPENCV = /usr/local/Cellar/opencv@2/2.4.13.4
OPENCVFLAGS += -I$(OPENCV)/include
OPENCVFLAGS += -L$(OPENCV)/lib \
			 -lopencv_calib3d \
             -lopencv_contrib -lopencv_core -lopencv_features2d -lopencv_flann \
             -lopencv_gpu -lopencv_highgui -lopencv_imgproc -lopencv_legacy \
             -lopencv_ml -lopencv_nonfree -lopencv_objdetect -lopencv_ocl \
             -lopencv_photo -lopencv_stitching -lopencv_superres -lopencv_ts \
             -lopencv_video -lopencv_videostab
endif
ifeq ($(PLAT),Linux)
OPENCVFLAGS += $(shell pkg-config --cflags opencv) -I/usr/include/opencv2
OPENCVFLAGS += $(shell pkg-config --libs opencv)
endif
CXXFLAGS += $(OPENCVFLAGS)