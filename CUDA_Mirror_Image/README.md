# CUDA Mirror Image by using OpenCV


```C++
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
        cv::flip(src_host,src_host,+1); // Mirror image by using OpenCV flip method
        cv::cuda::GpuMat dst, src; // CUDA function by using GPU Matrix with dst and source
        src.upload(src_host); // upload image to GPU
        cuda::cvtColor(src,dst, cv::COLOR_BGR2BGRA); // Convert to normal color 
        cv::Mat result_host(dst); // use OpenCV matrix for copy image to host
        // cv::imshow("Result", result_host); Cuda doesn't has GUI don't use it.
       // cv::waitKey(); 
        cv::imwrite("img.png",result_host); // write image instead using imshow
    }
    catch(const cv::Exception& ex)
    {
        std::cout << "Error: " << ex.what() << std::endl;
    }
    return 0;
}

```

## วิธีคอมไพล์

```bash
nvcc cuda_mirror.cpp -o cuda_mirror `pkg-config --cflags --libs opencv` -std=c++11
# หรือ
gcc cuda_mirror.cpp -o cuda_mirror `pkg-config --cflags --libs opencv` -std=c++11
```