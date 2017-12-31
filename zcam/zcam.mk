MODULE=zcam

CWD:=$(GET_LOCAL_DIR)

SOURCES+=$(CWD)/zcam.cpp

INCLUDES+=-I$(CWD)

ZWOPATH = $(HOME)/src/external/zwo

ifeq ($(PLAT),Darwin)
platform = mac
ZWOLIBS = DYLD_LIBRARY_PATH=$(ZWOPATH)/lib/$(platform)
endif

ifeq ($(PLAT),Linux)
platform = x64
ZWOLIBS = LD_LIBRARY_PATH=$(ZWOPATH)/lib/$(platform)

endif

CXXFLAGS += -L$(ZWOPATH)/lib/$(platform) -I$(ZWOPATH)/include -lASICamera2