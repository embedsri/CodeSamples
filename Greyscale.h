/*
 * Greyscale.h
 *
 *  Created on: Sep 13, 2018
 *      Author: techsri
 */

#ifndef GREYSCALE_H_
#define GREYSCALE_H_
#include <opencv2/opencv.hpp>
using namespace cv;
using namespace std;

class Greyscale {
private:
	std::vector<int> stdfutureInt={};
public:
	Mat greyScaleImage;
	Greyscale();
	virtual ~Greyscale();
	long long energy();
};

#endif /* GREYSCALE_H_ */
