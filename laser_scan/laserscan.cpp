#include "perception.hpp"
#include <math.h>
#include <vector>
using namespace cv;
using namespace std;


#define PI 3.14159265
const double rover_height;
const double laser_threshold;
const int RESOLUTION_WIDTH = 1280;
const int RESOLUTION_HEIGHT = 720; // 720p
const float focalLength = 2.8;
const int CAMERA_CENTER = 310;
Mat depthmatrix;

class laserscan {
private:
	int y; //which row laser scan is at 
	double theta;
	int sectioning_start = 0;
	int sectioning_end = 0;
	vector<double> expected;
	vector<double> actual;
	
public:
	laserscan(double y_in, int start_in = 0, int end_in = 0) : y(y_in), sectioning_start(start_in), sectioning_end(end_in) {}

	void get_angle() {
		double focal_length = focalLength; // in mm
		double diagonal = sqrt(((double)RESOLUTION_HEIGHT) ^ 2 + ((double)RESOLUTION_WIDTH) ^ 2); // in pixels
		double diagonal_mm = diagonal * 0.2645833333;
		double aov_diagonal = 2 * atan(diagonal_mm / (2 * focal_length)); // in degrees (?)

		double degree_per_pixel = aov_diagonal / diagonal;

		double distrow = abs(CAMERA_CENTER - (double)y);
		theta = 90 - (distrow * degree_per_pixel);
	}
	void get_expected() {
		double expected_depth = rover_height / cos(theta);
		if (sectioning_start < 0 || sectioning_end > depthmatrix.cols) {
			cout << "out of range" << endl;
			exit(1);
		} // remove if sure it won't go out of range
		if (sectioning_start == 0 && sectioning_end == 0) {
			for (int i = 0; i < depthmatrix.cols; ++i) {
				expected.push_back(expected_depth);
			}
		}
		else {
			for (int i = sectioning_start; i < sectioning_end; ++i) {
				expected.push_back(expected_depth);
			}
		}


	}
	void get_actual() {
		if (sectioning_start < 0 || sectioning_end > depthmatrix.cols) {
			cout << "out of range" << endl;
			exit(1);
		} // remove if sure it won't go out of range
		if (sectioning_start == 0 && sectioning_end == 0) {
			for (int i = 0; i < depthmatrix.cols; ++i) {
				float d = depthmatrix.at<float>(y, i);
				actual.push_back((double)d);
			}
		}
		else {
			for (int i = sectioning_start; i < sectioning_end; ++i) {
				float d = depthmatrix.at<float>(y, i);
				actual.push_back((double)d);
			}
		}
	}
	bool check_window() {
		int failcounter = 0;
		int testcounter = 0;
		for (int i = 0; i < (int)actual.size(); ++i) {
			if (failcounter != 0) {
				if (failcounter >= 16) {
					return false;
				}
				if (abs(actual[i] - expected[i]) >= laser_threshold) {
					++failcounter;
				}
				else {

				}
			}
			else {
				if (abs(actual[i] - expected[i]) >= laser_threshold) {
					++failcounter;
				}
			}
		}
		 // have a vector of what obstacles there are (boundaries)
		// fail counter >= 16
		// testcounter 
		// need to think of way to account for noise
		// need to get column values
		
	}
}
