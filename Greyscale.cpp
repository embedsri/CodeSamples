///*
// * Greyscale.cpp
// *
// *  Created on: Sep 13, 2018
// *      Author: techsri
// */
//


#include "Greyscale.h"
#include <future>


static long long square(int x) 
{
	     return x * x;
}
/* Minimum number of elements for multithreaded algorithm.
   Less than this and the algorithm is executed on single thread. */
static const int MT_MIN_SIZE = 128;
en
// Multithreaded implementation of standard library accumulate as in Antony Williams' Concurrency in Action.
template <typename InputIt, typename T>
    auto parallel_accumulate(InputIt first, InputIt last, T init)
{
    // Determine total size.
    const auto size = std::distance(first, last);
    // Determine how many threads the work shall be split into.
    const auto parts = (size < MT_MIN_SIZE)? 1 : std::thread::hardware_concurrency();

    std::vector<std::future<T>> futures;

    // For each part, calculate size and run accumulate on a separate thread.
    for (std::size_t i = 0; i != parts; ++i) 
    {
        const auto part_size = (size * i + size) / parts - (size * i) / parts;
        futures.emplace_back(std::async(std::launch::async,
            [=] { return std::accumulate(first, std::next(first, part_size), T{}); }));
        std::advance(first, part_size);
    }

    // Wait for all threads to finish execution and accumulate results.
    return std::accumulate(std::begin(futures), std::end(futures), init,
        [] (const T prev, auto& future) { return prev + future.get(); });
}
Greyscale::Greyscale() 
{
	 string ImageFile;
	 cout << "Please enter image name: ";
	 cin>>ImageFile;
	 greyScaleImage = cv::imread(ImageFile,cv::IMREAD_GRAYSCALE);
	 // Be sure that the image is loaded
	  if (greyScaleImage.empty())
	  {
	      // No image loaded
		  cout <<"No such image exists";
		  exit(-1);
	  }
}

Greyscale::~Greyscale() 
{
	// TODO Auto-generated destructor stub
}

long long Greyscale::energy()
{
	  for (int y = 0; y<greyScaleImage.rows; y++)
	    {
	     for (int x = 0; x<greyScaleImage.cols; x++)
	    	volatile unsigned char pixel_value = (unsigned char)greyScaleImage.at<uchar>(y, x);
	        // Ensure async function forces GCC to run square function asynchronously on multiple cores.
	        auto a = async(launch::async,square, pixel_value);
                //Store the result value of the asynchronous task
	        int v = a.get();
                //Append the std::future<int> return value into a Vector.            
                stdfutureInt.push_back(v);
	     }
	    }

	  parallel_accumulate(stdFutureInt.begin(),stdFutureInt.end(),0);
}
int main()
{

 const int kernelWidth=9;
 const int kernelHeight=9;

 Greyscale g;

 Mat openCvGaussianFilterImage(g.greyScaleImage.size(),CV_8UC1);

 //Apply low pass  GaussianBlur filter using inbuilt openCv function with kernel size 9x9.
 GaussianBlur(g.greyScaleImage, openCvGaussianFilterImage, Size( kernelWidth, kernelHeight ), 0 ,0 );

 //Calculate energy of the image.
 cout<<"Total energy of the image is:"<< g.energy()<<endl;
 namedWindow("Original Image",1);
 imshow("Original Image",g.greyScaleImage);

 namedWindow("openCvGaussianFilterImage",1);
 imshow("openCvGaussianFilterImage",openCvGaussianFilterImage);
 waitKey();
 return 0;
}
