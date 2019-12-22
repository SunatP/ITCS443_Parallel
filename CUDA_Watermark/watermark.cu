#include "opencv2/imgcodecs.hpp"
#include "opencv2/highgui.hpp"
#include "opencv2/opencv.hpp"
#include "opencv2/core.hpp"
#include "opencv2/cudaarithm.hpp"
#include <iostream>

using namespace cv;
using namespace std;

int main( void )
{
    try
{
   double alpha = 0.5;
   cv::Mat src1 = imread( "DSC_0174.jpg" ,cv::IMREAD_ANYCOLOR | cv::IMREAD_ANYDEPTH);
   cv::Mat src2 = imread( "image.png" ,cv::IMREAD_ANYCOLOR | cv::IMREAD_ANYDEPTH);
   cv::cuda::GpuMat dst, src,src_1,roi,watermarked_image;
   cv::Mat aum_resize;
  
//    src2.convertTo(src2, CV_32FC3);
   Size size(src2.cols,src2.rows);
   cv::resize(src1,aum_resize,size);
//    aum_resize.convertTo(aum_resize, CV_32FC3);
    cv::Mat ouImage = Mat::zeros(aum_resize.size(), aum_resize.type());
    // cv::cuda::multiply(1, aum_resize, aum_resize); 
    // cv::cuda::multiply(0.2, src2, src2); 
    // cv::cuda::add( aum_resize, src2, ouImage);
    cv::cuda::addWeighted(src2, alpha, aum_resize, 1.0-alpha, 0.0, watermarked_image);
    cv::Mat result_host(watermarked_image);
//   imshow( "Linear Blend", dst );
   imwrite("watermark.png", result_host);
   printf("Image Out OK");
}
catch(const cv::Exception& ex)
{
    std::cout << "Error: " << ex.what() << std::endl;
}
   return 0;
}
/**
int main(int argc, char** argv)
{
 
    // Read the images
    Mat foreground = imread("puppets.png");
    Mat background = imread("ocean.png");
    Mat alpha = imread("puppets_alpha.png");
     
    // Convert Mat to float data type
    foreground.convertTo(foreground, CV_32FC3);
    background.convertTo(background, CV_32FC3);
    
    // Normalize the alpha mask to keep intensity between 0 and 1
    alpha.convertTo(alpha, CV_32FC3, 1.0/255); // 
 
    // Storage for output image
    Mat ouImage = Mat::zeros(foreground.size(), foreground.type());
 
    // Multiply the foreground with the alpha matte
    multiply(alpha, foreground, foreground); 
 
    // Multiply the background with ( 1 - alpha )
    multiply(Scalar::all(1.0)-alpha, background, background); 
 
    // Add the masked foreground and background.
    add(foreground, background, ouImage); 
     
    // Display image
    imshow("alpha blended image", ouImage/255);
    waitKey(0);
     
    return 0;
}
**/