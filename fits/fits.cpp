
#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <fstream>

#include "fits.h"


/* TODOS

	Bury constructor and use refcounting
		may be challenging when outside entities want to use underlying data.

*/
Fits::Fits(void){};
Fits::~Fits(void){
	delete[] databuffer_;
};



#define PROC_STR_TOKEN(tkn,field) \
	if(!strncmp(token,tkn,strlen(tkn))) { \
		token=strtok(NULL,delim2); \
		if (token) { \
			strncpy(field,token,strlen(token)); \
			field[strlen(token)] = 0; \
		} \
		continue; \
	}
#define PROC_INT_TOKEN(tkn,field) \
	if(!strncmp(token,tkn,strlen(tkn))) { \
		token=strtok(NULL,delim2); \
		if (token) field = (uint32_t)atoi(token); \
		continue; \
	}
#define PROC_FL_TOKEN(tkn,field) \
	if(!strncmp(token,tkn,strlen(tkn))) { \
		token=strtok(NULL,delim2); \
		if (token) field = atof(token); \
		continue; \
	}
std::shared_ptr<Fits> Fits::Open(const char* fname){

	const char delim[2]="=";
	const char delim2[3]="/\0";
	char * token;

	std::shared_ptr<Fits> newfit;

	newfit.reset(new Fits());

	std::ifstream infile(fname);
	if (!infile.is_open())
		return NULL;

	newfit->num_records_ = 0;

	infile.read((char*)newfit->records_,sizeof(newfit->records_));

	for (int i=0; i < MAX_RECORDS; i++) {
		char tempstr[81];
		newfit->records_[i][79] = 0;
		strncpy(tempstr,newfit->records_[i],80);
		tempstr[80]=0;
		token=strtok(tempstr, delim);
//Tokens Required in every fits file
		PROC_INT_TOKEN("BITPIX", newfit->bpp_)
		PROC_INT_TOKEN("NAXIS1", newfit->width_)
		PROC_INT_TOKEN("NAXIS2", newfit->height_)
//Optional tokens
		PROC_INT_TOKEN("BZERO" , newfit->bzero_)
		PROC_STR_TOKEN("DATE-OBS", newfit->date_utc_)
		PROC_FL_TOKEN("EXPOSURE", newfit->exposure_)
		PROC_FL_TOKEN("XPIXSZ", newfit->xpixsz_)
		PROC_FL_TOKEN("YPIXSZ", newfit->ypixsz_)
		PROC_FL_TOKEN("CCD-TEMP", newfit->ccd_temp_)
		PROC_FL_TOKEN("GAIN", newfit->gain_)
//END token (always present)
		if(!strncmp(token,"END",3)) break;
		newfit->num_records_++;
	}

	newfit->datasize_ = newfit->width_ * newfit->height_ * (newfit->bpp_ / 8);
	newfit->databuffer_ = new uint8_t[newfit->datasize_];

	infile.read((char*)newfit->databuffer_, newfit->datasize_);
	uint32_t n = infile.gcount();
	if (n != newfit->datasize_) {
		return NULL;
	}
//TODO - more robust endian swap (FITS is big endian)
	uint16_t *databuff = (uint16_t*)newfit->databuffer_;
	for (int x=0; x< newfit->datasize_/(newfit->bpp_/8); x++) {
		uint16_t temp = databuff[x];
		databuff[x] = (((temp << 8)&0xff00) + (temp >>8)) - newfit->bzero_;
	}
	return std::move(newfit);
}

void Fits::printRecords() {
	printf("Capture date (UTC): %s\n", date_utc_);
	printf("Image size:         %d x %d\n", width_, height_);
	printf("Bits per pixel:     %d\n", bpp_);
	printf("Sensor temp (C):    %6.1f\n", ccd_temp_);
	printf("Pixel size:         %4.2f x %4.2f\n", xpixsz_, ypixsz_);
	printf("Exposure time (s):  %6.2f\n", exposure_);

}

cv::Mat Fits::getMat(float scale) {

	cv::Mat im_preview_ = cv::Mat(static_cast<int>(height_ * scale),
								  static_cast<int>(width_ * scale),
								  CV_8UC3);
  	resize(getMat(), im_preview_, im_preview_.size());
  	return im_preview_;
}

cv::Mat Fits::getMatRGB(float scale) {
 	cv::Mat im_rgb = cv::Mat(height_, width_, CV_16UC3);
  	cv::cvtColor(getMat(), im_rgb, cv::COLOR_BayerRG2RGB);

	cv::Mat im_out = cv::Mat(static_cast<int>(height_ * scale),
							 static_cast<int>(width_ * scale),
							 CV_8UC3);
  	resize(im_rgb, im_out, im_out.size());
  	return im_out;
}



