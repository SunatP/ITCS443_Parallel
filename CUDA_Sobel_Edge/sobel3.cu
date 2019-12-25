#include <iostream>
#include "opencv2/opencv.hpp"
#include "opencv2/core.hpp"
#include "opencv2/highgui.hpp"
#include "opencv2/cudaarithm.hpp"
#include <opencv2/cudaimgproc.hpp>
#include <stdlib.h>
#include <stdio.h>

using namespace cv;

int main( int argc, char** argv )
{

  cv::Mat src, src_gray;
  cv::Mat grad;
  int scale = 1;
  int delta = 0;

  int c;

  /// Load an image
  src = cv::imread( "image.png", cv::COLOR_BGR2GRAY );
  cv::GaussianBlur( src, src, Size(3,3), 0, 0, cv::BORDER_DEFAULT );

  /// Convert it to gray
  // cv::cuda::cvtColor( src, src_gray, cv::COLOR_BGR2GRAY );

  /// Generate grad_x and grad_y
  cv::Mat grad_x, grad_y;
  cv::Mat abs_grad_x, abs_grad_y;

  /// Gradient X
  //Scharr( src_gray, grad_x, ddepth, 1, 0, scale, delta, BORDER_DEFAULT );
//   cv::cuda::createSobelFilter (int srcType, int dstType, int dx, int dy, int ksize=3, double scale=1, int rowBorderMode=BORDER_DEFAULT, int columnBorderMode=-1);
  // cv::cuda::createSobelFilter(CV_8UC4, CV_8UC4, 1, 0, 3, 1, cv::BORDER_DEFAULT);
  cv::Sobel( src_gray, grad_x, CV_16S, 1, 0, 3, scale, delta, BORDER_DEFAULT );
  cv::convertScaleAbs( grad_x, abs_grad_x );

  /// Gradient Y
  //Scharr( src_gray, grad_y, ddepth, 0, 1, scale, delta, BORDER_DEFAULT );
  cv::Sobel( src_gray, grad_y, CV_16S, 0, 1, 3, scale, delta, cv::BORDER_DEFAULT );
  cv::convertScaleAbs( grad_y, abs_grad_y );

  /// Total Gradient (approximate)
  cv::cuda::addWeighted( abs_grad_x, 0.5, abs_grad_y, 0.5, 0, grad );

  cv::imwrite("sobel_out.png", grad);
  return 0;
  }