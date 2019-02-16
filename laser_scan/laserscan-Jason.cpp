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
	struct obstacle {
		int start_boundary = 0;
		int end_boundary = 0;
		double closest_dist = 0;
	};
	int y; //which row laser scan is at 
	double theta;
	int sectioning_start = 0;
	int sectioning_end = 0;
	vector<double> expected;
	vector<double> actual;
	vector<obstacle> obstacle_boundaries;
	
	
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
	void check_window() {
		int failcounter = 0;
		int testcounter = 0;
		//bool start_boundary_found = false;
		
		for (int i = 0; i < (int)actual.size(); ++i) {
			int start_obstacle = 0;
			int end_obstacle = 0;
			double closest_dista = 9999999999999.0;
			if (abs(actual[i] - expected[i]) >= laser_threshold) {
				failcounter = 0;
				for (int checking = i; checking < i + 20; ++checking) {
					if (abs(actual[checking] - expected[checking]) >= laser_threshold) {
						++failcounter;
					}
				}
				if (failcounter >= 16) {
					//start_boundary_found = true;
					for (int findend = i + 16; findend < (int)actual.size(); ++findend) {
						if (abs(actual[findend] - expected[findend]) < laser_threshold && abs(actual[findend + 1] - expected[findend + 1]) < laser_threshold
							&& abs(actual[findend + 2] - expected[findend + 2]) < laser_threshold) {
							end_obstacle = findend - 1;
							break;
						}
					}
				}
			}
			if (end_obstacle != 0) {
				obstacle avoid_this;
				avoid_this.start_boundary = start_obstacle;
				avoid_this.end_boundary = end_obstacle;
				
				
				for (int finddist = start_boundary; finddist <= end_obstacle; ++finddist) {
					if (depthmatrix.at<float>(y, finddist) < closest_dista) {
						closest_dista = depthmatrix.at<float>(y, finddist); // float to double conversion?
					}
				}
				avoid_this.closest_dist = closest_dista;
				obstacle_boundaries.push_back(avoid_this);
				i = end_obstacle;
			}
		}
		 // have a vector of what obstacles there are (boundaries)
		// fail counter >= 16
		// testcounter 
		// need to think of way to account for noise
		// need to get column values
		
	}
}
