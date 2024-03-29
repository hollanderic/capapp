#include "stdio.h"
#include <opencv2/opencv.hpp>
#include "ASICamera2.h"
#include <sys/time.h>
#include <stdarg.h>
#include <time.h>
#include <unistd.h>


#include "zcam.h"

using namespace cv;

//Converts ASI_IMG_TYPE to number of bytes per pixel
static inline uint32_t asiImgTypeToBPP(ASI_IMG_TYPE imtype) {
    switch (imtype) {
        case ASI_IMG_RAW8:
            return 1;
        case ASI_IMG_RGB24:
            return 3;
        case ASI_IMG_RAW16:
            return 2;
        case ASI_IMG_Y8:
            return 1;
        default:
            return 1;
    }
}

ZWOCamera::ZWOCamera() {
    //By default show all error messages in log
    this->log_flags_ = ECAM_LOGS_ERROR;
    uint32_t log_flags_ = this->log_flags_;

    this->connected_ = false;
    if (NumConnectedCameras() == 0) {
        eclogf(ERROR,"No cameras connected!\n");
        return;
    }

    if (ASIOpenCamera(0) != ASI_SUCCESS)
    {
        eclogf(ERROR,"OpenCamera error\n");
        return;
    }
    if (ASIInitCamera(0) != ASI_SUCCESS) {
        eclogf(ERROR,"Could not initialize Camera!\n");
        ASICloseCamera(0);
        return;
    }
    eclogf(INFO,"Opened CameraID = %d\n",this->idx_);

    ASI_CAMERA_INFO ASICameraInfo;
    ASIGetCameraProperty(&ASICameraInfo, 0);
    this->max_width_ = ASICameraInfo.MaxWidth;
    this->max_height_ = ASICameraInfo.MaxHeight;
    this->pixel_size_ = ASICameraInfo.PixelSize;
    this->idx_ = ASICameraInfo.CameraID;

    ASI_IMG_TYPE it;
    ASIGetROIFormat(this->idx_, &this->width_,
                                &this->height_,
                                &this->bin_, &it);

    this->bpp_ = asiImgTypeToBPP(it);
    eclogf(INFO,"Image size: %d x %d bpp:%d\n",width_,height_,this->bpp_);

#if 0
    int numcontrols;
    ASI_CONTROL_CAPS cap;
    ASIGetNumOfControls(this->idx_, &numcontrols);
    for (int i=0 ; i<numcontrols; i++) {
        ASIGetControlCaps(this->idx_,i,&cap);
        printf("Control %s\n",cap.Name);
        printf("\t%s\n",cap.Description);
        printf("\tMax: %ld\n",cap.MaxValue);
        printf("\tMin: %ld\n",cap.MinValue);
        printf("\tdef:%ld\n",cap.DefaultValue);
        printf("\tctl type:%d\n",cap.ControlType);
    }
#endif
    this->connected_ = true;
    return;
}

uint32_t ZWOCamera::NumConnectedCameras() {
    return ASIGetNumOfConnectedCameras();
}

ZWOCamera::~ZWOCamera() {
    if (idx_ >= 0) {
        ASICloseCamera(idx_);
    }
    printf("ZWOCamera closed\n");
}

long ZWOCamera::setGain(const char* val) {
    int g = atoi(val);
    printf("Gain set to %d\n",g);
    return setGain(g);
}

