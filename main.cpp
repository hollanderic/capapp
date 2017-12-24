
#include<opencv2/opencv.hpp>
#include<fits.h>

#define w 400

using namespace cv;
#define LINE_8 8


int main(int argc, char** argv ){

  char window[] = "FitsImage";

  auto fit = Fits::Open(argv[1]);
  if (!fit) {
    printf("Could not open file %s!\n",argv[1]);
    return -1;
  }
  printf("opened fit\n");

  //cv::Mat 8mat(height, width, CV_16UC1, inputBuffer);
  int height = fit->getHeight();
  int width = fit->getWidth();

  cv::Mat im_rgb_ = cv::Mat(height, width, CV_16UC3);
  cv::cvtColor(fit->getMat(), im_rgb_, cv::COLOR_BayerRG2RGB);

  //im_rgb16_.convertTo(im_rgb_, CV_8UC3, 1.0/256);

  cv::Mat im_preview_ = cv::Mat(height/4, width/4, CV_8UC3);
  resize(im_rgb_, im_preview_, im_preview_.size());
  imshow( window, im_preview_);
  fit->printRecords();

  waitKey( 0 );
  return(0);
}
