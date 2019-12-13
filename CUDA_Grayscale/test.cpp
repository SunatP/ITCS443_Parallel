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
        //cv::cuda::threshold(src, dst, 200.0, 255.0, cv::THRESH_BINARY);
        cuda::cvtColor(src,dst, cv::COLOR_BGR2GRAY);
	cv::Mat result_host(dst);
//        cv::imshow("Result", result_host);
  //      cv::waitKey();
    	cv::imwrite("img.png",result_host);
    }
    catch(const cv::Exception& ex)
    {
        std::cout << "Error: " << ex.what() << std::endl;
    }
    return 0;
}