uint32_t ZWOCamera::setROI(uint32_t x, uint32_t y, uint32_t width, uint32_t height) {

    ASI_ERROR_CODE err;
    err = ASISetStartPos(idx_,x,y);
    if (err != ASI_SUCCESS) {
        eclogf(ERROR,"Set Start Pos failed\n");
        return 1;
    }
    width = (width + 8 - 1) & ~0x07;
    height = (height + 8 - 1) & ~0x07;

    printf("setting roi - %d x %d\n",width,height);
    err = ASISetROIFormat(idx_, width, height, 1, ASI_IMG_RAW16);
    if (err != ASI_SUCCESS) {
        eclogf(ERROR,"Set ROI format failed - %d\n",err);
    }
    width_ = width;
    height_ = height;

    int w;
    int h;
    int bin;
    ASI_IMG_TYPE it;
    ASIGetROIFormat(idx_, &w, &h,  &bin, &it);
    width_ = w;
    height_ = h;
    bpp_ = asiImgTypeToBPP(it);
    printf("Image size: %d x %d\n",width_,height_);

    return 0;

}

long ZWOCamera::setExposure(const char* val){
    int e = atoi(val);
    return setExposure(e);
}

long ZWOCamera::setExposure(long val) {
    exposure_time_ = val;
    return setVal(ASI_EXPOSURE,val);
}

void ZWOCamera::startExposure() {
    ASI_BOOL dark = ASI_FALSE;
    ASIStartExposure(idx_, dark);
}

int ZWOCamera::exposureStatus() {
    ASI_EXPOSURE_STATUS stat;
    ASIGetExpStatus(idx_,&stat);
    if (stat == ASI_EXP_WORKING) return 1;
    if (stat == ASI_EXP_SUCCESS) return 0;
    if (stat == ASI_EXP_FAILED) return -1;
    return -1;
}

long ZWOCamera::getVal(ASI_CONTROL_TYPE ctl) {
    long val;
    ASI_BOOL pbool = ASI_FALSE;
    ASI_ERROR_CODE stat;
    stat = ASIGetControlValue(idx_,ctl,&val,&pbool);

    if (stat != ASI_SUCCESS) {
        printf("getVal failed %d\n",stat);
    }
    return val;
}

uint32_t ZWOCamera::saveTIFF(const char* fname){
    imwrite(fname,im_rgb_);
    return 0;
}

long ZWOCamera::setVal(ASI_CONTROL_TYPE ctl, long val){
    ASI_BOOL pbool = ASI_FALSE;
    ASI_ERROR_CODE stat;
    stat = ASISetControlValue(idx_,ctl,val,pbool);

    if (stat != ASI_SUCCESS) {
        printf("setVal failed %d\n",stat);
    }
    return val;
}

uint32_t ZWOCamera::loadData() {
    image_ = Mat::zeros(height_, width_, CV_8UC1 );
    ASI_ERROR_CODE stat;
    stat = ASIGetDataAfterExp(0,(unsigned char*)image_.ptr(),width_ * height_ * bpp_ );
    if (stat != ASI_SUCCESS) {
        eclogf(ERROR,"GetImage failed - %d\n",stat);
        return 1;
    }
    //cv::Mat 8mat(height, width, CV_16UC1, inputBuffer);
    //im_rgb_ = cv::Mat(height_, width_, CV_16UC3);
    //cv::cvtColor(image_, im_rgb_, cv::COLOR_BayerRG2RGB);
    //im_preview_ = cv::Mat(height_/4, width_/4, CV_8UC3);
    //resize(im_rgb_, im_preview_, im_preview_.size());
    //im_rgb_ = cv::Mat(width_, height_, CV_8UC3);
    //im_rgb16_.convertTo(im_rgb_, CV_8UC3, 1.0/256);
    return 0;
}

std::shared_ptr<AstroImage> ZWOCamera::getAstroImage(){

    std::shared_ptr<AstroImage> newimg = AstroImage::Create(width_,height_,bpp_ * 8);

    ASI_ERROR_CODE stat;
    stat = ASIGetDataAfterExp(0,(unsigned char*)newimg->getBuffer(),width_ * height_ * bpp_ );
    if (stat != ASI_SUCCESS) {
        eclogf(ERROR,"GetImage failed - %d\n",stat);
        return NULL;
    }
    newimg->exposure_ = (float)exposure_time_/1000000.0;

    return std::move(newimg);

}


