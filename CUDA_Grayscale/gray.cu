#include "kernel_gpu.h"
#include <cuda_runtime_api.h>
#include <cuda.h>
#include "opencv2/opencv.hpp"
#include "opencv2/core.hpp"
#include "opencv2/highgui.hpp"
#include "opencv2/cudaarithm.hpp"
#include <iomanip>
#include <iostream>
#include <math.h>
#include <sys/timeb.h>
#include <time.h>


#define DEBUG_TIME


__global__ void grayscale(float4* imagem, int width, int height)
{
	const int i = blockIdx.x * (blockDim.x * blockDim.y) + blockDim.x * threadIdx.y + threadIdx.x;

	if(i < width * height)
	{
		float v = 0.3 * imagem[i].x + 0.6 * imagem[i].y + 0.1 * imagem[i].z;
		imagem[i] = make_float4(v, v, v, 0);
	}
}


extern "C" void cuda_grayscale(float* imagem, int width, int height, dim3 blocks, dim3 block_size)
{
	grayscale <<< blocks, block_size >>> ((float4*)imagem, width, height);
}

int main(int argc, char** argv)
{
    if (argc < 3)
    {
        std::cout << "Usage: " << argv[0] << " <input.png>" << " <output.png>" << std::endl;
        exit(1);
    }

    IplImage* input_image = NULL;
	input_image = cvLoadImage("image.png", CV_LOAD_IMAGE_UNCHANGED);
    if(!input_image)
    {
        std::cout << "ERROR: Failed to load input image" << std::endl;
        return -1;
    }

	int width = input_image->width;
    int height = input_image->height;
    int bpp = input_image->nChannels;

#ifdef DEBUG
	std::cout << ">> Width:" << width << std::endl <<
		         ">> Height:" << height << std::endl <<
				 ">> Bpp:" << bpp << std::endl;

    std::cout << std::endl << ">>> Debugging Original data:" << std::endl;
    for (int i=0; i < width*height*bpp; i+=bpp)
    {
        if (!(i % (width*bpp)))
            std::cout << std::endl;

        std::cout << std::dec << "R:"<< (int) input_image->imageData[i] <<
                                 " G:" << (int) input_image->imageData[i+1] <<
                                 " B:" << (int) input_image->imageData[i+2] << "     ";
    }
    std::cout << std::endl << std::endl;
#endif

    float* cpu_image = new float[width * height * 4];
    if (!cpu_image)
    {
        std::cout << "ERROR: Failed to allocate memory" << std::endl;
        return -1;
    }

	for (int i = 0; i < width * height; i++)
	{
		cpu_image[i * 4 + 0] = (unsigned char)input_image->imageData[i * bpp + 0] / 255.f;
		cpu_image[i * 4 + 1] = (unsigned char)input_image->imageData[i * bpp + 1] / 255.f;
		cpu_image[i * 4 + 2] = (unsigned char)input_image->imageData[i * bpp + 2] / 255.f;
	}

#ifdef DEBUG_TIME
    //Start clock
    struct timeb start_time_st;
    ftime(&start_time_st);
#endif

    float* gpu_image = NULL;
	cudaError_t cuda_err = cudaMalloc((void **)(&gpu_image), (width * height * 4) * sizeof(float));
    if (cuda_err != cudaSuccess)
    {
        std::cout << "ERROR: Failed cudaMalloc" << std::endl;
        return -1;
    }

	cuda_err = cudaMemcpy(gpu_image, cpu_image, (width * height * 4) * sizeof(float), cudaMemcpyHostToDevice);
    if (cuda_err != cudaSuccess)
    {
        std::cout << "ERROR: Failed cudaMemcpy" << std::endl;
        return -1;
    }

	dim3 block(16, 16);
	dim3 grid((int)ceil(double((width * height) / 256.0)));

    cuda_grayscale(gpu_image, width, height, grid, block);

	cudaMemcpy(cpu_image, gpu_image, (width * height * 4) * sizeof(float), cudaMemcpyDeviceToHost);
    if (cuda_err != cudaSuccess)
    {
        std::cout << "ERROR: Failed cudaMemcpy" << std::endl;
        return -1;
    }

#ifdef DEBUG_TIME
    // Stop clock
    struct timeb stop_time_st;
    ftime(&stop_time_st);
    double elapsed = ((double) stop_time_st.time + ((double) stop_time_st.millitm * 0.001)) -
                     ((double) start_time_st.time + ((double) start_time_st.millitm * 0.001));

	std::cout << "* Time elapsed: " << std::setprecision(5) << elapsed << " sec" << std::endl;
#endif

    cuda_err = cudaFree(gpu_image);
    if (cuda_err != cudaSuccess)
    {
        std::cout << "ERROR: Failed cudaFree" << std::endl;
        return -1;
    }

	char* buff = new char[width * height * bpp];
    if (!buff)
    {
        std::cout << "ERROR: Failed to allocate memory" << std::endl;
        return -1;
    }


	for (int i = 0; i < (width * height); i++)
	{
		buff[i * bpp + 0] = (char)floor(cpu_image[i * 4 + 0] * 255.f);
		buff[i * bpp + 1] = (char)floor(cpu_image[i * 4 + 1] * 255.f);
		buff[i * bpp + 2] = (char)floor(cpu_image[i * 4 + 2] * 255.f);
	}

#ifdef DEBUG
    std::cout << std::endl << ">>> Debugging Output data:" << std::endl;
    for (int i=0; i < width*height*bpp; i+=bpp)
    {
        if (!(i % (width*bpp)))
            std::cout << std::endl;

        std::cout << std::dec << "R:"<< (int) buff[i] <<
                                 " G:" << (int) buff[i+1] <<
                                 " B:" << (int) buff[i+2] << "     ";
    }
    std::cout << std::endl << std::endl;
#endif

	IplImage* out_image = cvCreateImage(cvSize(width, height), input_image->depth, bpp);
    if (!out_image)
    {
        std::cout << "ERROR: Failed cvCreateImage" << std::endl;
        return -1;
    }

	out_image->imageData = buff;

	if (!cvSaveImage(argv[2], out_image))
    {
        std::cout << "ERROR: Failed cvSaveImage" << std::endl;
    }

	cvReleaseImage(&input_image);
    cvReleaseImage(&out_image);


    delete[] cpu_image;
    delete[] buff;

	return 0;
}

