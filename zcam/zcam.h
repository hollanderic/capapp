
#pragma once

#include <astro_image.h>
#include <opencv2/opencv.hpp>
#include <unistd.h>

#include "ASICamera2.h"

using namespace cv;

#define ECAM_LOGS_ALL   (0xffffffff)
#define ECAM_LOGS_ERROR ( 1 << 0 )
#define ECAM_LOGS_DEBUG ( 1 << 1 )
#define ECAM_LOGS_INFO  ( 1 << 2 )


#define eclogf(flags,fmt...) \
    do { \
        if (ECAM_LOGS_##flags & log_flags_) { \
            printf(fmt); \
        } \
    } while(0);

#define eclogf_s(flags,fmt...) \
    do { \
        if (ECAM_LOGS_##flags & this->log_flags_) { \
            printf(fmt); \
        } \
    } while(0);


class ZWOCamera {
public:
    static ZWOCamera *Create();
    static uint32_t NumConnectedCameras();

    ZWOCamera();
    ZWOCamera(uint32_t idx);

    ~ZWOCamera();

    long        getGain()               { return getVal(ASI_GAIN);};
    long        setGain(long val)       { return setVal(ASI_GAIN,val);};
    long        setGain(const char* val);

    long        getExposure()           { return getVal(ASI_EXPOSURE);};
    long        setExposure(long val);
    long        setExposure(const char* val);
    void        startExposure();
    int         exposureStatus();

    float       getTemperature()        { return (float)getVal(ASI_TEMPERATURE)/10.0;}

    uint32_t    setMonoBin()            { return setVal(ASI_MONO_BIN,1);};

    long        isHWBin()               { return getVal(ASI_HARDWARE_BIN);};

    uint32_t    getWidth()              { return width_;};
    uint32_t    getHeight()             { return height_;};
    bool        isConnected()           { return connected_;};

    void        enableLogs(uint32_t flags) { log_flags_ |= flags;};
    void        disableLogs(uint32_t flags) { log_flags_ &= ~flags;};

    uint32_t    saveTIFF(const char* fname);

    uint32_t    setROI(uint32_t x, uint32_t y, uint32_t width, uint32_t height);
    uint32_t    loadData();
    double      adu();

    std::shared_ptr<AstroImage> getAstroImage();

private:
    static void onMouse(int event, int x, int y, int, void*);
    long getVal( ASI_CONTROL_TYPE ctl);
    long setVal( ASI_CONTROL_TYPE ctl, long val);

    bool connected_;
    bool mouse_down_;

    uint32_t log_flags_ = ECAM_LOGS_ALL;

    int      idx_ = -1;
    uint32_t max_width_;
    uint32_t max_height_;
    int      width_;
    int      height_;
    uint32_t pixel_size_;
    uint32_t bpp_;
    int      bin_;
    long     exposure_time_;

    Mat image_;
    Mat im_rgb16_;
    Mat im_rgb_;
    Mat im_preview_;
};
