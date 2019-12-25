#include <iostream>
#include <cuda_runtime.h>
// #include <opencv2/core/core.hpp>
// #include <opencv2/highgui/highgui.hpp>
// #include <opencv2/opencv.hpp>
#include "opencv2/opencv.hpp"
#include "opencv2/core.hpp"
#include "opencv2/highgui.hpp"
#include "opencv2/cudaarithm.hpp"
#include <vector>
#include <stdio.h>
#include <cuda_runtime.h>

using namespace std;
using namespace cv;

size_t numRows, numCols;

/* Mirror operations */

__global__ 
void mirror(const uchar4* const inputChannel, uchar4* outputChannel, int numRows, int numCols, bool vertical)
{
  int col = blockIdx.x * blockDim.x + threadIdx.x;
  int row = blockIdx.y * blockDim.y + threadIdx.y;

  if ( col >= numCols || row >= numRows )
  {
   return;
  }

  if(!vertical)
  { 
  
    int thread_x = blockDim.x * blockIdx.x + threadIdx.x;
    int thread_y = blockDim.y * blockIdx.y + threadIdx.y;
    
    int thread_x_new = thread_x;
    int thread_y_new = numRows-thread_y;

    int myId = thread_y * numCols + thread_x;
    int myId_new = thread_y_new * numCols + thread_x_new;
    outputChannel[myId_new] = inputChannel[myId];
   	
  }

  else
  {
  	int thread_x = blockDim.x * blockIdx.x + threadIdx.x;
    int thread_y = blockDim.y * blockIdx.y + threadIdx.y;
    
    int thread_x_new = numCols-thread_x;
    int thread_y_new = thread_y;

    int myId = thread_y * numCols + thread_x;
    int myId_new = thread_y_new * numCols + thread_x_new;
  
  	outputChannel[myId_new] = inputChannel[myId];  // linear data store in global memory	
  }
}         



uchar4* mirror_ops(uchar4 *d_inputImageRGBA, size_t numRows, size_t numCols, bool vertical)
{
	//Set reasonable block size (i.e., number of threads per block)
    const dim3 blockSize(4,4,1);
    //Calculate Grid SIze
    int a=numCols/blockSize.x, b=numRows/blockSize.y;	
    const dim3 gridSize(a+1,b+1,1);

    const size_t numPixels = numRows * numCols;

    uchar4 *d_outputImageRGBA;
    cudaMalloc(&d_outputImageRGBA, sizeof(uchar4) * numPixels);

    //Call mirror kernel.
    mirror<<<gridSize, blockSize>>>(d_inputImageRGBA, d_outputImageRGBA, numRows, numCols, vertical);

    cudaDeviceSynchronize(); 
    
    //Initialize memory on host for output uchar4*
    uchar4* h_out;
    h_out = (uchar4*)malloc(sizeof(uchar4) * numPixels);

    //copy output from device to host
    cudaMemcpy(h_out, d_outputImageRGBA, sizeof(uchar4) * numPixels, cudaMemcpyDeviceToHost);
    
    //cleanup memory on device
    cudaFree(d_inputImageRGBA);
    cudaFree(d_outputImageRGBA);

    //return h_out
        return h_out;
}


void loadImageRGBA(string &filename, uchar4 **imagePtr,size_t *numRows, size_t *numCols)
{
    // loading the image
    cv::Mat image = imread(filename.c_str(), cv::IMREAD_ANYCOLOR | cv::IMREAD_ANYDEPTH);

    // forming a 4-channel(RGBA) image.
    cv::Mat imageRGBA;
    cvtColor(image, imageRGBA, cv::COLOR_BGR2BGRA);

    *imagePtr = new uchar4[image.rows * image.cols];
    unsigned char *cvPtr = imageRGBA.ptr<unsigned char>(0);
    for(size_t i = 0; i < image.rows * image.cols; ++i)
    {
        (*imagePtr)[i].x = cvPtr[4*i + 0];
        (*imagePtr)[i].y = cvPtr[4*i + 1];
        (*imagePtr)[i].z = cvPtr[4*i + 2];
        (*imagePtr)[i].w = cvPtr[4*i + 3];
    }
    *numRows = image.rows;
    *numCols = image.cols;
}

void saveImageRGBA(uchar4* image, string &output_filename,size_t numRows, size_t numCols)
{
    // Forming the Mat object from uchar4 array.
    int sizes[2] = {numRows, numCols};
    Mat imageRGBA(2, sizes, CV_8UC4, (void *)image);
    // Converting back to BGR system
    Mat imageOutputBGR;
    cvtColor(imageRGBA, imageOutputBGR, cv::COLOR_BGR2BGRA);
    // Writing the image
    imwrite(output_filename.c_str(), imageOutputBGR);
}

uchar4* load_image_in_GPU(string filename)
{ // Load the image into main memory
  uchar4 *h_image, *d_in;
  loadImageRGBA(filename, &h_image, &numRows, &numCols);
  // Allocate memory to the GPU
  cudaMalloc((void **) &d_in, numRows * numCols * sizeof(uchar4));
  cudaMemcpy(d_in, h_image, numRows * numCols * sizeof(uchar4), cudaMemcpyHostToDevice);
  // No need to keep this image in RAM now.
  free(h_image);
  return d_in;
}




int main(int argc, char **argv)
{
    // variables_map vm;
    string input_file = "image.png";
    string output_file = "output.png";
    
    uchar4 *d_in = load_image_in_GPU(input_file);
    uchar4 *h_out = NULL;

    h_out = mirror_ops(d_in, numRows, numCols, true);

    cudaFree(d_in);
    if(h_out != NULL)
    {
        saveImageRGBA(h_out, output_file, numRows, numCols); 
    }
        
}