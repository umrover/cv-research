#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/opencv.hpp>
#include <iostream>
#include "tennis_ball_detection.h"

using namespace cv;
using namespace std;

Mat greenFilter(const Mat& src) {
	assert(src.type() == CV_8UC3);

	Mat greenOnly;
	// 36 170 80
	Scalar lowerb = Scalar(26, 50, 150);
	Scalar upperb = Scalar(60, 210, 255);
	inRange(src, lowerb, upperb, greenOnly);

	return greenOnly;
}

vector<Point2f> findTennisBall(Mat &src) {

	Mat hsv;

	cvtColor(src, hsv, COLOR_BGR2HSV);

	Mat mask = greenFilter(hsv);

	// smoothing
	//medianBlur(mask, mask, 11);
	Size ksize(5, 5);
	GaussianBlur(mask, mask, ksize, 1, 1, BORDER_DEFAULT);

	vector<vector<Point> > contours;
	vector<Vec4i> hierarchy;

	/// Find contours
	findContours(mask, contours, hierarchy, RETR_TREE, CHAIN_APPROX_SIMPLE, Point(0, 0));

	vector<vector<Point> > contours_poly(contours.size());
	vector<Point2f>center(contours.size());
	vector<float>radius(contours.size());

	for (unsigned i = 0; i < contours.size(); i++) {
		approxPolyDP(Mat(contours[i]), contours_poly[i], 3, true);
		minEnclosingCircle((Mat)contours_poly[i], center[i], radius[i]);
	}

	/// Draw polygonal contour + bonding rects + circles
	//Mat drawing = Mat::zeros( mask.size(), CV_8UC3);
	for (unsigned i = 0; i < contours.size(); i++) {
		Scalar color = Scalar(0, 0, 255);
		drawContours(src, contours_poly, i, color, 1, 8, vector<Vec4i>(), 0, Point());
		circle(src, center[i], (int)radius[i], color, 2, 8, 0);
	}

	return center;
}
