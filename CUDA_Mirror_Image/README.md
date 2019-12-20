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

```bash
u6088130@cuda-machine:~/CPP_cuda$ nvprof ./mirror2
==8992== NVPROF is profiling process 8992, command: ./mirror2
==8992== Profiling application: ./mirror2
==8992== Profiling result:
            Type  Time(%)      Time     Calls       Avg       Min       Max  Name
 GPU activities:   54.32%  5.7756ms         1  5.7756ms  5.7756ms  5.7756ms  [CUDA memcpy DtoH]
                   45.50%  4.8376ms         1  4.8376ms  4.8376ms  4.8376ms  [CUDA memcpy HtoD]
                    0.18%  18.944us         1  18.944us  18.944us  18.944us  void cv::cudev::grid_transform_detail::transformSimple<cv::cudev::GlobPtr<uchar3>, uchar4, cv::cudev::BGR_to_BGRA_func<unsigned char>, cv::cudev::WithOutMask>(uchar3, cv::cudev::GlobPtr<cv::cudev::GlobPtr<uchar3>>, uchar4, unsigned char, int, int)
      API calls:   78.31%  266.70ms         2  133.35ms  2.8528ms  263.84ms  cudaMallocPitch
                   18.84%  64.161ms         1  64.161ms  64.161ms  64.161ms  cudaLaunchKernel
                    1.79%  6.0822ms         2  3.0411ms  189.76us  5.8924ms  cudaMemcpy2D
                    0.54%  1.8479ms       191  9.6740us     119ns  468.05us  cuDeviceGetAttribute
                    0.25%  835.38us         2  417.69us  133.97us  701.41us  cuDeviceTotalMem
                    0.16%  535.78us         2  267.89us  63.106us  472.68us  cuDeviceGetName
                    0.11%  379.03us         2  189.51us  163.57us  215.46us  cudaFree
                    0.01%  20.429us         1  20.429us  20.429us  20.429us  cudaDeviceSynchronize
                    0.00%  2.1690us         1  2.1690us  2.1690us  2.1690us  cuInit
                    0.00%  1.8260us         4     456ns     153ns  1.2010us  cuDeviceGetCount
                    0.00%  1.1960us         3     398ns     160ns     712ns  cuDeviceGet
                    0.00%  1.1320us         1  1.1320us  1.1320us  1.1320us  cudaGetDevice
                    0.00%     925ns         1     925ns     925ns     925ns  cudaGetLastError
                    0.00%     489ns         1     489ns     489ns     489ns  cuDriverGetVersion
                    0.00%     437ns         2     218ns     214ns     223ns    p
                    0.00%     230ns         1     230ns     230ns     230ns  cudaGetDeviceCount

```