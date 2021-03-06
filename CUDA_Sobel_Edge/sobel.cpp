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
        cv::cuda::GpuMat dst, src;
        src.upload(src_host);
        cv::Ptr<cv::cuda::Filter> filterx,filtery,filterxy;
        // cv::flip(src_host,sobel_dst,+1);
        // cv::Mat sobelx;
	    filterx = cv::cuda::createSobelFilter(CV_8UC1,CV_8UC1,1,0);
        filterx -> apply(src_host,dst);
        // cv::cuda::Sobel(src, dst, cv::CV_16S,0,1,3,1,0,BORDER_DEFAULT);
        cv::Mat result_host(dst);
        dst.download(src_host);
//        cv::imshow("Result", result_host);
  //      cv::waitKey();
    	cv::imwrite("imgsobel.png",src_host);
    }
    catch(const cv::Exception& ex)
    {
        std::cout << "Error: " << ex.what() << std::endl;
    }
    return 0;
}
