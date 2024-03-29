
#pragma once

#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>

#include <iostream>
#include <memory>
//#include <zcam.h>

#include <opencv2/opencv.hpp>

typedef enum {
	RAW16_RGGB,
	RAW16_MONO,
	RAW8_RGGB,
	RAW8_MONO
} data_format_t;

#define MAX_RECORDS 36

typedef std::shared_ptr<uint8_t> AstroImageBuffer;
class AstroImage;


class  AstroImage {

public:
	~AstroImage();
	friend class ZWOCamera;

	int32_t getWidth()  {return width_;};
	int32_t getHeight() {return height_;};
	int32_t getDepth()  {return bpp_;};

	// TODO - this needs to go away or offer better protections
	char*   getBuffer()   {return (char*)databuffer_;};
	cv::Mat getMat()    {return cv::Mat(height_, width_,
		                                CV_16UC1, (void*)databuffer_);};
	cv::Mat getMat(float scale);
	cv::Mat getMatRGB(float scale);
	void	printRecords();

	static std::shared_ptr<AstroImage> OpenFITS(const char* fname);
	static std::shared_ptr<AstroImage> Create(int width, int height, int bpp);

	int SaveJpg(const char* fname);

	FILE *infile;
	int id_;

private:
	AstroImage();
	char records_[MAX_RECORDS][80];
	int  num_records_;
	uint8_t* databuffer_;
	uint32_t datasize_;
	uint32_t bpp_;
	uint32_t width_;
	uint32_t height_;
	uint32_t bzero_;
	float xpixsz_;
	float ypixsz_;
	float ccd_temp_;
	float exposure_;
	float gain_;
	char  object_[70];
	char  date_utc_[70];
	char  creator_[70];
	data_format_t data_format;
};

