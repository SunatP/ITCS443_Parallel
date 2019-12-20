#include <iostream>
#include "opencv2/opencv.hpp"
#include "opencv2/core.hpp"
#include "opencv2/highgui.hpp"
#include "opencv2/cudaarithm.hpp"

using namespace cv;

int main (int argc, char* argv[])
{
    try
    {   
        cv::Mat src_host = cv::imread("image.png", cv::IMREAD_ANYCOLOR | cv::IMREAD_ANYDEPTH);
        cv::flip(src_host,src_host,+1);
        cv::Mat sobelx;
        cv::cuda::GpuMat dst, src;
	    cv::Sobel(src_host, sobelx, CV_32F,1,0);
        src.upload(sobelx);

        // cv::cuda::Sobel(src, dst, cv::CV_16S,0,1,3,1,0,BORDER_DEFAULT);
        cv::Mat result_host(sobelx);
//        cv::imshow("Result", result_host);
  //      cv::waitKey();
    	cv::imwrite("imgsobel.png",sobelx);
    }
    catch(const cv::Exception& ex)
    {
        std::cout << "Error: " << ex.what() << std::endl;
    }
    return 0;
}
